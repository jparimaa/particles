#ifndef shader_h
#define shader_h

#include <glad/glad.h>

GLuint createShaderProgram(int numFiles, const char** files);
void deleteShaderProgram(GLuint program);

#endif