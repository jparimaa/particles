#include "transform.h"

static const vec3 FORWARD = {0.0f, 0.0f, -1.0f};
static const vec3 UP = {0.0f, 1.0f, 0.0f};
static const vec3 RIGHT = {1.0f, 0.0f, 0.0f};

static void transform_getDirectionVector(const Transform* t, const vec3 dir, vec3 out)
{
    mat4 rotationMatrix;
    glm_euler((float*)t->rotation, rotationMatrix);

    vec4 v = GLM_VEC4_ZERO_INIT;
    v[0] = dir[0];
    v[1] = dir[1];
    v[2] = dir[2];

    glm_mat4_mulv(rotationMatrix, v, v);
    glm_vec4_copy3(v, out);
}

void transform_init(Transform* t)
{
    glm_vec3_zero(t->position);
    glm_vec3_zero(t->rotation);
    glm_vec3_one(t->scale);
}

void transform_move(Transform* t, vec3 v)
{
    glm_vec3_add(t->position, v, t->position);
}

void transform_rotate(Transform* t, float angle, vec3 axis)
{
    glm_vec3_rotate(t->rotation, angle, axis);
}

void transform_getModelMatrix(const Transform* t, mat4 modelMatrix)
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

void transform_getForward(const Transform* t, vec3 forward)
{
    transform_getDirectionVector(t, FORWARD, forward);
}

void transform_getUp(const Transform* t, vec3 up)
{
    transform_getDirectionVector(t, UP, up);
}

void transform_getRight(const Transform* t, vec3 right)
{
    transform_getDirectionVector(t, RIGHT, right);
}