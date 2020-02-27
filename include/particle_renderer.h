#ifndef particle_renderer_h
#define particle_renderer_h

#include "camera.h"
#include "particle_state.h"
#include "emitter_parameters.h"

#include <glad/glad.h>

struct ParticleRenderer
{
    GLuint VAO;
    GLuint VBO;
    GLuint sprite;
    GLuint drawShader;
    GLuint updateShader;
    GLuint particleStateBuffer;
    GLuint particleBuffer;
    GLuint emitterParametersBuffer;
};
typedef struct ParticleRenderer ParticleRenderer;

void particle_renderer_init(ParticleRenderer* particleRenderer, int maxParticleCount, EmitterParameters* params, int emitterCount, int* particleCounts);
void particle_renderer_deinit(ParticleRenderer* particleRenderer);
void particle_renderer_update(ParticleRenderer* particleRenderer, const ParticleState* states, int count, float timeDelta);
void particle_renderer_render(ParticleRenderer* particleRenderer, const Camera* camera, int count);

#endif