#include "camera_controller.h"

#include <assert.h>

static void cameraController_move(Transform* t, const vec3 dir, float amount, bool negate)
{
    vec3 v;
    glm_vec3_copy((float*)dir, v);
    if (negate)
    {
        glm_vec3_negate(v);
    }
    glm_vec3_scale(v, amount, v);
    transform_move(t, v);
}

void cameraController_init(CameraController* cameraController, Camera* camera, Input* input)
{
    assert(camera);
    assert(input);
    cameraController->camera = camera;
    cameraController->input = input;
    cameraController->movementSpeed = 1.0f;
}

void cameraController_update(CameraController* cameraController, float timeDelta)
{
    assert(cameraController->camera);

    Input* input = cameraController->input;
    Transform* t = &cameraController->camera->transform;
    float moveAmount = cameraController->movementSpeed * timeDelta;

    if (input->keyDown[GLFW_KEY_W])
    {
        vec3 v;
        transform_getForward(t, v);
        cameraController_move(t, v, moveAmount, false);
    }
    if (input->keyDown[GLFW_KEY_S])
    {
        vec3 v;
        transform_getForward(t, v);
        cameraController_move(t, v, moveAmount, true);
    }
    if (input->keyDown[GLFW_KEY_A])
    {
        vec3 v;
        transform_getRight(t, v);
        cameraController_move(t, v, moveAmount, true);
    }
    if (input->keyDown[GLFW_KEY_D])
    {
        vec3 v;
        transform_getRight(t, v);
        cameraController_move(t, v, moveAmount, false);
    }
}