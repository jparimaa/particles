#ifndef emitter_parameters_h
#define emitter_parameters_h

#include <cglm/cglm.h>

struct EmitterParameters
{
    vec3 direction;
    vec3 directionVariance;
    vec2 particleLifeTime; // Min-max
    vec2 speed; // Min-max
    vec2 acceleration; // Min-max
    vec2 startSize; // Min-max
    vec2 scalingRate; // Min-max
    float emissionRate;
    vec4 startColor;
    vec4 endColor;
    float gravityModifier;
    int maxParticleCount;
};
typedef struct EmitterParameters EmitterParameters;

void emitter_parameters_init(EmitterParameters* emitterParameters);

#endif