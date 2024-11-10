//
// Created by tobiasjanca on 10/2/24.
//

#include "shaders.h"

ShaderProgram::ShaderProgram(const char *vertex_file, const char *fragment_file) {
    shader_id = shader_loader.loadShader(vertex_file, fragment_file);

    GLint status;
    glGetProgramiv(shader_id, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLength;
        glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto *strInfoLog = new GLchar[infoLogLength + 1];
        glGetProgramInfoLog(shader_id, infoLogLength, nullptr, strInfoLog);
        fprintf(stderr, "Shader Program linker failure: %s\n", strInfoLog);
        delete[] strInfoLog;
    }

    view_matrix_id = glGetUniformLocation(shader_id, "viewMatrix");
    model_matrix_id = glGetUniformLocation(shader_id, "modelMatrix");
    projection_matrix_id = glGetUniformLocation(shader_id, "projectionMatrix");
    normal_matrix_id = glGetUniformLocation(shader_id, "normalMatrix");
    camera_pos_id = glGetUniformLocation(shader_id, "viewPosition");
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shader_id);
}

void ShaderProgram::use_shader() const {
    glUseProgram(shader_id);
}

void ShaderProgram::set_model_mat(const Matrix &matrix) {
    model_mat = matrix.mat;
}


void ShaderProgram::set_view_matrix(const glm::mat4 &view) {
    view_mat = view;
}


void ShaderProgram::set_projection_matrix(const glm::mat4 &projection) {
    projection_mat = projection;
}


void ShaderProgram::set_normal_matrix() {
    normal_matrix = glm::transpose(glm::inverse(glm::mat3(model_mat)));
}

void ShaderProgram::set_camera_position(const glm::vec3 &pos) {
    camera_pos = pos;
}

void ShaderProgram::update_all_matrices() {
    if (model_matrix_id != -1) {
        glUniformMatrix4fv(model_matrix_id, 1, GL_FALSE, &model_mat[0][0]);
    }
    if (view_matrix_id != -1) {
        glUniformMatrix4fv(view_matrix_id, 1, GL_FALSE, &view_mat[0][0]);
    }
    if (projection_matrix_id != -1) {
        glUniformMatrix4fv(projection_matrix_id, 1, GL_FALSE, &projection_mat[0][0]);
    }
    if (normal_matrix_id != -1) {
        glUniformMatrix3fv(normal_matrix_id, 1, GL_FALSE, &normal_matrix[0][0]);
    }

    if (camera_pos_id != -1) {
        glUniform3fv(camera_pos_id, 1, &camera_pos[0]);
    }
}

void ShaderProgram::update(Subject *subject) {
    if (const auto *camera = dynamic_cast<Camera *>(subject)) {
        set_view_matrix(camera->get_view_matrix());
        set_projection_matrix(camera->get_projection_matrix());
        set_camera_position(camera->get_camera_pos());
    }
}
