#include "particle.h"

void particle_init(Particle* particle)
{
    transform_init(&particle->transform);
    glm_vec3_zero(particle->direction);
    particle->lifeTime = 10.0f;
}
