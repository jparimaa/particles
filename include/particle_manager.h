#ifndef particle_manager_h
#define particle_manager_h

#include "particle.h"
#include "camera.h"

#include <glad/glad.h>

struct ParticleManager
{
    int particleCount;
    Particle* particles;

    GLuint VAO;
    GLuint VBO;
    GLuint sprite;
    GLuint shader;
    GLuint wvpBuffer;
};
typedef struct ParticleManager ParticleManager;

void particle_manager_init(ParticleManager* particleManager);
void particle_manager_deinit(ParticleManager* particleManager);
void particle_manager_update(ParticleManager* particleManager, float timeDelta, const Camera* camera);
void particle_manager_render(ParticleManager* particleManager);

#endif