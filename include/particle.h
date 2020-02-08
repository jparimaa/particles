#ifndef particle_h
#define particle_h

#include <cglm/cglm.h>

struct Particle
{
    vec3 position;
    float scale;
    vec3 color;
    float rotation;
};
typedef struct Particle Particle;

void particle_init(Particle* particle);

struct ParticleFlow
{
    vec3 direction;
    float lifeTime;
};
typedef struct ParticleFlow ParticleFlow;

void particle_flow_init(ParticleFlow* particleFlow);

#endif