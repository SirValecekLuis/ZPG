//
// Created by tobiasjanca on 11/24/24.
//
#ifndef SKYBOX_H
#define SKYBOX_H

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <vector>
#include <string>
#include <GLFW/glfw3.h>

#include "model.h"
#include "shaders.h"

class Skybox {
public:
    explicit Skybox(const std::vector<std::string> &faces);

    ~Skybox();

    void draw(const glm::mat4 &view, const glm::mat4 &projection) const;

    void switch_follow();

private:
    Model *cube_model;
    ShaderProgram *shader;
    GLuint cubemapTexture{};

    static const float skyboxVertices[];

    void load_cubemap(const std::vector<std::string> &faces);

    GLuint VAO, VBO;
    bool camera_follows = true;

    Matrix model_mat = Matrix(0, 0, 0, 0, 0, 50);
};


#endif //SKYBOX_H
