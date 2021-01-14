#version 460 core

in vec2 vertexTexCoord;
out vec4 fragColor;

uniform sampler2D textTex;
uniform vec3 textColor;

void main()
{    
    vec4 sampled = vec4(1.f, 1.f, 1.f, texture(textTex, vertexTexCoord).r);
    fragColor = vec4(textColor, 1.f) * sampled;
}