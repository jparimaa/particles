#ifndef particle_emitter_h
#define particle_emitter_h

#include "particle.h"
#include "camera.h"
#include "particle_renderer.h"

#include <glad/glad.h>

struct ParticleEmitter
{
    int maxParticleCount;
    int particleCount;
    Particle* particles;
    ParticleRenderer particleRenderer;
};
typedef struct ParticleEmitter ParticleEmitter;

void particle_emitter_init(ParticleEmitter* particleEmitter, int maxParticleCount);
void particle_emitter_deinit(ParticleEmitter* particleEmitter);
void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta);
void particle_emitter_render(ParticleEmitter* particleEmitter, const Camera* camera);
void particle_emitter_reset(ParticleEmitter* particleEmitter);

#endif