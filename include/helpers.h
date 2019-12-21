#ifndef helpers_h
#define helpers_h

#include <glad/glad.h>

char* concat(const char* s1, const char* s2);

void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

#endif