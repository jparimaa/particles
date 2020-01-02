#version 450 core
layout (location = 0) in vec3 position;

layout (std430, binding = 0) readonly buffer WVP
{
    mat4 wvpMatrices[];
};

void main()
{
	gl_Position = wvpMatrices[gl_InstanceID] * vec4(position, 1.0);
}