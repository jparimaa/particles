#include "shader.h"
#include "macros.h"
#include "helpers.h"

#include <cglm/cglm.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdbool.h>

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

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

    mat4 perspectiveMatrix;
    glm_perspective(0.785398f, 4.0f / 3.0f, 0.5f, 50.0f, perspectiveMatrix);

    // clang-format off
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
        0.5f, -0.5f, 0.0f, // right
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
    GLuint shaderProgram = createShaderProgram(2, files);

    glUseProgram(shaderProgram);
    glClearColor(0.0f, 0.0f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
