#version 460 core

in vec2 fsTexCoord;
out vec4 fragColor;

uniform sampler2D spriteTex;
uniform vec3 spriteColor;
uniform bool isText;

void main()
{
    vec4 texColor;
    if (isText)
        texColor = vec4(1.f, 1.f, 1.f, texture(spriteTex, fsTexCoord).r);
    else
        texColor = texture(spriteTex, fsTexCoord);

    // Ignore fragments that are transparent below a threshold.
    if (texColor.a < .1)
        discard;

    fragColor = vec4(spriteColor, 1.f) * texColor;
}