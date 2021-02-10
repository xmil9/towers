#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 vsTexCoord;
out vec2 fsTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 0.f, 1.f);
    fsTexCoord = vsTexCoord;
}