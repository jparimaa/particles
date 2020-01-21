#ifndef emitter_parameters_h
#define emitter_parameters_h

#include <cglm/cglm.h>

struct EmitterParameters
{
    vec3 direction;
    float directionVariationAngleInRadins;
    float particleLifeTime;
    float startSpeed;
    float startSize;
    float emissionRate;
    vec3 startColor;
    float gravityModifier;
    int maxParticleCount;
};
typedef struct EmitterParameters EmitterParameters;

void emitter_parameters_init(EmitterParameters* emitterParameters);

#endif