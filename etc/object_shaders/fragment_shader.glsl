#version 430 core

struct Material {
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
};

in vec3 fragNormal;
in vec3 fragPosition;
in vec2 fragTexCoords;

out vec4 FragColor;

uniform Material material;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

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

    FragColor = texture(texture_diffuse1, fragTexCoords) * vec4((diffusion + material.ambientStrength) * lightColor, 1.0) +
        texture(texture_specular1, fragTexCoords) * vec4((specular) * lightColor, 1.0);

}
