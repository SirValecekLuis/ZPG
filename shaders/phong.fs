#version 330 core

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;
in vec2 ex_TexCoord;

out vec4 out_Color;

uniform vec3 viewPosition;
uniform sampler2D textureSampler;
uniform bool useTextureSampler;

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float cutOff;
    float outerCutOff;
    int type;  // 0 = POINT, 1 = DIRECTIONAL, 2 = SPOTLIGHT
};

uniform int light_count;
uniform Light lights[10];

uniform float material_ambient;   // ra
uniform float material_diffuse;   // rd
uniform float material_specular;  // rs
uniform float material_shininess; // shininess

vec4 calculatePointLight(Light light, vec3 normal, vec3 viewDir, vec4 baseColor) {
    vec3 lightDir = normalize(light.position - ex_worldPosition.xyz);
    float distance = length(light.position - ex_worldPosition.xyz);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    vec4 ambient = material_ambient * vec4(light.color * light.intensity, 1.0) * baseColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = material_diffuse * diff * vec4(light.color * light.intensity, 1.0) * baseColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    vec4 specular = material_specular * spec * vec4(light.color, 1.0);

    return (ambient + diffuse + specular) * attenuation;
}

vec4 calculateDirectionalLight(Light light, vec3 normal, vec3 viewDir, vec4 baseColor) {
    vec3 lightDir = normalize(-light.direction);

    vec4 ambient = material_ambient * vec4(light.color * light.intensity, 1.0) * baseColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = material_diffuse * diff * vec4(light.color * light.intensity, 1.0) * baseColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    vec4 specular = material_specular * spec * vec4(light.color, 1.0);

    return ambient + diffuse + specular;
}

vec4 calculateSpotLight(Light light, vec3 normal, vec3 viewDir, vec4 baseColor) {
    vec3 lightDir = normalize(light.position - ex_worldPosition.xyz);

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float distance = length(light.position - ex_worldPosition.xyz);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    vec4 ambient = material_ambient * vec4(light.color * light.intensity, 1.0) * baseColor;

    float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = material_diffuse * diff * vec4(light.color * light.intensity, 1.0) * baseColor;

    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
    vec4 specular = material_specular * spec * vec4(light.color, 1.0);

    return (ambient + diffuse + specular) * attenuation * intensity;
}

void main() {
    vec3 normal = normalize(ex_worldNormal);
    vec3 viewDir = normalize(viewPosition - ex_worldPosition.xyz);

    vec4 baseColor;
    if (useTextureSampler) {
        baseColor = texture(textureSampler, ex_TexCoord);
    } else {
        baseColor = vec4(0.385, 0.647, 0.812, 1.0);
    }

    vec4 finalColor = vec4(0.0);
    for (int i = 0; i < light_count; ++i) {
        if (lights[i].type == 0) {  // Point light
            finalColor += calculatePointLight(lights[i], normal, viewDir, baseColor);
        }
        else if (lights[i].type == 1) {  // Directional light
            finalColor += calculateDirectionalLight(lights[i], normal, viewDir, baseColor);
        }
        else if (lights[i].type == 2) {  // Spotlight
            finalColor += calculateSpotLight(lights[i], normal, viewDir, baseColor);
        }
    }

    out_Color = clamp(finalColor, 0.0, 1.0);
}
