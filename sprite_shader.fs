#version 460 core

in vec2 vertexTexCoord;
out vec4 fragColor;

uniform sampler2D spriteTex;
uniform vec3 spriteColor;

void main()
{
    fragColor = vec4(1.0f, 0.5f, 0.5f, 1.0f);
    //fragColor = vec4(spriteColor, 1.0f) * texture(spriteTex, vertexTexCoord);
}