#ifndef particle_system_h
#define particle_system_h

#include "particle_emitter.h"
#include "emitter_parameters.h"
#include "particle.h"
#include "particle_renderer.h"
#include "camera.h"

struct ParticleSystem
{
    int maxEmitterCount;
    ParticleEmitter* emitters;
    int emitterCount;
    int maxParticleCount;
    Particle* particles;
    ParticleFlow* particleFlows;
    ParticleRenderer particleRenderer;
};
typedef struct ParticleSystem ParticleSystem;

void particle_system_init(ParticleSystem* system, int maxEmitterCount);
void particle_system_deinit(ParticleSystem* system);
int particle_system_add_emitter(ParticleSystem* system, EmitterParameters* parameters);
void particle_system_finalize(ParticleSystem* system);
void particle_system_update(ParticleSystem* system, float timeDelta);
void particle_system_render(ParticleSystem* system, const Camera* camera);
void particle_system_reset(ParticleSystem* system);

#endif