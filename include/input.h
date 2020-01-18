#ifndef input_h
#define input_h

#include <GLFW/glfw3.h>

#include <stdbool.h>

struct Input
{
    GLFWwindow* window;
    float mouseX;
    float mouseY;
    float mouseDeltaX;
    float mouseDeltaY;
    bool keyPressed[GLFW_KEY_LAST];
    bool keyDown[GLFW_KEY_LAST];
    bool keyReleased[GLFW_KEY_LAST];
};
typedef struct Input Input;

void input_init(Input* input, GLFWwindow* window);
void input_clear(Input* input);
void input_update(Input* input);

#endif