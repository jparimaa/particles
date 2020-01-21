#ifndef particle_emitter_h
#define particle_emitter_h

#include "particle.h"
#include "camera.h"
#include "particle_renderer.h"
#include "emitter_parameters.h"

#include <glad/glad.h>

struct ParticleEmitter
{
    EmitterParameters parameters;
    Particle* particles;
    int freeIndicesCount;
    int* freeIndices;
    int usedIndicesCount;
    int* usedIndices;
    float timeSinceLastEmit;
    ParticleRenderer particleRenderer;
};
typedef struct ParticleEmitter ParticleEmitter;

void particle_emitter_init(ParticleEmitter* particleEmitter, EmitterParameters* emitterParameters);
void particle_emitter_deinit(ParticleEmitter* particleEmitter);
void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta);
void particle_emitter_render(ParticleEmitter* particleEmitter, const Camera* camera);
void particle_emitter_reset(ParticleEmitter* particleEmitter);
void particle_emitter_emit(ParticleEmitter* particleEmitter);

#endif