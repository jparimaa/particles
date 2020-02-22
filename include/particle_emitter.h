#ifndef particle_emitter_h
#define particle_emitter_h

#include "camera.h"
#include "particle_state.h"
#include "emitter_parameters.h"

#include <glad/glad.h>

#include <stdbool.h>

struct ParticleEmitter
{
    EmitterParameters* parameters;
    int startIndex;
    int particleCount;
    float timeSinceLastEmit;
};
typedef struct ParticleEmitter ParticleEmitter;

void particle_emitter_init(ParticleEmitter* particleEmitter, EmitterParameters* emitterParameters);
void particle_emitter_deinit(ParticleEmitter* particleEmitter);
void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta, ParticleState* states);
void particle_emitter_reset(ParticleEmitter* particleEmitter);

#endif