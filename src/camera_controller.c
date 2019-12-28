#include "camera_controller.h"
#include "transform.h"

#include <assert.h>

static const float ROTATION_LIMIT_IN_RADIANS = 1.51844f;

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
    cameraController->mouseSensitivity = 0.003f;
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
    if (input->keyDown[GLFW_KEY_R])
    {
        t->position[0] = 0.0f;
        t->position[1] = 0.0f;
        t->position[2] = 5.0f;
        t->rotation[0] = 0.0f;
        t->rotation[1] = 0.0f;
        t->rotation[2] = 0.0f;
    }

    float dx = input->mouseDeltaX * cameraController->mouseSensitivity;
    float dy = input->mouseDeltaY * cameraController->mouseSensitivity;

    transform_rotate(t, dx, (float*)UP);
    transform_rotate(t, dy, (float*)RIGHT);

    if (t->rotation[0] > ROTATION_LIMIT_IN_RADIANS)
    {
        t->rotation[0] = ROTATION_LIMIT_IN_RADIANS;
    }
    if (t->rotation[0] < -ROTATION_LIMIT_IN_RADIANS)
    {
        t->rotation[0] = -ROTATION_LIMIT_IN_RADIANS;
    }
}