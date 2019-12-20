#include "shader.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

static bool createShader(const char* filename, GLuint* shaderOut)
{
    GLenum type = 0;
    if (strstr(filename, ".vert") != NULL)
    {
        type = GL_VERTEX_SHADER;
    }
    if (strstr(filename, ".geom") != NULL)
    {
        type = GL_GEOMETRY_SHADER;
    }
    if (strstr(filename, ".frag") != NULL)
    {
        type = GL_FRAGMENT_SHADER;
    }
    if (strstr(filename, ".tes") != NULL)
    {
        type = GL_TESS_EVALUATION_SHADER;
    }
    if (strstr(filename, ".tcs") != NULL)
    {
        type = GL_TESS_CONTROL_SHADER;
    }
    if (strstr(filename, ".comp") != NULL)
    {
        type = GL_COMPUTE_SHADER;
    }

    assert(type != 0);
    *shaderOut = glCreateShader(type);

    if (*shaderOut == 0)
    {
        printf("ERROR: Could not create shader\n");
        return false;
    }

    return true;
};

static bool compileShader(const char* filename, GLuint shader)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        return false;
    }

    fseek(file, 0L, SEEK_END);
    long int numBytes = ftell(file);
    fseek(file, 0L, SEEK_SET);

    GLchar* shaderSource = calloc(numBytes, sizeof(GLchar));
    if (shaderSource == NULL)
    {
        return false;
    }

    fread(shaderSource, sizeof(GLchar), numBytes, file);
    fclose(file);

    glShaderSource(shader, 1, &shaderSource, NULL);
    free(shaderSource);

    glCompileShader(shader);
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE)
    {
        printf("ERROR: Shader compilation failed: %s\n", filename);
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
        char* errorMessage = malloc(maxLength * sizeof(char));
        glGetShaderInfoLog(shader, maxLength, &maxLength, errorMessage);
        printf(errorMessage);
        free(errorMessage);
    }

    return isCompiled == GL_TRUE;
}

static bool linkProgram(GLuint program)
{
    glLinkProgram(program);
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);

    if (isLinked == GL_FALSE)
    {
        printf("ERROR: Shader linking failed\n");
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        char* errorMessage = malloc(maxLength * sizeof(char));
        glGetProgramInfoLog(program, maxLength, &maxLength, errorMessage);
        printf(errorMessage);
        free(errorMessage);
        return false;
    }
    return true;
}

static void deleteShaders(int numShaders, GLuint* shaders, GLuint program)
{
    for (int i = 0; i < numShaders; ++i)
    {
        glDetachShader(program, shaders[i]);
        glDeleteShader(shaders[i]);
    }
}

GLuint createShaderProgram(int numFiles, const char** files)
{
    GLuint program = glCreateProgram();

    if (program == 0)
    {
        return 0;
    }

    GLuint* shaders = calloc(numFiles, sizeof(GLuint));
    bool failed = false;

    for (int i = 0; i < numFiles; ++i)
    {
        GLuint shader;

        if (!createShader(files[i], &shader))
        {
            failed = true;
            break;
        }

        if (!compileShader(files[i], shader))
        {
            failed = true;
            break;
        }

        shaders[i] = shader;
        glAttachShader(program, shader);
    }

    if (failed || !linkProgram(program))
    {
        deleteShaders(numFiles, shaders, program);
        glDeleteProgram(program);
        free(shaders);
        return 0;
    }

    deleteShaders(numFiles, shaders, program);
    free(shaders);

    return program;
}

void deleteShaderProgram(GLuint program)
{
    glDeleteProgram(program);
}
