#version 450 core

layout (binding = 0) uniform sampler2D tex;

in vec2 texCoord;
in vec4 particleColor;

out vec4 color;

void main()
{
	color = texture(tex, texCoord);
	color *= particleColor;
} 