#include "macros.h"
#include "helpers.h"
#include "camera.h"
#include "camera_controller.h"
#include "particle_manager.h"
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

    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

    // Setup utilities
    Camera camera;
    camera_init(&camera);
    camera.transform.position[0] = 0.0f;
    camera.transform.position[2] = 5.0f;

    Input input;
    input_init(&input, window);

    CameraController cameraController;
    cameraController_init(&cameraController, &camera, &input);

    ParticleManager particleManager;
    particle_manager_init(&particleManager, 100);

    float previousTime = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        float timeDelta = currentTime - previousTime;
        previousTime = currentTime;

        input_update(&input);
        cameraController_update(&cameraController, timeDelta);
        particle_manager_update(&particleManager, timeDelta, &camera);

        glClear(GL_COLOR_BUFFER_BIT);

        particle_manager_render(&particleManager);

        glfwSwapBuffers(window);
        glfwPollEvents();

        if (input.keyReleased[GLFW_KEY_ESCAPE])
        {
            glfwSetWindowShouldClose(window, true);
        }
    }

    particle_manager_deinit(&particleManager);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
