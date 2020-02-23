#ifndef emitter_parameters_h
#define emitter_parameters_h

#include <cglm/cglm.h>

struct EmitterParameters
{
    vec4 startColorMin;
    vec4 startColorMax;
    vec4 endColorMin;
    vec4 endColorMax;
    vec2 particleLifeTime; // Min-max
    vec2 speed; // Min-max
    vec2 acceleration; // Min-max
    vec2 startSize; // Min-max
    vec2 scalingRate; // Min-max
    float emissionRate;
    float gravityModifier;
    vec4 direction;
    vec4 directionVariance;
};
typedef struct EmitterParameters EmitterParameters;

void emitter_parameters_init(EmitterParameters* emitterParameters);

#endif