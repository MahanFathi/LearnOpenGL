#version 430 core

struct Material {
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 FragColor;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cubeColor;
uniform Material material;

void main()
{
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    vec3 viewDirection = normalize(cameraPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, fragNormal);

    // diffusion
    float diffusion = material.diffuseStrength *
        max(dot(fragNormal, lightDirection), 0.0);

    // specular
    float specular = material.specularStrength *
        pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

    FragColor = vec4((specular + diffusion + material.ambientStrength) * lightColor * cubeColor, 1.0);
}
