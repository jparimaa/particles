#ifndef transform_h
#define transform_h

#include <cglm/cglm.h>

struct Transform
{
    vec3 position;
    vec3 rotation;
    vec3 scale;
};
typedef struct Transform Transform;

extern const vec3 FORWARD;
extern const vec3 UP;
extern const vec3 RIGHT;

void transform_init(Transform* t);

void transform_move(Transform* t, vec3 v);
void transform_rotate(Transform* t, float angle, vec3 axis);

void transform_getModelMatrix(const Transform* t, mat4 modelMatrix);
void transform_getForward(const Transform* t, vec3 forward);
void transform_getUp(const Transform* t, vec3 up);
void transform_getRight(const Transform* t, vec3 right);

#endif