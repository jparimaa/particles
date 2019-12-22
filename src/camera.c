#include "camera.h"

void initCamera(struct Camera* camera)
{
    camera->FOV = 0.785398f;
    camera->aspectRatio = 4.0f / 3.0f;
    camera->nearClipDistance = 0.5f;
    camera->farClipDistance = 100.0f;
    initTransform(&camera->transform);
}

void getViewMatrix(const Camera* camera, mat4 viewMatrix)
{    
    vec3 forward;
    getForward(&camera->transform, forward);
    
    vec3 lookAt;
    glm_vec3_copy((float*)camera->transform.position, lookAt);
    glm_vec3_add(forward, lookAt, lookAt);

    vec3 up;
    getUp(&camera->transform, up);

    glm_lookat((float*)camera->transform.position, lookAt, up, viewMatrix);
}

void getProjectionMatrix(const Camera* camera, mat4 projectionMatrix)
{
    glm_perspective(camera->FOV, camera->aspectRatio, camera->nearClipDistance, camera->farClipDistance, projectionMatrix);
}