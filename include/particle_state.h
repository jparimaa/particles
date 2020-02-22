#ifndef particle_state_h
#define particle_state_h

#include <stdint.h>

enum ParticleStatus
{
    PARTICLE_STATUS_DEAD = 0,
    PARTICLE_STATUS_ALIVE = 1,
    PARTICLE_STATUS_RESET = 2
};
typedef enum ParticleStatus ParticleStatus;

struct ParticleState
{
    int32_t status;
    float lifetime;
};
typedef struct ParticleState ParticleState;

void particle_state_init(ParticleState* state);

#endif
