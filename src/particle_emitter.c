#include "particle_emitter.h"
#include "helpers.h"

#include <stdlib.h>

void particle_emitter_init(ParticleEmitter* particleEmitter, EmitterParameters* parameters, ParticleState** states, int startIndex, int count)
{
    particleEmitter->parameters = parameters;
    particleEmitter->particleStates = states;
    particleEmitter->startIndex = startIndex;
    particleEmitter->count = count;
    particle_emitter_reset(particleEmitter);
}

void particle_emitter_deinit(ParticleEmitter* particleEmitter)
{
}

void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta)
{
    EmitterParameters* params = particleEmitter->parameters;

    particleEmitter->timeSinceLastEmit += timeDelta;
    int spawnCount = (int)(particleEmitter->timeSinceLastEmit / (1.0f / params->emissionRate));

    int lastIndex = particleEmitter->startIndex + particleEmitter->count;
    for (int i = particleEmitter->startIndex; i < lastIndex; ++i)
    {
        ParticleState* state = &(*particleEmitter->particleStates)[i];
        state->lifetime -= timeDelta;

        if (state->lifetime < 0.0f)
        {
            state->status = PARTICLE_STATUS_DEAD;
        }

        if (state->status == PARTICLE_STATUS_RESET)
        {
            state->status = PARTICLE_STATUS_ALIVE;
        }

        if (spawnCount > 0 && state->status == PARTICLE_STATUS_DEAD)
        {
            state->status = PARTICLE_STATUS_RESET;
            state->lifetime = randomBetweenFloats(params->particleLifeTime[0], params->particleLifeTime[1]);
            --spawnCount;
            particleEmitter->timeSinceLastEmit = 0.0f;
        }
    }
}

void particle_emitter_reset(ParticleEmitter* particleEmitter)
{
    particleEmitter->timeSinceLastEmit = 0.0f;
}
