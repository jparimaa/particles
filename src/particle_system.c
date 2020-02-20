#include "particle_system.h"

void particle_system_init(ParticleSystem* system, int maxEmitterCount)
{
    system->maxEmitterCount = maxEmitterCount;
    system->emitters = malloc(maxEmitterCount * sizeof(ParticleEmitter));
    system->emitterCount = 0;
}

void particle_system_deinit(ParticleSystem* system)
{
    for (int i = 0; i < system->emitterCount; ++i)
    {
        particle_emitter_deinit(&system->emitters[i]);
    }
    free(system->emitters);
    free(system->particles);
    free(system->particleFlows);
    particle_renderer_deinit(&system->particleRenderer);
}

int particle_system_add_emitter(ParticleSystem* system, EmitterParameters* parameters)
{
    int index = system->emitterCount;
    if (index == system->maxEmitterCount)
    {
        return -1;
    }
    particle_emitter_init(&system->emitters[index], parameters);
    ++system->emitterCount;
    return index;
}

void particle_system_finalize(ParticleSystem* system)
{
    int particleCount = 0;
    for (int i = 0; i < system->emitterCount; ++i)
    {
        system->emitters[i].startIndex = particleCount;
        EmitterParameters* ep = &system->emitters[i].parameters;
        float maxCount = ep->particleLifeTime[1] / (1.0f / ep->emissionRate);
        particleCount += (int)(maxCount + 1.0f);
    }
    system->maxParticleCount = particleCount;
    system->particles = malloc(sizeof(Particle) * particleCount);
    system->particleFlows = malloc(sizeof(ParticleFlow) * particleCount);

    for (int i = 0; i < particleCount; ++i)
    {
        system->particles[i].scale = 0.0f;
    }

    particle_renderer_init(&system->particleRenderer, particleCount);
}

void particle_system_update(ParticleSystem* system, float timeDelta)
{
    // Todo: multithread
    for (int i = 0; i < system->emitterCount; ++i)
    {
        particle_emitter_update(&system->emitters[i], timeDelta, system->particles, system->particleFlows);
    }

    particle_renderer_update(&system->particleRenderer, system->particles, system->maxParticleCount);
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
}
