#version 450 core

layout (binding = 0) uniform sampler2D tex;

out vec4 color;

void main()
{
	color = texture(tex, vec2(0.0f, 0.0f));
} 