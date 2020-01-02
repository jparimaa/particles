#include "transform.h"

const vec3 FORWARD = {0.0f, 0.0f, -1.0f};
const vec3 UP = {0.0f, 1.0f, 0.0f};
const vec3 RIGHT = {1.0f, 0.0f, 0.0f};

static void transform_getDirectionVector(const Transform* t, const vec3 dir, vec3 out)
{
    mat4 rotationMatrix;
    glm_euler_yxz((float*)t->rotation, rotationMatrix);

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
    vec3 v;
    glm_vec3_scale(axis, angle, v);
    glm_vec3_add(t->rotation, v, t->rotation);
}

void transform_getWorldMatrix(const Transform* t, mat4 worldMatrix)
{
    mat4 scaleMatrix = GLM_MAT4_IDENTITY_INIT;
    glm_scale(scaleMatrix, (float*)t->scale);

    mat4 rotationMatrix;
    glm_euler((float*)t->rotation, rotationMatrix);

    mat4 translationMatrix = GLM_MAT4_IDENTITY_INIT;
    glm_translate(translationMatrix, (float*)t->position);

    glm_mat4_mulN((mat4* []){&translationMatrix, &rotationMatrix, &scaleMatrix}, 3, worldMatrix);
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