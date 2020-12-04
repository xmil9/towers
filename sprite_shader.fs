#version 460 core

in vec2 vertexTexCoord;
out vec4 fragColor;

uniform sampler2D spriteTex;
uniform vec3 spriteColor;

void main()
{
    vec4 texColor = texture(spriteTex, vertexTexCoord);
    // Ignore fragments that are transparent below a threshold.
    if (texColor.a < .1)
        discard;
    fragColor = vec4(spriteColor, 1.0f) * texColor;
}