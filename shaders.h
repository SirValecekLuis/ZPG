#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "matrix.h"
#include "camera.h"
#include "observer.h"
#include "shader_loader.h"

class ShaderProgram final : public Observer {
public:
    ShaderProgram(const char *vertex_file, const char *fragment_file);
    ~ShaderProgram() override;

    void use_shader() const;
    void set_model_mat(const Matrix &matrix);
    void set_view_matrix(const glm::mat4& view);
    void set_projection_matrix(const glm::mat4& projection);
    void set_normal_matrix();
    void set_camera_position(const glm::vec3& pos);
    void update(Subject* subject) override;
    void update_all_matrices();

private:
    GLuint shader_id = 0;
    GLint model_matrix_id = -1;
    GLint view_matrix_id = -1;
    GLint projection_matrix_id = -1;
    GLint normal_matrix_id = -1;
    GLint camera_pos_id = -1;

    glm::mat4 model_mat{};
    glm::mat4 view_mat{};
    glm::mat4 projection_mat{};
    glm::mat3 normal_matrix{};
    glm::vec3 camera_pos{};

    ShaderLoader shader_loader;
};

#endif // SHADERS_H
