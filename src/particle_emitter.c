#include "particle_emitter.h"
#include "helpers.h"
#include "macros.h"
#include "shader.h"

#include <stdlib.h>

void particle_emitter_init(ParticleEmitter* particleEmitter, EmitterParameters* emitterParameters)
{
    int maxParticleCount = emitterParameters->maxParticleCount;
    particleEmitter->parameters = *emitterParameters;
    particleEmitter->particles = malloc(sizeof(Particle) * maxParticleCount);
    particleEmitter->freeIndices = malloc(sizeof(int) * maxParticleCount);
    particleEmitter->usedIndices = malloc(sizeof(int) * maxParticleCount);
    particleEmitter->timeSinceLastEmit = 0.0f;

    particle_emitter_reset(particleEmitter);

    particle_renderer_init(&particleEmitter->particleRenderer, maxParticleCount);
}

void particle_emitter_deinit(ParticleEmitter* particleEmitter)
{
    particle_renderer_deinit(&particleEmitter->particleRenderer);
    free(particleEmitter->particles);
    free(particleEmitter->freeIndices);
    free(particleEmitter->usedIndices);
}

void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta)
{
    EmitterParameters* params = &particleEmitter->parameters;

    particleEmitter->timeSinceLastEmit += timeDelta;
    int spawnCount = (int)(particleEmitter->timeSinceLastEmit / (1.0f / params->emissionRate));
    spawnCount = MIN(spawnCount, particleEmitter->freeIndicesCount);
    if (spawnCount > 0)
    {
        particleEmitter->timeSinceLastEmit = 0.0f;
    }

    for (int i = 0; i < spawnCount; ++i)
    {
        particle_emitter_emit(particleEmitter);
    }

    for (int i = 0; i < particleEmitter->usedIndicesCount; ++i)
    {
        Particle* p = &particleEmitter->particles[i];
        vec3 movement = GLM_VEC3_ZERO_INIT;
        glm_vec3_scale(p->direction, timeDelta, movement);
        glm_vec3_add(p->transform.position, movement, p->transform.position);
    }

    particle_renderer_update(&particleEmitter->particleRenderer, particleEmitter->particles, particleEmitter->usedIndicesCount, particleEmitter->usedIndices);
}

void particle_emitter_render(ParticleEmitter* particleEmitter, const Camera* camera)
{
    particle_renderer_render(&particleEmitter->particleRenderer, camera, particleEmitter->usedIndicesCount);
}

void particle_emitter_reset(ParticleEmitter* particleEmitter)
{
    particleEmitter->freeIndicesCount = particleEmitter->parameters.maxParticleCount;
    particleEmitter->usedIndicesCount = 0;
    for (int i = 0; i < particleEmitter->freeIndicesCount; ++i)
    {
        particleEmitter->freeIndices[particleEmitter->freeIndicesCount - 1 - i] = i;
    }
}

void particle_emitter_emit(ParticleEmitter* particleEmitter)
{
    if (particleEmitter->freeIndicesCount == 0)
    {
        return;
    }

    --particleEmitter->freeIndicesCount;
    int index = particleEmitter->freeIndices[particleEmitter->freeIndicesCount];

    particleEmitter->usedIndices[particleEmitter->usedIndicesCount] = index;
    ++particleEmitter->usedIndicesCount;

    Particle* p = &particleEmitter->particles[index];
    particle_init(p);
    p->direction[0] = randomZeroToOne() - 0.5f;
    p->direction[1] = randomZeroToOne() - 0.5f;
    p->direction[2] = randomZeroToOne() - 0.5f;
    p->lifeTime = randomZeroToOne() * 20.0f;
}