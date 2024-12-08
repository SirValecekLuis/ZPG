//
// Created by tobiasjanca on 10/2/24.
//

#include "shaders.h"

#include "material.h"

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
    light_count_id = glGetUniformLocation(shader_id, "light_count");
    texture_sampler_id = glGetUniformLocation(shader_id, "textureSampler");
    use_sampler_id = glGetUniformLocation(shader_id, "useTextureSampler");
    skybox_id = glGetUniformLocation(shader_id, "skyboxTexture");
    texture_unit_id = glGetUniformLocation(shader_id, "textureUnitID");
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(shader_id);

    // for (auto &light: lights) {
    //     if (light != nullptr) {
    //         delete light;
    //         light = nullptr;
    //     }
    // }

    lights.clear();
}

void ShaderProgram::use_shader() const {
    glUseProgram(shader_id);
}

void ShaderProgram::add_light(const LightType type, const glm::vec3 &position, const glm::vec3 &color,
                              const float intensity) {
    if (lights.size() < 10) {
        auto *new_light = new Light(type, position, color, intensity);
        lights.push_back(new_light);
    }
}

void ShaderProgram::add_light(Light *light) {
    if (lights.size() < 10) {
        lights.push_back(light);
    }
}

void ShaderProgram::update_lights() const {
    if (light_count_id != -1) {
        glUniform1i(light_count_id, static_cast<GLint>(lights.size()));
    }

    for (size_t i = 0; i < lights.size(); ++i) {
        std::string base = "lights[" + std::to_string(i) + "].";

        glUniform1i(glGetUniformLocation(shader_id, (base + "type").c_str()),
                    static_cast<GLint>(lights[i]->type));

        glUniform3fv(glGetUniformLocation(shader_id, (base + "position").c_str()),
                     1, &lights[i]->position[0]);

        glUniform3fv(glGetUniformLocation(shader_id, (base + "direction").c_str()),
                     1, &lights[i]->direction[0]);

        glUniform3fv(glGetUniformLocation(shader_id, (base + "color").c_str()),
                     1, &lights[i]->color[0]);

        glUniform1f(glGetUniformLocation(shader_id, (base + "intensity").c_str()),
                    lights[i]->intensity);

        glUniform1f(glGetUniformLocation(shader_id, (base + "cutOff").c_str()),
                    lights[i]->cutOff);
        glUniform1f(glGetUniformLocation(shader_id, (base + "outerCutOff").c_str()),
                    lights[i]->outerCutOff);
    }
}

void ShaderProgram::set_model_mat(const glm::mat4 &mod) {
    model_mat = mod;
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

    bind_texture_uniforms();
    update_lights();
}

void ShaderProgram::update(Subject *subject) {
    if (const auto *camera = dynamic_cast<Camera *>(subject)) {
        set_view_matrix(camera->get_view_matrix());
        set_projection_matrix(camera->get_projection_matrix());
        set_camera_position(camera->get_camera_pos());
    }
}

void ShaderProgram::apply_material(const Material &material) const {
    glUniform1f(glGetUniformLocation(shader_id, "material_ambient"), material.get_ra());
    glUniform1f(glGetUniformLocation(shader_id, "material_diffuse"), material.get_rd());
    glUniform1f(glGetUniformLocation(shader_id, "material_specular"), material.get_rs());
    glUniform1f(glGetUniformLocation(shader_id, "material_shininess"), 64.0f);
}

void ShaderProgram::bind_texture_uniforms(const GLuint textureUnit) const {
    if (texture_sampler_id != -1) {
        glUniform1i(texture_sampler_id, textureUnit);
    }
    if (use_sampler_id != -1) {
        glUniform1i(use_sampler_id, is_using_texture ? 1 : 0);
    }
}

void ShaderProgram::set_use_texture(const bool use_texture) {
    is_using_texture = use_texture;
}

void ShaderProgram::set_skyboxTexture(const int value) const {
    if (skybox_id != -1) {
        glUniform1i(skybox_id, value);
    }
}

void ShaderProgram::set_texture_unit(const int value) const {
    if (texture_unit_id != -1) {
        glUniform1i(texture_unit_id, value);
    }
}
