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
    particleEmitter->particleData = malloc(sizeof(vec4) * maxParticleCount);
    particleEmitter->timeSinceLastEmit = 0.0f;

    particle_emitter_reset(particleEmitter);

    particle_renderer_init(&particleEmitter->particleRenderer, maxParticleCount);
}

void particle_emitter_deinit(ParticleEmitter* particleEmitter)
{
    particle_renderer_deinit(&particleEmitter->particleRenderer);
    free(particleEmitter->particles);
    free(particleEmitter->particleData);
}

void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta)
{
    EmitterParameters* params = &particleEmitter->parameters;

    particleEmitter->timeSinceLastEmit += timeDelta;
    int spawnCount = (int)(particleEmitter->timeSinceLastEmit / (1.0f / params->emissionRate));
    spawnCount = MIN(spawnCount, particleEmitter->parameters.maxParticleCount - particleEmitter->particleCount);
    if (spawnCount > 0)
    {
        particleEmitter->timeSinceLastEmit = 0.0f;
    }

    for (int i = 0; i < spawnCount; ++i)
    {
        particle_emitter_emit(particleEmitter);
    }

    vec3 gravity;
    glm_vec3_scale((float*)UP, -1.0f, gravity);
    glm_vec3_scale(gravity, params->gravityModifier, gravity);
    glm_vec3_scale(gravity, timeDelta, gravity);

    for (int i = 0; i < particleEmitter->particleCount; ++i)
    {
        Particle* p = &particleEmitter->particles[i];
        p->lifeTime -= timeDelta;

        if (p->lifeTime < 0.0f)
        {
            particle_emitter_destroy(particleEmitter, i);
            continue;
        }

        glm_vec3_add(p->direction, gravity, p->direction);

        vec3 movement = GLM_VEC3_ZERO_INIT;
        glm_vec3_scale(p->direction, timeDelta, movement);
        glm_vec3_add(p->transform.position, movement, p->transform.position);

        glm_vec3_copy(p->transform.position, particleEmitter->particleData[i]);
        particleEmitter->particleData[i][3] = 1.0f;
    }

    particle_renderer_update(&particleEmitter->particleRenderer, particleEmitter->particleData, particleEmitter->particleCount);
}

void particle_emitter_render(ParticleEmitter* particleEmitter, const Camera* camera)
{
    particle_renderer_render(&particleEmitter->particleRenderer, camera, particleEmitter->particleCount);
}

void particle_emitter_reset(ParticleEmitter* particleEmitter)
{
    particleEmitter->particleCount = 0;
    particleEmitter->timeSinceLastEmit = 0.0f;
}

void particle_emitter_emit(ParticleEmitter* particleEmitter)
{
    if (particleEmitter->particleCount == particleEmitter->parameters.maxParticleCount)
    {
        return;
    }

    Particle* p = &particleEmitter->particles[particleEmitter->particleCount];
    particle_init(p);
    ++particleEmitter->particleCount;

    EmitterParameters* ep = &particleEmitter->parameters;
    p->direction[0] = ep->direction[0] + randomBetweenFloats(-ep->directionVariance[0], ep->directionVariance[0]);
    p->direction[1] = ep->direction[1] + randomBetweenFloats(-ep->directionVariance[1], ep->directionVariance[1]);
    p->direction[2] = ep->direction[2] + randomBetweenFloats(-ep->directionVariance[2], ep->directionVariance[2]);
    glm_vec3_normalize(p->direction);
    glm_vec3_scale(p->direction, ep->startSpeed, p->direction);
    p->lifeTime = ep->particleLifeTime;
}

void particle_emitter_destroy(ParticleEmitter* particleEmitter, int index)
{
    int lastIndex = particleEmitter->particleCount - 1;
    particleEmitter->particles[index] = particleEmitter->particles[lastIndex];
    --particleEmitter->particleCount;
}