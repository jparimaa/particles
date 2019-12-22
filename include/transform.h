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

void initTransform(Transform* t);

void move(Transform* t, vec3 v);
void rotate(Transform* t, float angle, vec3 axis);

void getModelMatrix(const Transform* t, mat4 modelMatrix);
void getForward(const Transform* t, vec3 forward);
void getUp(const Transform* t, vec3 up);

#endif