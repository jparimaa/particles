#include "shader.h"
#include "macros.h"
#include "helpers.h"
#include "camera.h"
#include "camera_controller.h"
#include "input.h"

#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wtype-limits"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma GCC diagnostic pop

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

    // clang-format off
    float vertices[] = {
        0.5f, -0.5f, 0.0f, // right
        -0.5f, -0.5f, 0.0f, // left
        0.0f, 0.5f, 0.0f // top
    };
    // clang-format on

    // Create buffer
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);

    // Create shader
    const char* files[] = {CREATE_PATH(SHADER_PATH, "simple.vert"), CREATE_PATH(SHADER_PATH, "simple.frag")};
    GLuint shaderProgram = shader_createProgram(2, files);

    glUseProgram(shaderProgram);
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

    // Load image
    const char* imagePath = CREATE_PATH(ASSET_PATH, "green_square.png");
    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc* imageData = stbi_load(imagePath, &width, &height, &channels, 4);
    GLuint texture;
    if (!imageData)
    {
        printf("ERROR: Failed to load image %s\n", imagePath);
    }
    else
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    stbi_image_free(imageData);

    // Setup utilities
    Camera camera;
    camera_init(&camera);
    camera.transform.position[0] = 0.0f;
    camera.transform.position[2] = 5.0f;
    mat4 projectionMatrix;
    camera_getProjectionMatrix(&camera, projectionMatrix);

    Input input;
    input_init(&input, window);

    CameraController cameraController;
    cameraController_init(&cameraController, &camera, &input);

    float previousTime = 0.0;

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = (float)glfwGetTime();
        float timeDelta = currentTime - previousTime;
        previousTime = currentTime;

        input_update(&input);
        cameraController_update(&cameraController, timeDelta);

        if (input.keyReleased[GLFW_KEY_ESCAPE])
        {
            glfwSetWindowShouldClose(window, true);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        mat4 wvpMatrix;
        mat4 viewMatrix;
        camera_getViewMatrix(&camera, viewMatrix);
        mat4 worldMatrix = GLM_MAT4_IDENTITY_INIT;
        glm_mat4_mulN((mat4* []){&projectionMatrix, &viewMatrix, &worldMatrix}, 3, wvpMatrix);

        glUniformMatrix4fv(0, 1, GL_FALSE, wvpMatrix[0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader_deleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
