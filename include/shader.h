#ifndef shader_h
#define shader_h

#include <glad/glad.h>

GLuint shader_createProgram(int numFiles, const char** files);
void shader_deleteProgram(GLuint program);

#endif