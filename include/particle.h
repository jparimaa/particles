#ifndef particle_h
#define particle_h

#include "transform.h"

#include <cglm/cglm.h>

struct Particle
{
    Transform transform;
    vec3 direction;
    float lifeTime;
};
typedef struct Particle Particle;

void particle_init(Particle* particle);

#endif