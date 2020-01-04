#ifndef particle_renderer_h
#define particle_renderer_h

#include "camera.h"
#include "particle.h"

#include <glad/glad.h>

struct ParticleRenderer
{
    GLuint VAO;
    GLuint VBO;
    GLuint sprite;
    GLuint shader;
    GLuint wvpBuffer;
};
typedef struct ParticleRenderer ParticleRenderer;

void particle_renderer_init(ParticleRenderer* particleRenderer, int maxParticleCount);
void particle_renderer_deinit(ParticleRenderer* particleRenderer);
void particle_renderer_update(ParticleRenderer* particleRenderer, const Camera* camera, const Particle* particles, int particleCount);
void particle_renderer_render(ParticleRenderer* particleRenderer, int particleCount);

#endif