#version 460 core

in vec2 vertexTexCoord;
out vec4 fragColor;

uniform sampler2D spriteTex;
uniform vec3 spriteColor;

void main()
{
    vec3 texColor = vec3(texture(spriteTex, vertexTexCoord));
    fragColor = vec4(spriteColor, 1.0f) * vec4(texColor, 1.f);
}