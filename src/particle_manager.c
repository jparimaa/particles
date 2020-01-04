#include "particle_manager.h"
#include "helpers.h"
#include "macros.h"
#include "shader.h"

#include <stdlib.h>

void particle_manager_init(ParticleManager* particleManager, int maxParticleCount)
{
    particleManager->maxParticleCount = maxParticleCount;
    particleManager->particleCount = 10;
    particleManager->particles = malloc(sizeof(Particle) * maxParticleCount);
    for (int i = 0; i < particleManager->particleCount; ++i)
    {
        particle_init(&particleManager->particles[i]);
        particleManager->particles[i].direction[0] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].direction[1] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].direction[2] = randomZeroToOne() - 0.5f;
        particleManager->particles[i].lifeTime = randomZeroToOne() * 20.0f;
    }

    particle_renderer_init(&particleManager->particleRenderer, maxParticleCount);
}

void particle_manager_deinit(ParticleManager* particleManager)
{
    particle_renderer_deinit(&particleManager->particleRenderer);
    free(particleManager->particles);
}

void particle_manager_update(ParticleManager* particleManager, float timeDelta, const Camera* camera)
{
    for (int i = 0; i < particleManager->particleCount; ++i)
    {
        Particle* p = &particleManager->particles[i];
        vec3 movement = GLM_VEC3_ZERO_INIT;
        glm_vec3_scale(p->direction, timeDelta, movement);
        glm_vec3_add(p->transform.position, movement, p->transform.position);
    }

    particle_renderer_update(&particleManager->particleRenderer, camera, particleManager->particles, particleManager->particleCount);
}

void particle_manager_render(ParticleManager* particleManager)
{
    particle_renderer_render(&particleManager->particleRenderer, particleManager->particleCount);
}
