//
// Created by tobiasjanca on 10/13/24.
//

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <random>

#include "matrix.h"
#include "model.h"
#include "shaders.h"
#include "tree.h"
#include "bushes.h"

#include <GLFW/glfw3.h>

class Scene {
public:
    virtual ~Scene() = default;

    virtual void render() = 0;

    virtual void cleanup() = 0;

    virtual void init() = 0;

protected:
    bool run = false;
};

class ForestScene final : public Scene {
public:
    explicit ForestScene(GLFWwindow *window);

    ~ForestScene() override;

    void cleanup() override;

    void init() override;

    void render() override;

private:
    std::vector<Matrix *> tree_matrices;
    GLFWwindow *window;
    VertexShader *vertex_shader_matrix;
    FragmentShader *fragment_shader_matrix;
    ShaderProgram *shader_program_matrix;
    Model *tree_model;
};


class BushesScene final : public Scene {
public:
    explicit BushesScene(GLFWwindow *window);

    ~BushesScene();

    void cleanup() override;

    void init() override;

    void render() override;

private:
    std::vector<Matrix *> bush_matrices;
    GLFWwindow *window;
    VertexShader *vertex_shader_matrix;
    FragmentShader *fragment_shader_matrix;
    ShaderProgram *shader_program_matrix;
    Model *bush_model;
};


#endif //SCENE_H
