#version 430 core

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 FragColor;

uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cubeColor;

void main()
{
    float ambient = 0.1;
    float specularStrength = 0.5;
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    vec3 viewDirection = normalize(cameraPosition - fragPosition);
    vec3 reflectDirection = reflect(-lightDirection, fragNormal);

    // diffusion
    float diffusion = max(dot(fragNormal, lightDirection), 0.0);

    // specular
    float specular = specularStrength * pow(max(dot(viewDirection, reflectDirection), 0.0), 16);

    FragColor = vec4((specular + diffusion + ambient) * lightColor * cubeColor, 1.0);
}
