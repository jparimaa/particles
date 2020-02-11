#include "emitter_parameters.h"

void emitter_parameters_init(EmitterParameters* emitterParameters)
{
    glm_vec3_zero(emitterParameters->direction);
    emitterParameters->direction[1] = 1.0f;
    emitterParameters->directionVariance[0] = 1.0f;
    emitterParameters->directionVariance[1] = 0.0f;
    emitterParameters->directionVariance[2] = 1.0f;
    emitterParameters->particleLifeTime[0] = 5.0f;
    emitterParameters->particleLifeTime[1] = 7.0f;
    emitterParameters->speed[0] = 0.5f;
    emitterParameters->speed[1] = 0.6f;
    emitterParameters->acceleration[0] = 1.0f;
    emitterParameters->acceleration[1] = 1.0f;
    emitterParameters->startSize[0] = 1.0f;
    emitterParameters->startSize[1] = 2.0f;
    emitterParameters->scalingRate[0] = 1.0f;
    emitterParameters->scalingRate[1] = 1.0f;
    emitterParameters->emissionRate = 5.0f;
    glm_vec4_one(emitterParameters->startColor);
    glm_vec4_zero(emitterParameters->endColor);
    emitterParameters->endColor[0] = 0.4f;
    emitterParameters->gravityModifier = 0.2f;
    emitterParameters->maxParticleCount = 8;
}