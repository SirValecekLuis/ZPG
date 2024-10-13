//
// Created by tobiasjanca on 10/2/24.
//

#include "shaders.h"

Shader::~Shader() {
    if (shader_id != 0) {
        glDeleteShader(shader_id);
    }
}

VertexShader::VertexShader(const char *shader_string) {
    shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_id, 1, &shader_string, nullptr);
    glCompileShader(shader_id);
}

FragmentShader::FragmentShader(const char *shader_string) {
    shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_id, 1, &shader_string, nullptr);
    glCompileShader(shader_id);
}

ShaderProgram::ShaderProgram(const VertexShader &vertex, const FragmentShader &fragment) {
    shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex.shader_id);
    glAttachShader(shader_id, fragment.shader_id);
    glLinkProgram(shader_id);

    GLint status;
    glGetProgramiv(shader_id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(shader_id, infoLogLength, nullptr, strInfoLog);
        fprintf(stderr, "Linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
}


ShaderProgram::ShaderProgram(const VertexShader &vertex, const FragmentShader &fragment,
                             const Matrix &matrix) : ShaderProgram(vertex, fragment) {
    change_matrix(matrix);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shader_id);
}


void ShaderProgram::change_fragment_shader(const VertexShader &fragment) const {
    glAttachShader(shader_id, fragment.shader_id);
    glLinkProgram(shader_id);
}

void ShaderProgram::change_vertex_shader(const VertexShader &vertex) const {
    glAttachShader(shader_id, vertex.shader_id);
    glLinkProgram(shader_id);
}

void ShaderProgram::use_shader() const {
    glUseProgram(shader_id);

    if (matrix_id != -1) {
        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &mat[0][0]);
    }
}


void ShaderProgram::change_matrix(const Matrix& matrix) {
    mat = matrix.mat;

    matrix_id = glGetUniformLocation(shader_id, "modelMatrix");

    if (matrix_id == -1) {
        std::cerr << "Can't find a var 'modelMatrix'" << std::endl;
    }
}
