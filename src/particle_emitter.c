#include "particle_emitter.h"
#include "helpers.h"
#include "macros.h"
#include "shader.h"

#include <stdlib.h>

void particle_emitter_init(ParticleEmitter* particleEmitter, int maxParticleCount)
{
    particleEmitter->maxParticleCount = maxParticleCount;
    particleEmitter->particleCount = 10;
    particleEmitter->particles = malloc(sizeof(Particle) * maxParticleCount);

    particle_emitter_reset(particleEmitter);

    particle_renderer_init(&particleEmitter->particleRenderer, maxParticleCount);
}

void particle_emitter_deinit(ParticleEmitter* particleEmitter)
{
    particle_renderer_deinit(&particleEmitter->particleRenderer);
    free(particleEmitter->particles);
}

void particle_emitter_update(ParticleEmitter* particleEmitter, float timeDelta)
{
    for (int i = 0; i < particleEmitter->particleCount; ++i)
    {
        Particle* p = &particleEmitter->particles[i];
        vec3 movement = GLM_VEC3_ZERO_INIT;
        glm_vec3_scale(p->direction, timeDelta, movement);
        glm_vec3_add(p->transform.position, movement, p->transform.position);
    }

    particle_renderer_update(&particleEmitter->particleRenderer, particleEmitter->particles, particleEmitter->particleCount);
}

void particle_emitter_render(ParticleEmitter* particleEmitter, const Camera* camera)
{
    particle_renderer_render(&particleEmitter->particleRenderer, camera, particleEmitter->particleCount);
}

void particle_emitter_reset(ParticleEmitter* particleEmitter)
{
    for (int i = 0; i < particleEmitter->particleCount; ++i)
    {
        particle_init(&particleEmitter->particles[i]);
        particleEmitter->particles[i].direction[0] = randomZeroToOne() - 0.5f;
        particleEmitter->particles[i].direction[1] = randomZeroToOne() - 0.5f;
        particleEmitter->particles[i].direction[2] = randomZeroToOne() - 0.5f;
        particleEmitter->particles[i].lifeTime = randomZeroToOne() * 20.0f;
    }
}
