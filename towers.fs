#version 460 core

in vec3 vertexNormal;
in vec3 fragPos;
in vec4 vertexColor;
in vec2 vertexTexCoord;
out vec4 fragColor;

uniform sampler2D texSampler;
uniform sampler2D texSampler2;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambient;

void main()
{
    vec3 objColor = vec3(mix(texture(texSampler, vertexTexCoord),
                             texture(texSampler2, vertexTexCoord),
                             0.2));

    // The diffuse intensity depends on angle of the incoming light and
    // the fragment/vertex normal.
    vec3 lightDir = normalize(lightPos - fragPos);  
    float diffuseIntensity = max(dot(normalize(vertexNormal), lightDir), 0.0);
    vec3 diffuse = lightColor * diffuseIntensity;
    
    fragColor = vec4((ambient + diffuse) * objColor, 1.0f);
}
