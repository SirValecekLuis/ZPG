#version 450
layout(location = 0) in vec3 vp;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

out vec3 fragmentLocalPosition;

void main () {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
    fragmentLocalPosition = vp;
}
