#ifndef emitter_parameters_h
#define emitter_parameters_h

#include <cglm/cglm.h>

struct EmitterParameters
{
    vec3 direction;
    float directionVariationAngle;
    float particleLifeTime;
    float startSpeed;
    float startSize;
    float startColor;
    float emissionRate;
    float gravityModifier;
    int maxParticleCount;
};
typedef struct EmitterParameters EmitterParameters;

#endif