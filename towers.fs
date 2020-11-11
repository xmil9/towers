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
uniform vec3 viewPos;

void main()
{
    vec3 objColor = vec3(mix(texture(texSampler, vertexTexCoord),
                             texture(texSampler2, vertexTexCoord),
                             0.2));

    vec3 vnorm = normalize(vertexNormal);
    vec3 lightDir = normalize(lightPos - fragPos);  
    
    // The diffuse intensity depends on angle of the incoming light and
    // the fragment/vertex normal.
    float diffuseIntensity = max(dot(vnorm, lightDir), 0.0);
    vec3 diffuse = lightColor * diffuseIntensity;
    
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectionDir = reflect(-lightDir, vnorm);
    float specularIntensity = pow(max(dot(viewDir, reflectionDir), 0.0), 8);
    float specularStrength = 0.5;
    vec3 specular = specularStrength * specularIntensity * lightColor;

    fragColor = vec4((ambient + diffuse + specular) * objColor, 1.0f);
}
