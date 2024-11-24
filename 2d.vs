#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    vec4 pos = vec4(aPos.xy, 0.0, 1.0);
    gl_Position = pos;

    TexCoord = aTexCoord;
}
