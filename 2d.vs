#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fragColor;

void main()
{
    vec4 pos = vec4(aPos.xy, 0.0, 1.0);

    gl_Position = pos;

    fragColor = aNormal;
}