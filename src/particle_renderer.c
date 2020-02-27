#include "particle_renderer.h"
#include "macros.h"
#include "shader.h"
#include "helpers.h"

#define STB_IMAGE_IMPLEMENTATION
#ifdef _MSC_VER
#include <stb_image.h>
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stb_image.h>
#pragma GCC diagnostic pop
#endif

#include <math.h>

struct Particle
{
    vec4 color;
    vec4 colorChangeRate;
    vec3 position;
    float scale;
    vec3 direction;
    float rotation;
    float acceleration;
    float scalingRate;
    int emitterIndex;
};
typedef struct Particle Particle;

static const int PARTICLE_STATE_SIZE = sizeof(ParticleState);
static const int PARTICLE_SIZE = sizeof(Particle);
static const int EMITTER_PARAMETERS_SIZE = sizeof(EmitterParameters);

void particle_renderer_init(ParticleRenderer* particleRenderer, int maxParticleCount, EmitterParameters* params, int emitterCount, int* particleCounts)
{
    // clang-format off
    float vertices[] = {
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
		-0.5f,  0.5f, 0.0f,   0.0f, 1.0f // top left
    };
    // clang-format on

    // Create buffer
    glGenVertexArrays(1, &particleRenderer->VAO);
    glBindVertexArray(particleRenderer->VAO);

    glGenBuffers(1, &particleRenderer->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, particleRenderer->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLsizei stride = 5 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Create shaders
    const char* drawShaderFiles[] = {CREATE_PATH(SHADER_PATH, "simple.vert"), CREATE_PATH(SHADER_PATH, "simple.frag")};
    particleRenderer->drawShader = shader_createProgram(2, drawShaderFiles);

    const char* updateShaderFile[] = {CREATE_PATH(SHADER_PATH, "update.comp")};
    particleRenderer->updateShader = shader_createProgram(1, updateShaderFile);

    // Load image
    const char* imagePath = CREATE_PATH(ASSET_PATH, "ball.png");
    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc* imageData = stbi_load(imagePath, &width, &height, &channels, 4);
    if (!imageData)
    {
        printf("ERROR: Failed to load image %s\n", imagePath);
    }
    else
    {
        glGenTextures(1, &particleRenderer->sprite);
        glBindTexture(GL_TEXTURE_2D, particleRenderer->sprite);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    stbi_image_free(imageData);

    const GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;

    // Particle state
    glGenBuffers(1, &particleRenderer->particleStateBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->particleStateBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleRenderer->particleStateBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, maxParticleCount * PARTICLE_STATE_SIZE, NULL, GL_MAP_WRITE_BIT);

    // Particles
    glGenBuffers(1, &particleRenderer->particleBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->particleBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particleRenderer->particleBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, maxParticleCount * PARTICLE_SIZE, NULL, GL_MAP_WRITE_BIT);

    Particle* particlesBuffer = (Particle*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, maxParticleCount * PARTICLE_SIZE, access);
    int particleIndex = 0;
    int emitterIndex = 0;
    for (int i = 0; i < emitterCount; ++i)
    {
        for (int j = 0; j < particleCounts[i]; ++j)
        {
            particlesBuffer[particleIndex].emitterIndex = emitterIndex;
            ++particleIndex;
        }
        ++emitterIndex;
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    // Emitter parameters
    glGenBuffers(1, &particleRenderer->emitterParametersBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->emitterParametersBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particleRenderer->emitterParametersBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, emitterCount * EMITTER_PARAMETERS_SIZE, NULL, GL_MAP_WRITE_BIT);

    EmitterParameters* emitterParametersBuffer = (EmitterParameters*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, emitterCount * EMITTER_PARAMETERS_SIZE, access);
    memcpy(emitterParametersBuffer, params, emitterCount * EMITTER_PARAMETERS_SIZE);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
}

void particle_renderer_deinit(ParticleRenderer* particleRenderer)
{
    glDeleteTextures(1, &particleRenderer->sprite);
    glDeleteVertexArrays(1, &particleRenderer->VAO);
    glDeleteBuffers(1, &particleRenderer->VBO);
    glDeleteBuffers(1, &particleRenderer->particleStateBuffer);
    shader_deleteProgram(particleRenderer->drawShader);
    shader_deleteProgram(particleRenderer->updateShader);
}

void particle_renderer_update(ParticleRenderer* particleRenderer, const ParticleState* states, int count, float timeDelta)
{
    if (count <= 0)
    {
        return;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->particleStateBuffer);
    GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
    ParticleState* particleStateBuffer = (ParticleState*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, count * PARTICLE_STATE_SIZE, access);

    memcpy(particleStateBuffer, states, count * PARTICLE_STATE_SIZE);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);

    glUseProgram(particleRenderer->updateShader);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->particleBuffer);
    glUniform1f(0, timeDelta);
    glUniform1i(1, count);
    glUniform4f(2, randomZeroToOne(), randomZeroToOne(), randomZeroToOne(), randomZeroToOne());

    int dispatch = (int)ceil(((double)count / 1024.0));

    glDispatchCompute(dispatch, 1, 1);
}

void particle_renderer_render(ParticleRenderer* particleRenderer, const Camera* camera, int count)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glUseProgram(particleRenderer->drawShader);
    glBindVertexArray(particleRenderer->VAO);

    mat4 viewMatrix;
    mat4 projectionMatrix;
    camera_getViewMatrix(camera, viewMatrix);
    camera_getProjectionMatrix(camera, projectionMatrix);

    mat4 viewProjectionMatrix;
    glm_mat4_mulN((mat4*[]){&projectionMatrix, &viewMatrix}, 2, viewProjectionMatrix);

    glUniformMatrix4fv(0, 1, 0, viewProjectionMatrix[0]);
    glUniform3f(1, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glUniform3f(2, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particleRenderer->sprite);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->particleStateBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->particleBuffer);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
}
