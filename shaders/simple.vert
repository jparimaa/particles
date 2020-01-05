#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

layout (std430, binding = 0) readonly buffer WVP
{
    mat4 wvpMatrices[];
};

out vec2 texCoord;

void main()
{
	gl_Position = wvpMatrices[gl_InstanceID] * vec4(position, 1.0);
    texCoord = uv;
}