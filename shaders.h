//
// Created by tobiasjanca on 10/2/24.
//

#ifndef SHADERS_H
#define SHADERS_H

// Include GLEW
#include <GL/glew.h>

class Shader {
public:
    virtual ~Shader() = default;

    GLuint shader_id = 0;
};

class VertexShader final : public Shader {
public:
    explicit VertexShader(const char *shader_string);
};

class FragmentShader final : public Shader {
public:
    explicit FragmentShader(const char *shader_string);
};

class ShaderProgram {
public:
    ShaderProgram(const VertexShader &vertex, const FragmentShader &fragment);

    void set_shader() const;

private:
    GLuint shader_id = 0;
};

#endif //SHADERS_H
