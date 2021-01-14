#version 460 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;
out vec2 vertexTexCoord;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * vec4(pos, 0.f, 1.f);
    vertexTexCoord = texCoord;
}