#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

layout (location = 0) uniform mat4 viewProjectionMatrix;
layout (location = 1) uniform vec3 cameraUp;
layout (location = 2) uniform vec3 cameraRight;

struct Particle
{
    vec3 position;
    float scale;
    vec4 color;
    float rotation;
};

layout (std430, binding = 0) readonly buffer PositionSizeBuffer
{
    Particle particles[];
};

out vec2 texCoord;
out vec4 particleColor;

void main()
{
    vec3 vertexPosition = 
        particles[gl_InstanceID].position +
		cameraRight * position.x * particles[gl_InstanceID].scale +
		cameraUp * position.y * particles[gl_InstanceID].scale;

	gl_Position = viewProjectionMatrix * vec4(vertexPosition, 1.0);
    texCoord = uv;
    particleColor = particles[gl_InstanceID].color;
}