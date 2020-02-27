#include "macros.h"
#include "helpers.h"
#include "camera.h"
#include "camera_controller.h"
#include "particle_system.h"
#include "emitter_parameters.h"
#include "input.h"

#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>

bool initialize(GLFWwindow** window)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    *window = glfwCreateWindow(1024, 764, "GL", NULL, NULL);
    if (!*window)
    {
        printf("ERROR: Failed to create GLFW window");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window);
    glfwSetWindowPos(*window, 200, 200);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("ERROR: Failed to create GLAD");
        glfwDestroyWindow(*window);
        glfwTerminate();
        return false;
    }

    if (glDebugMessageCallback)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        GLuint unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &unusedIds, GL_TRUE);
        glDebugMessageCallback(glDebugCallback, NULL);
    }

    return true;
}

int main()
{
    GLFWwindow* window = NULL;
    if (!initialize(&window))
    {
        return 1;
    }

    Camera camera;
    camera_init(&camera);
    camera.transform.position[0] = 0.0f;
    camera.transform.position[2] = 5.0f;

    Input input;
    input_init(&input, window);

    CameraController cameraController;
    cameraController_init(&cameraController, &camera, &input);

    EmitterParameters emitterParameters;
    emitter_parameters_init(&emitterParameters);

    ParticleSystem particleSystem;
    particle_system_init(&particleSystem, 2);
    particle_system_add_emitter(&particleSystem, &emitterParameters);
    emitterParameters.direction[1] = -1.0f;
    particle_system_add_emitter(&particleSystem, &emitterParameters);
    particle_system_add_emitter(&particleSystem, &emitterParameters);
    particle_system_add_emitter(&particleSystem, &emitterParameters);
    particle_system_finalize(&particleSystem);

    float previousTime = 0.0f;
    float fpsTimer = 0.0f;
    int frameCounter = 0;

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        float timeDelta = currentTime - previousTime;
        previousTime = currentTime;

        fpsTimer += timeDelta;
        ++frameCounter;
        if (fpsTimer > 1.0f)
        {
            printf("FPS: %f\n", (float)frameCounter / fpsTimer);
            fpsTimer = 0.0f;
            frameCounter = 0;
        }

        input_clear(&input);
        glfwPollEvents();

        input_update(&input);
        cameraController_update(&cameraController, timeDelta);
        particle_system_update(&particleSystem, timeDelta);

        glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        particle_system_render(&particleSystem, &camera);

        glfwSwapBuffers(window);

        if (input.keyReleased[GLFW_KEY_ESCAPE])
        {
            glfwSetWindowShouldClose(window, true);
        }
        if (input.keyReleased[GLFW_KEY_F])
        {
            particle_system_reset(&particleSystem);
        }
    }

    particle_system_deinit(&particleSystem);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
