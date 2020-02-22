#include "particle_state.h"

void particle_state_init(ParticleState* state)
{
    state->status = PARTICLE_STATUS_DEAD;
    state->lifetime = 0.0f;
}