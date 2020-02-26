#include "particle_system.h"

void particle_system_init(ParticleSystem* system, int maxEmitterCount)
{
    system->maxEmitterCount = maxEmitterCount;
    system->emitterParameters = malloc(maxEmitterCount * sizeof(EmitterParameters));
    system->emitters = malloc(maxEmitterCount * sizeof(ParticleEmitter));
    system->emitterCount = 0;
}

void particle_system_deinit(ParticleSystem* system)
{
    for (int i = 0; i < system->emitterCount; ++i)
    {
        particle_emitter_deinit(&system->emitters[i]);
    }
    free(system->emitterParameters);
    free(system->emitters);
    free(system->particleStates);
    particle_renderer_deinit(&system->particleRenderer);
}

int particle_system_add_emitter(ParticleSystem* system, EmitterParameters* parameters)
{
    int index = system->emitterCount;
    if (index == system->maxEmitterCount)
    {
        return -1;
    }
    system->emitterParameters[index] = *parameters;
    ++system->emitterCount;
    return index;
}

void particle_system_finalize(ParticleSystem* system)
{
    int particleCount = 0;
    for (int i = 0; i < system->emitterCount; ++i)
    {
        EmitterParameters* ep = &system->emitterParameters[i];
        int maxCount = (int)(ep->particleLifeTime[1] / (1.0f / ep->emissionRate) + 1.0f);
        particle_emitter_init(&system->emitters[i], &system->emitterParameters[i], &system->particleStates, particleCount, maxCount);
        particleCount += maxCount;
    }

    system->maxParticleCount = particleCount;
    system->particleStates = malloc(particleCount * sizeof(ParticleState));

    for (int i = 0; i < particleCount; ++i)
    {
        particle_state_init(&system->particleStates[i]);
    }

    particle_renderer_init(&system->particleRenderer, particleCount, system->emitterParameters, system->emitterCount);
}

void particle_system_update(ParticleSystem* system, float timeDelta)
{
    // Todo: multithread
    for (int i = 0; i < system->emitterCount; ++i)
    {
        particle_emitter_update(&system->emitters[i], timeDelta);
    }

    particle_renderer_update(&system->particleRenderer, system->particleStates, system->maxParticleCount, timeDelta);
}

void particle_system_render(ParticleSystem* system, const Camera* camera)
{
    particle_renderer_render(&system->particleRenderer, camera, system->maxParticleCount);
}

void particle_system_reset(ParticleSystem* system)
{
    for (int i = 0; i < system->emitterCount; ++i)
    {
        particle_emitter_reset(&system->emitters[i]);
    }

    for (int i = 0; i < system->maxParticleCount; ++i)
    {
        particle_state_init(&system->particleStates[i]);
    }
}
