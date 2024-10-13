//
// Created by tobiasjanca on 10/2/24.
//

#ifndef SHADERS_H
#define SHADERS_H

#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/glm.hpp>

#include "matrix.h"


class Shader {
public:
    virtual ~Shader();

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

    ShaderProgram(const VertexShader &vertex, const FragmentShader &fragment, const Matrix& matrix);

    ~ShaderProgram();

    void use_shader() const;

    void change_vertex_shader(const VertexShader &vertex) const;

    void change_fragment_shader(const VertexShader &fragment) const;

    void change_matrix(const Matrix& matrix);

private:
    GLuint shader_id = 0;
    GLint matrix_id = -1;
    glm::mat4 mat{};
};

#endif //SHADERS_H
