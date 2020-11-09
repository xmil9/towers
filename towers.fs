#version 460 core

in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 fragColor;

uniform sampler2D texSampler;
uniform sampler2D texSampler2;
uniform vec3 lightColor;

void main()
{
    vec3 objColor = vec3(mix(texture(texSampler, vertexTexCoord),
                             texture(texSampler2, vertexTexCoord),
                             0.2));
    fragColor = vec4(lightColor * objColor, 1.0f);
}
