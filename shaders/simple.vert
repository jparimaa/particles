#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

layout (location = 0) uniform mat4 viewProjectionMatrix;
layout (location = 1) uniform vec3 cameraUp;
layout (location = 2) uniform vec3 cameraRight;

layout (std430, binding = 0) readonly buffer PositionSizeBuffer
{
    vec4 positionAndSize[];
};

out vec2 texCoord;

void main()
{
    vec3 vertexPosition = 
        positionAndSize[gl_InstanceID].xyz +
		cameraRight * position.x * positionAndSize[gl_InstanceID].w +
		cameraUp * position.y * positionAndSize[gl_InstanceID].w;

	gl_Position = viewProjectionMatrix * vec4(vertexPosition, 1.0);
    texCoord = uv;
}