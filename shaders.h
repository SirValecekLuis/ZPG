//
// Created by tobiasjanca on 10/2/24.
//

#ifndef SHADERS_H
#define SHADERS_H

#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "matrix.h"
#include "camera.h"
#include "observer.h"

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

class ShaderProgram final : public Observer {
public:
    ShaderProgram(const VertexShader &vertex, const FragmentShader &fragment);

    ~ShaderProgram() override;

    void use_shader() const;

    void change_vertex_shader(const VertexShader &vertex) const;

    void change_fragment_shader(const VertexShader &fragment) const;

    void set_model_mat(const Matrix &matrix);

    void set_view_matrix(const glm::mat4& view);

    void set_projection_matrix(const glm::mat4& projection);

    void set_normal_matrix();

    void set_camera_position(const glm::vec3& pos) const;

    void update(Subject* subject) override;

private:
    GLuint shader_id = 0;
    GLint model_matrix_id = -1;
    GLint view_matrix_id = -1;
    GLint projection_matrix_id = -1;
    GLint normal_matrix_id = -1;

    glm::mat4 model_mat{};
    glm::mat4 view_mat{};
    glm::mat4 projection_mat{};
    glm::mat3 normal_matrix{};

};

#endif //SHADERS_H
