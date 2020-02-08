#include "particle.h"

void particle_init(Particle* particle)
{
    glm_vec3_zero(particle->position);
    particle->scale = 1.0f;
    glm_vec3_one(particle->color);
    particle->rotation = 0.0f;
}

void particle_flow_init(ParticleFlow* particleFlow)
{
    glm_vec3_zero(particleFlow->direction);
    particleFlow->lifeTime = 1.0f;
}
