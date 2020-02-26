#ifndef particle_emitter_h
#define particle_emitter_h

#include "particle_state.h"
#include "emitter_parameters.h"

struct ParticleEmitter
{
    EmitterParameters* parameters;
    ParticleState** particleStates;
    int startIndex;
    int count;
    float timeSinceLastEmit;
};
typedef struct ParticleEmitter ParticleEmitter;

void particle_emitter_init(ParticleEmitter* particleEmitter, EmitterParameters* parameters, ParticleState** states, int startIndex, int count);
void particle_emitter_deinit(ParticleEmitter* particleEmitter);
void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta);
void particle_emitter_reset(ParticleEmitter* particleEmitter);

#endif