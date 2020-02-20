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

void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta, Particle* particles, ParticleFlow* flows)
{
    EmitterParameters* params = &particleEmitter->parameters;

    particleEmitter->timeSinceLastEmit += timeDelta;
    int spawnCount = (int)(particleEmitter->timeSinceLastEmit / (1.0f / params->emissionRate));
    if (spawnCount > 0)
    {
        particleEmitter->timeSinceLastEmit = 0.0f;
    }

    for (int i = 0; i < spawnCount; ++i)
    {
        if (!particle_emitter_emit_particle(particleEmitter, particles, flows))
        {
            break;
        }
    }

    vec3 gravity;
    glm_vec3_scale((float*)UP, -1.0f, gravity);
    glm_vec3_scale(gravity, params->gravityModifier, gravity);
    glm_vec3_scale(gravity, timeDelta, gravity);

    int lastIndex = particleEmitter->startIndex + particleEmitter->particleCount;
    for (int i = particleEmitter->startIndex; i < lastIndex; ++i)
    {
        ParticleFlow* pf = &flows[i];
        pf->lifeTime -= timeDelta;

        if (pf->lifeTime < 0.0f)
        {
            particle_emitter_destroy_particle(particleEmitter, i, particles, flows);
        }

        Particle* p = &particles[i];

        // Gravity
        glm_vec3_add(pf->direction, gravity, pf->direction);

        // Acceleration
        glm_vec3_scale(pf->direction, pf->acceleration, pf->direction);

        // Position
        vec3 movement = GLM_VEC3_ZERO_INIT;
        glm_vec3_scale(pf->direction, timeDelta, movement);
        glm_vec3_add(p->position, movement, p->position);

        // Color
        vec4 colorChange;
        glm_vec4_scale(pf->colorChangeRate, timeDelta, colorChange);
        glm_vec4_add(p->color, colorChange, p->color);

        // Size
        float scaleChange = ((pf->scalingRate * p->scale) - p->scale) * timeDelta;
        p->scale += scaleChange;
    }
}

void particle_emitter_reset(ParticleEmitter* particleEmitter)
{
    particleEmitter->particleCount = 0;
    particleEmitter->timeSinceLastEmit = 0.0f;
}

bool particle_emitter_emit_particle(ParticleEmitter* particleEmitter, Particle* particles, ParticleFlow* flows)
{
    int index = particleEmitter->startIndex + particleEmitter->particleCount;
    Particle* p = &particles[index];
    ParticleFlow* pf = &flows[index];
    particle_init(p);
    ++particleEmitter->particleCount;

    EmitterParameters* ep = &particleEmitter->parameters;

    // Particle
    p->scale = randomBetweenFloats(ep->startSize[0], ep->startSize[1]);
    p->color[0] = randomBetweenFloats(ep->startColorMin[0], ep->startColorMax[0]);
    p->color[1] = randomBetweenFloats(ep->startColorMin[1], ep->startColorMax[1]);
    p->color[2] = randomBetweenFloats(ep->startColorMin[2], ep->startColorMax[2]);
    p->color[3] = randomBetweenFloats(ep->startColorMin[3], ep->startColorMax[3]);

    // Particle flow
    pf->direction[0] = ep->direction[0] + randomBetweenFloats(-ep->directionVariance[0], ep->directionVariance[0]);
    pf->direction[1] = ep->direction[1] + randomBetweenFloats(-ep->directionVariance[1], ep->directionVariance[1]);
    pf->direction[2] = ep->direction[2] + randomBetweenFloats(-ep->directionVariance[2], ep->directionVariance[2]);
    glm_vec3_normalize(pf->direction);
    float speed = randomBetweenFloats(ep->speed[0], ep->speed[1]);
    glm_vec3_scale(pf->direction, speed, pf->direction);

    pf->acceleration = randomBetweenFloats(ep->acceleration[0], ep->acceleration[1]);
    pf->lifeTime = randomBetweenFloats(ep->particleLifeTime[0], ep->particleLifeTime[1]);
    pf->scalingRate = randomBetweenFloats(ep->scalingRate[0], ep->scalingRate[1]);

    vec4 endColor;
    endColor[0] = randomBetweenFloats(ep->endColorMin[0], ep->endColorMax[0]);
    endColor[1] = randomBetweenFloats(ep->endColorMin[1], ep->endColorMax[1]);
    endColor[2] = randomBetweenFloats(ep->endColorMin[2], ep->endColorMax[2]);
    endColor[3] = randomBetweenFloats(ep->endColorMin[3], ep->endColorMax[3]);

    glm_vec4_sub(endColor, p->color, pf->colorChangeRate);
    glm_vec4_divs(pf->colorChangeRate, pf->lifeTime, pf->colorChangeRate);

    return true;
}

void particle_emitter_destroy_particle(ParticleEmitter* particleEmitter, int index, Particle* particles, ParticleFlow* flows)
{
    int lastIndex = particleEmitter->startIndex + particleEmitter->particleCount - 1;
    particles[index] = particles[lastIndex];
    flows[index] = flows[lastIndex];
    particles[lastIndex].scale = 0.0f;
    --particleEmitter->particleCount;
}