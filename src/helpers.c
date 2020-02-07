#include "helpers.h"
#include "macros.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char* concat(const char* s1, const char* s2)
{
    char* result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

float randomZeroToOne()
{
    return (float)rand() / (float)RAND_MAX;
}

float randomBetweenFloats(float a, float b)
{
    float randomZeroOne = randomZeroToOne();
    float diff = b - a;
    float r = randomZeroOne * diff;
    return a + r;
}

void APIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    UNUSED(length);
    UNUSED(userParam);

    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        return;
    }

    printf("OpenGL debug callback function\n");
    printf("    Message: %s\n", message);
    printf("    Source: ");

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        printf("API");
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        printf("WINDOW_SYSTEM");
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        printf("SHADER_COMPILER");
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        printf("THIRD_PARTY");
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        printf("APPLICATION");
        break;
    case GL_DEBUG_SOURCE_OTHER:
        printf("OTHER");
        break;
    }
    printf("\n");

    printf("    Type: ");
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        printf("TYPE_ERROR");
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        printf("DEPRECATED_BEHAVIOR");
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        printf("UNDEFINED_BEHAVIOR");
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        printf("PORTABILITY");
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        printf("PERFORMANCE");
        break;
    case GL_DEBUG_TYPE_MARKER:
        printf("MARKER");
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        printf("PUSH_GROUP");
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        printf("POP_GROUP");
        break;
    case GL_DEBUG_TYPE_OTHER:
        printf("OTHER");
        break;
    default:
        printf("UNKNOWN");
        break;
    }
    printf("\n");

    printf("    ID: %d\n", id);
    printf("    Severity: ");
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_LOW:
        printf("LOW");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        printf("MEDIUM");
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        printf("HIGH");
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        printf("NOTIFICATION");
        break;
    default:
        printf("UNKNOWN");
        break;
    }
    printf("\n\n");
}
