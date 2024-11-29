#include "skybox.h"
#include <stdexcept>

const float Skybox::skyboxVertices[] = {
    -1.0f, 1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,

    -1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, -1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f
};

Skybox::Skybox(const std::vector<std::string> &faces) {
    cube_model = new Model(skyboxVertices, sizeof(skyboxVertices), 3 * sizeof(GLfloat));

    shader = new ShaderProgram("../skybox.vs", "../skybox.fs");

    load_cubemap(faces);
}

Skybox::~Skybox() {
    delete cube_model;
    delete shader;
    glDeleteTextures(1, &cubemapTexture);
}

void Skybox::load_cubemap(const std::vector<std::string> &faces) {
    if (faces.size() != 6) {
        throw std::runtime_error("Skybox requires exactly 6 texture faces");
    }

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glActiveTexture(GL_TEXTURE0);
    cubemapTexture = SOIL_load_OGL_cubemap(
        faces[0].c_str(), faces[1].c_str(), faces[2].c_str(),
        faces[3].c_str(), faces[4].c_str(), faces[5].c_str(),
        SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS
    );

    if (cubemapTexture == 0) {
        std::cerr << "Failed to load cubemap texture: " << SOIL_last_result() << std::endl;
        exit(EXIT_FAILURE);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
}

void Skybox::draw(const glm::mat4 &view, const glm::mat4 &projection) const {
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 mod_view = view;
    if (camera_follows) {
        mod_view = glm::mat4(glm::mat3(view));
    }

    shader->use_shader();
    shader->set_skyboxTexture(0);
    shader->set_model_mat(model_mat.mat);
    shader->set_view_matrix(mod_view);
    shader->set_projection_matrix(projection);
    shader->update_all_matrices();

    cube_model->bind_vao();
    cube_model->draw();

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Skybox::switch_follow() {
    camera_follows = !camera_follows;
    printf("Camera follows: %d\n", camera_follows);
}
