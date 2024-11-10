#version 330 core

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_Color;

uniform vec3 viewPosition;

void main() {
    vec3 lightPosition = vec3(0.0, 0.0, 0.0);

    vec3 normal = normalize(ex_worldNormal);
    vec3 lightVector = normalize(lightPosition - ex_worldPosition.xyz);
    vec3 viewVector = normalize(viewPosition - ex_worldPosition.xyz);

    vec3 halfwayVector = normalize(lightVector + viewVector);

    float shininess = 64.0;
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0);

    float dot_product = max(dot(lightVector, normal), 0.0);
    float spec = pow(max(dot(normal, halfwayVector), 0.0), shininess);

    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);
    vec4 specular = spec * specularColor;

    out_Color = ambient + diffuse + specular;
}