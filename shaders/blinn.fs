#version 330 core

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_Color;

uniform vec3 viewPosition;

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
};

uniform int light_count;
uniform Light lights[10];

void main() {
    vec3 normal = normalize(ex_worldNormal);
    vec3 viewVector = normalize(viewPosition - ex_worldPosition.xyz);

    float shininess = 64.0;
    vec4 baseColor = vec4(0.385, 0.647, 0.812, 1.0);
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0);

    vec4 finalColor = vec4(0.1, 0.1, 0.1, 1.0) * baseColor;

    for (int i = 0; i < light_count; ++i) {
        vec3 lightDir = normalize(lights[i].position - ex_worldPosition.xyz);
        vec3 halfwayVector = normalize(lightDir + viewVector);

        float distance = length(lights[i].position - ex_worldPosition.xyz);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

        float diff = max(dot(lightDir, normal), 0.0);
        vec4 diffuse = diff * baseColor * vec4(lights[i].color * lights[i].intensity, 1.0);

        float spec = pow(max(dot(normal, halfwayVector), 0.0), shininess);
        vec4 specular = spec * specularColor * vec4(lights[i].color * lights[i].intensity, 1.0);

        finalColor += (diffuse + specular) * attenuation;
    }

    out_Color = clamp(finalColor, 0.0, 1.0);
}