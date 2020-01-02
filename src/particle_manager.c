#include "particle_manager.h"
#include "helpers.h"
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

#include <stdlib.h>

static const int MAX_PARTICLE_COUNT = 100;
static const int MATRIX_SIZE = 16 * sizeof(float);

void particle_manager_init(ParticleManager* particleManager)
{
    particleManager->particleCount = 10;
    particleManager->particles = malloc(sizeof(Particle) * MAX_PARTICLE_COUNT);
    for (int i = 0; i < particleManager->particleCount; ++i)
    {
        particle_init(&particleManager->particles[i]);
        particleManager->particles[i].direction[0] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].direction[1] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].direction[2] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].lifeTime = randomZeroToOne() * 20.0f;
    }

    // clang-format off
    float vertices[] = {
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        0.5f, 0.5f, 0.0f, // top right
		-0.5f, 0.5f, 0.0f // top left
    };
    // clang-format on

    // Create buffer
    glGenVertexArrays(1, &particleManager->VAO);
    glBindVertexArray(particleManager->VAO);

    glGenBuffers(1, &particleManager->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, particleManager->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    // Create shader
    const char* files[] = {CREATE_PATH(SHADER_PATH, "simple.vert"), CREATE_PATH(SHADER_PATH, "simple.frag")};
    particleManager->shader = shader_createProgram(2, files);

    // Load image
    const char* imagePath = CREATE_PATH(ASSET_PATH, "green_square.png");
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
        glGenTextures(1, &particleManager->sprite);
        glBindTexture(GL_TEXTURE_2D, particleManager->sprite);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    stbi_image_free(imageData);

    // Create wvp buffer
    glGenBuffers(1, &particleManager->wvpBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleManager->wvpBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleManager->wvpBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLE_COUNT * MATRIX_SIZE, NULL, GL_MAP_WRITE_BIT);
}

void particle_manager_deinit(ParticleManager* particleManager)
{
    glDeleteTextures(1, &particleManager->sprite);
    glDeleteVertexArrays(1, &particleManager->VAO);
    glDeleteBuffers(1, &particleManager->VBO);
    glDeleteBuffers(1, &particleManager->wvpBuffer);
    shader_deleteProgram(particleManager->shader);
    free(particleManager->particles);
}

void particle_manager_update(ParticleManager* particleManager, float timeDelta, const Camera* camera)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleManager->wvpBuffer);
    GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
    mat4* wvpBuffer = (mat4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_PARTICLE_COUNT * MATRIX_SIZE, access);

    mat4 viewMatrix;
    camera_getViewMatrix(camera, viewMatrix);
    mat4 projectionMatrix;
    camera_getProjectionMatrix(camera, projectionMatrix);

    for (int i = 0; i < particleManager->particleCount; ++i)
    {
        Particle* p = &particleManager->particles[i];
        vec3 movement = GLM_VEC3_ZERO_INIT;
        glm_vec3_scale(p->direction, timeDelta, movement);
        glm_vec3_add(p->transform.position, movement, p->transform.position);

        mat4 worldMatrix;
        transform_getWorldMatrix(&p->transform, worldMatrix);

        mat4 wvpMatrix;
        glm_mat4_mulN((mat4* []){&projectionMatrix, &viewMatrix, &worldMatrix}, 3, wvpMatrix);

        glm_mat4_copy(wvpMatrix, *wvpBuffer);
        ++wvpBuffer;
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void particle_manager_render(ParticleManager* particleManager)
{
    glUseProgram(particleManager->shader);
    glBindVertexArray(particleManager->VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particleManager->sprite);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleManager->wvpBuffer);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleManager->particleCount);
}
