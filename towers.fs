#version 460 core

in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 fragColor;

uniform sampler2D texSampler;
uniform sampler2D texSampler2;

void main()
{
    fragColor = mix(texture(texSampler, vertexTexCoord),
                    texture(texSampler2, vertexTexCoord),
                    0.2);
}
