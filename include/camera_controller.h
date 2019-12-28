#ifndef camera_controller_h
#define camera_controller_h

#include "camera.h"
#include "input.h"

struct CameraController
{
    Camera* camera;
    Input* input;
    float movementSpeed;
    float mouseSensitivity;
};
typedef struct CameraController CameraController;

void cameraController_init(CameraController* cameraController, Camera* camera, Input* input);
void cameraController_update(CameraController* cameraController, float timeDelta);

#endif