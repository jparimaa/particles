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

static const int MATRIX_SIZE = 16 * sizeof(float);

void particle_renderer_init(ParticleRenderer* particleRenderer, int maxParticleCount)
{
    // clang-format off
    float vertices[] = {
        0.5f, -0.5f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        0.5f, 0.5f, 0.0f, // top right
		-0.5f, 0.5f, 0.0f // top left
    };
    // clang-format on

    // Create buffer
    glGenVertexArrays(1, &particleRenderer->VAO);
    glBindVertexArray(particleRenderer->VAO);

    glGenBuffers(1, &particleRenderer->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, particleRenderer->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    // Create shader
    const char* files[] = {CREATE_PATH(SHADER_PATH, "simple.vert"), CREATE_PATH(SHADER_PATH, "simple.frag")};
    particleRenderer->shader = shader_createProgram(2, files);

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
        glGenTextures(1, &particleRenderer->sprite);
        glBindTexture(GL_TEXTURE_2D, particleRenderer->sprite);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    stbi_image_free(imageData);

    // Create wvp buffer
    glGenBuffers(1, &particleRenderer->wvpBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->wvpBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleRenderer->wvpBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, maxParticleCount * MATRIX_SIZE, NULL, GL_MAP_WRITE_BIT);
}

void particle_renderer_deinit(ParticleRenderer* particleRenderer)
{
    glDeleteTextures(1, &particleRenderer->sprite);
    glDeleteVertexArrays(1, &particleRenderer->VAO);
    glDeleteBuffers(1, &particleRenderer->VBO);
    glDeleteBuffers(1, &particleRenderer->wvpBuffer);
    shader_deleteProgram(particleRenderer->shader);
}

void particle_renderer_update(ParticleRenderer* particleRenderer, const Camera* camera, const Particle* particles, int particleCount)
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->wvpBuffer);
    GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
    mat4* wvpBuffer = (mat4*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, particleCount * MATRIX_SIZE, access);

    mat4 viewMatrix;
    camera_getViewMatrix(camera, viewMatrix);
    mat4 projectionMatrix;
    camera_getProjectionMatrix(camera, projectionMatrix);

    for (int i = 0; i < particleCount; ++i)
    {
        const Particle* p = &particles[i];

        mat4 worldMatrix;
        transform_getWorldMatrix(&p->transform, worldMatrix);

        mat4 wvpMatrix;
        glm_mat4_mulN((mat4* []){&projectionMatrix, &viewMatrix, &worldMatrix}, 3, wvpMatrix);

        glm_mat4_copy(wvpMatrix, *wvpBuffer);
        ++wvpBuffer;
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void particle_renderer_render(ParticleRenderer* particleRenderer, int particleCount)
{
    glUseProgram(particleRenderer->shader);
    glBindVertexArray(particleRenderer->VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, particleRenderer->sprite);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleRenderer->wvpBuffer);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);
}
