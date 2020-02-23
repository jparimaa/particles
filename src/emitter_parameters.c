#include "emitter_parameters.h"

void emitter_parameters_init(EmitterParameters* emitterParameters)
{
    glm_vec3_zero(emitterParameters->direction);
    emitterParameters->direction[1] = 1.0f;

    emitterParameters->directionVariance[0] = 1.0f;
    emitterParameters->directionVariance[1] = 0.0f;
    emitterParameters->directionVariance[2] = 1.0f;

    emitterParameters->particleLifeTime[0] = 1.0f;
    emitterParameters->particleLifeTime[1] = 4.0f;

    emitterParameters->speed[0] = 0.7f;
    emitterParameters->speed[1] = 1.3f;

    emitterParameters->acceleration[0] = 0.99f;
    emitterParameters->acceleration[1] = 0.999f;

    emitterParameters->startSize[0] = 0.3f;
    emitterParameters->startSize[1] = 0.6f;

    emitterParameters->scalingRate[0] = 1.0f;
    emitterParameters->scalingRate[1] = 1.0f;

    emitterParameters->emissionRate = 1.0f;

    emitterParameters->startColorMin[0] = 0.85f;
    emitterParameters->startColorMin[1] = 0.85f;
    emitterParameters->startColorMin[2] = 0.85f;
    emitterParameters->startColorMin[3] = 1.0f;
    glm_vec4_one(emitterParameters->startColorMax);

    glm_vec4_zero(emitterParameters->endColorMin);
    glm_vec4_zero(emitterParameters->endColorMax);
    emitterParameters->endColorMax[1] = 0.4f;
    emitterParameters->endColorMax[2] = 0.4f;

    emitterParameters->gravityModifier = 0.2f;
}