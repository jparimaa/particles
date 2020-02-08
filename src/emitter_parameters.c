#include "emitter_parameters.h"

void emitter_parameters_init(EmitterParameters* emitterParameters)
{
    glm_vec3_zero(emitterParameters->direction);
    emitterParameters->direction[1] = 1.0f;
    emitterParameters->directionVariance[0] = 1.0f;
    emitterParameters->directionVariance[1] = 0.0f;
    emitterParameters->directionVariance[2] = 1.0f;
    emitterParameters->particleLifeTime = 5.0f;
    emitterParameters->startSpeed = 0.5f;
    emitterParameters->startSize = 1.0f;
    emitterParameters->emissionRate = 5.0f;
    glm_vec3_one(emitterParameters->startColor);
    emitterParameters->gravityModifier = 0.1f;
    emitterParameters->maxParticleCount = 3;
}