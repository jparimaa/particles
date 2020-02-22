#include "particle_emitter.h"
#include "helpers.h"
#include "macros.h"
#include "shader.h"

#include <stdlib.h>

void particle_emitter_init(ParticleEmitter* particleEmitter, EmitterParameters* emitterParameters)
{
    particleEmitter->parameters = *emitterParameters;
    particle_emitter_reset(particleEmitter);
}

void particle_emitter_deinit(ParticleEmitter* particleEmitter)
{
}

void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta, ParticleState* states)
{
    EmitterParameters* params = &particleEmitter->parameters;

    particleEmitter->timeSinceLastEmit += timeDelta;
    int spawnCount = (int)(particleEmitter->timeSinceLastEmit / (1.0f / params->emissionRate));

    int lastIndex = particleEmitter->startIndex + particleEmitter->particleCount;
    for (int i = particleEmitter->startIndex; i < lastIndex; ++i)
    {
        ParticleState* state = &states[i];
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
    particleEmitter->particleCount = 0;
    particleEmitter->timeSinceLastEmit = 0.0f;
}
