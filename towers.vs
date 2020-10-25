#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
out vec4 vertexColor;
out vec2 vertexTexCoord;

void main()
{
	gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);
	vertexColor = color;
	vertexTexCoord = texCoord;
}
