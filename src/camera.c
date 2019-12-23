#include "camera.h"

void camera_init(Camera* camera)
{
    camera->FOV = 0.785398f;
    camera->aspectRatio = 4.0f / 3.0f;
    camera->nearClipDistance = 0.5f;
    camera->farClipDistance = 100.0f;
    transform_init(&camera->transform);
}

void camera_getViewMatrix(const Camera* camera, mat4 viewMatrix)
{
    vec3 forward;
    transform_getForward(&camera->transform, forward);

    vec3 lookAt;
    glm_vec3_copy((float*)camera->transform.position, lookAt);
    glm_vec3_add(forward, lookAt, lookAt);

    vec3 up;
    transform_getUp(&camera->transform, up);

    glm_lookat((float*)camera->transform.position, lookAt, up, viewMatrix);
}

void camera_getProjectionMatrix(const Camera* camera, mat4 projectionMatrix)
{
    glm_perspective(camera->FOV, camera->aspectRatio, camera->nearClipDistance, camera->farClipDistance, projectionMatrix);
}