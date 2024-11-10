#version 330 core

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_Color;

uniform vec3 viewPosition;

void main() {
    vec3 lightPosition = vec3(0.0, 0.0, 0.0);

    vec3 normal = normalize(ex_worldNormal);
    vec3 lightVector = normalize(lightPosition - ex_worldPosition.xyz);

    float dot_product = max(dot(lightVector, normal), 0.0);

    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);

    out_Color = ambient + diffuse;
}