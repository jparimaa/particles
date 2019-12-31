#include "particle_manager.h"
#include "helpers.h"

#include <stdlib.h>

static const int MAX_PARTICLE_COUNT = 100;

void particle_manager_init(ParticleManager* particleManager)
{
    particleManager->particleCount = 1;
    particleManager->particles = malloc(sizeof(Particle) * MAX_PARTICLE_COUNT);
    for (int i = 0; i < particleManager->particleCount; ++i)
    {
        particle_init(&particleManager->particles[i]);
        particleManager->particles[i].direction[0] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].direction[1] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].direction[2] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].lifeTime = randomZeroToOne() * 20.0f;
    }
}

void particle_manager_update(ParticleManager* particleManager, float timeDelta)
{
    for (int i = 0; i < particleManager->particleCount; ++i)
    {
        Particle* p = &particleManager->particles[i];
        vec3 movement = GLM_VEC3_ZERO_INIT;
        glm_vec3_scale(p->direction, timeDelta, movement);
        glm_vec3_add(p->transform.position, movement, p->transform.position);
        printf("#%i : %f %f %f\n", i, p->transform.position[0], p->transform.position[1], p->transform.position[2]);
    }
}

void particle_manager_render(ParticleManager* particleManager)
{
}
