#version 430 core

struct Material {
    float ambientStrength;
    float diffuseStrength;
    float specularStrength;
    float shininess;
    sampler2D textureSampler;
    sampler2D textureSamplerSpecular;
};

in vec3 fragNormal;
in vec3 fragPosition;
in vec2 fragTexCoords;

out vec4 FragColor;

uniform Material material;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;

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

    FragColor = texture(material.textureSampler, fragTexCoords) * vec4((diffusion + material.ambientStrength) * lightColor, 1.0) +
        texture(material.textureSamplerSpecular, fragTexCoords) * vec4((specular) * lightColor, 1.0);

}
