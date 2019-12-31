#ifndef particle_manager_h
#define particle_manager_h

#include "particle.h"

struct ParticleManager
{
    int particleCount;
    Particle* particles;
};
typedef struct ParticleManager ParticleManager;

void particle_manager_init(ParticleManager* particleManager);
void particle_manager_update(ParticleManager* particleManager, float timeDelta);
void particle_manager_render(ParticleManager* particleManager);

#endif