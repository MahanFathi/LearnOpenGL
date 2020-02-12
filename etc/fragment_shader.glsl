#version 430 core

in vec3 fragNormal;
in vec3 fragPosition;

out vec4 FragColor;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 cubeColor;

void main()
{
    float ambient = 0.1;
    vec3 lightDirection = normalize(lightPosition - fragPosition);
    float lightingDiff = max(dot(fragNormal, lightDirection), 0.0);
    FragColor = vec4((lightingDiff + ambient) * lightColor * cubeColor, 1.0);
}
