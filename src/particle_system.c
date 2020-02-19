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

void particle_system_update(ParticleSystem* system, float timeDelta)
{
    // Todo: multithread
    for (int i = 0; i < system->emitterCount; ++i)
    {
        particle_emitter_update(&system->emitters[i], timeDelta);
    }
}

void particle_system_render(ParticleSystem* system, const Camera* camera)
{
    for (int i = 0; i < system->emitterCount; ++i)
    {
        particle_emitter_render(&system->emitters[i], camera);
    }
}

void particle_system_reset(ParticleSystem* system)
{
    for (int i = 0; i < system->emitterCount; ++i)
    {
        particle_emitter_reset(&system->emitters[i]);
    }
}
