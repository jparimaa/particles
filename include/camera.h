#ifndef camera_h
#define camera_h

#include "transform.h"

#include <cglm/cglm.h>

struct Camera
{
    float FOV;
    float aspectRatio;
    float nearClipDistance;
    float farClipDistance;
    Transform transform;
};
typedef struct Camera Camera;

void camera_init(Camera* camera);

void camera_getViewMatrix(const Camera* camera, mat4 viewMatrix);
void camera_getProjectionMatrix(const Camera* camera, mat4 projectionMatrix);

#endif