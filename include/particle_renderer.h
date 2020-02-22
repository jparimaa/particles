#ifndef particle_renderer_h
#define particle_renderer_h

#include "camera.h"
#include "particle_state.h"

#include <glad/glad.h>

struct ParticleRenderer
{
    GLuint VAO;
    GLuint VBO;
    GLuint sprite;
    GLuint shader;
    GLuint particleStateBuffer;
    GLuint particleBuffer;
};
typedef struct ParticleRenderer ParticleRenderer;

void particle_renderer_init(ParticleRenderer* particleRenderer, int maxParticleCount);
void particle_renderer_deinit(ParticleRenderer* particleRenderer);
void particle_renderer_update(ParticleRenderer* particleRenderer, const ParticleState* states, int count);
void particle_renderer_render(ParticleRenderer* particleRenderer, const Camera* camera, int count);

#endif