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

    glm_vec4_zero(forward);
    glm_vec4_copy3((float*)FORWARD, forward);

    glm_mat4_mulv(rotationMatrix, forward, forward);
}

void getUp(const Transform* t, vec3 up)
{
    mat4 rotationMatrix;
    glm_euler((float*)t->rotation, rotationMatrix);

    glm_vec4_zero(up);
    glm_vec4_copy3((float*)UP, up);

    glm_mat4_mulv(rotationMatrix, up, up);
}