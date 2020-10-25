#version 460 core

in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 fragColor;

uniform sampler2D tex;

void main()
{
    fragColor = texture(tex, vertexTexCoord);
}
