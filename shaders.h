#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "matrix.h"
#include "camera.h"
#include "observer.h"
#include "shader_loader.h"
#include "light.h"
#include "material.h"

class ShaderProgram final : public Observer {
public:
    ShaderProgram(const char *vertex_file, const char *fragment_file);

    ~ShaderProgram() override;

    void use_shader() const;

    void set_model_mat(const Matrix &matrix);

    void set_view_matrix(const glm::mat4 &view);

    void set_projection_matrix(const glm::mat4 &projection);

    void set_normal_matrix();

    void set_camera_position(const glm::vec3 &pos);

    void update(Subject *subject) override;

    void update_all_matrices();

    void add_light(LightType type, const glm::vec3 &position = glm::vec3(0.f, 1.f, 0.f),
                   const glm::vec3 &color = glm::vec3(1.f, 1.f, 1.f),
                   float intensity = 0.8f);

    void add_light(Light *light);

    void update_lights() const;

    void apply_material(Material& material);

private:
    GLuint shader_id;
    GLint model_matrix_id;
    GLint view_matrix_id;
    GLint projection_matrix_id;
    GLint normal_matrix_id;
    GLint camera_pos_id;
    GLint light_count_id;

    std::vector<Light *> lights;
    glm::mat4 model_mat{};
    glm::mat4 view_mat{};
    glm::mat4 projection_mat{};
    glm::mat3 normal_matrix{};
    glm::vec3 camera_pos{};

    ShaderLoader shader_loader;
};

#endif // SHADERS_H
