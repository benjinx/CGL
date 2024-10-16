#version 450 core

layout (location = 0) in vec3 vNormal;
layout (location = 1) in vec2 vTexcoord;
layout (location = 2) in vec3 vPosition;
layout (location = 3) in vec3 vNormalOG;

layout (location = 2) uniform vec3 uEyePosition;
layout (location = 3) uniform vec3 uLightPosition;


layout (location = 0) out vec4 FragColor;

void main()
{
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    vec3 objectColor = vec3(abs(vNormalOG));

    // Ambient
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPosition - vPosition);
    float diffuseAmount = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = diffuseAmount * lightColor;

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDirection = normalize(uEyePosition - vPosition);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);
    vec3 specular = specularStrength * specularAmount * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    // halfLambert
    // scaled by 1/2, add 1/2 and squared
    float halfLambertDiffuse = pow(diffuseAmount * 0.5 + 0.5, 2.0f);
    vec3 colour = diffuseAmount * objectColor;
    colour += specular + ambient;

    FragColor = vec4(colour.rgb, 1.0f);//vec4(colour.rgb, 1.0f);
}