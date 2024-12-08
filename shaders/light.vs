#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;
layout (location = 2) in vec2 in_TexCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec4 ex_worldPosition;
out vec3 ex_worldNormal;
out vec2 ex_TexCoord;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
    ex_worldPosition = modelMatrix * vec4(in_Position, 1.0);
    ex_worldNormal = normalize(normalMatrix * in_Normal);
    ex_TexCoord = in_TexCoord;
}