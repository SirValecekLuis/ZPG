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

    GLint status;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto *strInfoLog = new GLchar[infoLogLength + 1];
        glGetShaderInfoLog(shader_id, infoLogLength, nullptr, strInfoLog);
        fprintf(stderr, "Shader compilation failed: %s\n", strInfoLog);
        delete[] strInfoLog;
    }
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

    if (model_matrix_id != -1) {
        glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &model_mat[0][0]);
    }

    // if (view_matrix_id != -1) {
    //     glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, &view_mat[0][0]);
    // }
    //
    // if (projection_matrix_id != -1) {
    //     glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, &projection_mat[0][0]);
    // }
    //
    // if (normal_matrix_id != -1) {
    //     glUniformMatrix3fv(projection_matrix_id, 1, GL_FALSE, &normal_matrix[0][0]);
    // }
}


void ShaderProgram::set_model_mat(const Matrix &matrix) {
    model_mat = matrix.mat;

    model_matrix_id = glGetUniformLocation(shader_id, "modelMatrix");

    if (model_matrix_id == -1) {
        std::cerr << "Can't find a var 'modelMatrix'" << std::endl;
    }
}


void ShaderProgram::set_view_matrix(const glm::mat4 &view) {
    view_mat = view;

    view_matrix_id = glGetUniformLocation(shader_id, "viewMatrix");

    if (view_matrix_id != -1) {
        glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, &view_mat[0][0]);
    }
}


void ShaderProgram::set_projection_matrix(const glm::mat4 &projection) {
    projection_mat = projection;

    projection_matrix_id = glGetUniformLocation(shader_id, "projectionMatrix");

    if (projection_matrix_id != -1) {
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, &projection_mat[0][0]);
    }
}


void ShaderProgram::set_normal_matrix() {
    normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_mat)));

    normal_matrix_id = glGetUniformLocation(shader_id, "normalMatrix");

    if (normal_matrix_id != -1) {
        glUniformMatrix3fv(normal_matrix_id, 1, GL_FALSE, &normal_matrix[0][0]);
    }
}

void ShaderProgram::set_camera_position(const glm::vec3& pos) const {
    if (const GLint camera_pos_id = glGetUniformLocation(shader_id, "viewPosition"); camera_pos_id != -1) {
        glUniform3fv(camera_pos_id, 1, &pos[0]);
    }
}

void ShaderProgram::update(Subject *subject) {
    if (const auto *camera = dynamic_cast<Camera *>(subject)) {
        set_view_matrix(camera->get_view_matrix());
        set_projection_matrix(camera->get_projection_matrix());
        set_camera_position(camera->get_camera_pos());
    }
}
