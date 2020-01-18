#include "input.h"
#include "macros.h"

#include <assert.h>

static void input_keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    UNUSED(scancode);
    UNUSED(mods);

    Input* input = glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS)
    {
        input->keyPressed[key] = true;
        input->keyDown[key] = true;
    }
    if (action == GLFW_RELEASE)
    {
        input->keyReleased[key] = true;
        input->keyDown[key] = false;
    }
    if (action == GLFW_REPEAT)
    {
        input->keyDown[key] = true;
    }
}

void input_init(Input* input, GLFWwindow* window)
{
    assert(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, input);
    glfwSetKeyCallback(window, input_keyCallback);

    double x;
    double y;
    glfwGetCursorPos(window, &x, &y);

    input->window = window;
    input->mouseX = (float)x;
    input->mouseY = (float)y;
    input->mouseDeltaX = 0.0f;
    input->mouseDeltaY = 0.0f;

    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        input->keyPressed[i] = false;
        input->keyDown[i] = false;
        input->keyReleased[i] = false;
    }
}

void input_clear(Input* input)
{
    for (int i = 0; i < GLFW_KEY_LAST; ++i)
    {
        input->keyPressed[i] = false;
        input->keyReleased[i] = false;
    }
}

void input_update(Input* input)
{
    assert(input->window);

    double xd;
    double yd;
    glfwGetCursorPos(input->window, &xd, &yd);
    float xf = (float)xd;
    float yf = (float)yd;
    input->mouseDeltaX = input->mouseX - xf;
    input->mouseDeltaY = input->mouseY - yf;
    input->mouseX = xf;
    input->mouseY = yf;
}