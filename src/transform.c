#include "transform.h"

static const vec3 FORWARD = {0.0f, 0.0f, -1.0f};
static const vec3 UP = {0.0f, 1.0f, 0.0f};
static const vec3 RIGHT = {1.0f, 0.0f, 0.0f};

void initTransform(Transform* t)
{
    glm_vec3_zero(t->position);
    glm_vec3_zero(t->rotation);
    glm_vec3_one(t->scale);
}

void move(Transform* t, vec3 v)
{
    glm_vec3_add(t->position, v, t->position);
}

void rotate(Transform* t, float angle, vec3 axis)
{
    glm_vec3_rotate(t->rotation, angle, axis);
}

void getModelMatrix(const Transform* t, mat4 modelMatrix)
{
    mat4 scaleMatrix = GLM_MAT4_IDENTITY_INIT;
    glm_scale(modelMatrix, (float*)t->scale);

    mat4 rotationMatrix;
    glm_euler((float*)t->rotation, rotationMatrix);

    mat4 translationMatrix;
    glm_translate(translationMatrix, (float*)t->position);

    glm_mat4_identity(modelMatrix);
    glm_mat4_mul(modelMatrix, scaleMatrix, modelMatrix);
    glm_mat4_mul(modelMatrix, rotationMatrix, modelMatrix);
    glm_mat4_mul(modelMatrix, translationMatrix, modelMatrix);
}

void getForward(const Transform* t, vec3 forward)
{
    mat4 rotationMatrix;
    glm_euler((float*)t->rotation, rotationMatrix);

    vec4 f = GLM_VEC4_ZERO_INIT;
    f[0] = FORWARD[0];
    f[1] = FORWARD[1];
    f[2] = FORWARD[2];

    glm_mat4_mulv(rotationMatrix, f, f);
    glm_vec4_copy3(f, forward);
}

void getUp(const Transform* t, vec3 up)
{
    mat4 rotationMatrix;
    glm_euler((float*)t->rotation, rotationMatrix);

    vec4 u = GLM_VEC4_ZERO_INIT;
    u[0] = UP[0];
    u[1] = UP[1];
    u[2] = UP[2];

    glm_mat4_mulv(rotationMatrix, u, u);
    glm_vec4_copy3(u, up);
}