#version 460 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;
layout (location = 3) in vec2 texCoord;
out vec3 vertexNormal;
out vec3 fragPos;
out vec4 vertexColor;
out vec2 vertexTexCoord;

uniform mat4 model;
uniform mat3 normalMat;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos, 1.0);
	fragPos = vec3(model * vec4(pos, 1.0));
	vertexNormal = normalMat * normal;
	vertexColor = color;
	vertexTexCoord = texCoord;
}
