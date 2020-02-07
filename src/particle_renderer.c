#include "particle_renderer.h"
#include "macros.h"
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#ifdef _MSC_VER
#include <stb_image.h>
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#include <stb_image.h>
#pragma GCC diagnostic pop
#endif

static const int BUFFER_ELEMENT_SIZE = 4 * sizeof(float);

void particle_renderer_init(ParticleRenderer* particleRenderer, int maxParticleCount)
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

    // Create shader
    const char* files[] = {CREATE_PATH(SHADER_PATH, "simple.vert"), CREATE_PATH(SHADER_PATH, "simple.frag")};
    particleRenderer->shader = shader_createProgram(2, files);

    // Load image
    const char* imagePath = CREATE_PATH(ASSET_PATH, "smoke_particle.png");
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

    // Create wvp buffer
    glGenBuffers(1, &particleRenderer->positionAndSizeBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->positionAndSizeBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleRenderer->positionAndSizeBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, maxParticleCount * BUFFER_ELEMENT_SIZE, NULL, GL_MAP_WRITE_BIT);
}

void particle_renderer_deinit(ParticleRenderer* particleRenderer)
{
    glDeleteTextures(1, &particleRenderer->sprite);
    glDeleteVertexArrays(1, &particleRenderer->VAO);
    glDeleteBuffers(1, &particleRenderer->VBO);
    glDeleteBuffers(1, &particleRenderer->positionAndSizeBuffer);
    shader_deleteProgram(particleRenderer->shader);
}

void particle_renderer_update(ParticleRenderer* particleRenderer, const vec4* particleData, int count)
{
    if (count <= 0)
    {
        return;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->positionAndSizeBuffer);
    GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT;
    vec4* positionAndSizeBuffer = (vec4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, count * BUFFER_ELEMENT_SIZE, access);

    memcpy(positionAndSizeBuffer, particleData, count * sizeof(vec4));

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
}

void particle_renderer_render(ParticleRenderer* particleRenderer, const Camera* camera, int count)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);

    glUseProgram(particleRenderer->shader);
    glBindVertexArray(particleRenderer->VAO);

    mat4 viewMatrix;
    mat4 projectionMatrix;
    camera_getViewMatrix(camera, viewMatrix);
    camera_getProjectionMatrix(camera, projectionMatrix);

    mat4 viewProjectionMatrix;
    glm_mat4_mulN((mat4* []){&projectionMatrix, &viewMatrix}, 2, viewProjectionMatrix);

    glUniformMatrix4fv(0, 1, 0, viewProjectionMatrix[0]);
    glUniform3f(1, viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]);
    glUniform3f(2, viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particleRenderer->sprite);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->positionAndSizeBuffer);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, count);
}
