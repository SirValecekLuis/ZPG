#version 330 core
in vec2 TexCoord;

uniform sampler2D textureSampler;
uniform bool useTexture;

out vec4 FragColor;

void main()
{
    if(useTexture) {
        vec4 texColor = texture(textureSampler, TexCoord);

        if(texColor.a < 0.1) {
            discard;
        }

        FragColor = texColor;
    } else {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
