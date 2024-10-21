#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <random>

#include "matrix.h"
#include "model.h"
#include "shaders.h"
#include "camera.h"
#include "tree.h"
#include "bushes.h"

#include <GLFW/glfw3.h>

class Scene {
public:
    explicit Scene(GLFWwindow *window);

    virtual ~Scene();

    virtual void render();

    virtual void cleanup();

    virtual void init();

    void add_model(Model *model);

    Camera *camera;

protected:
    GLFWwindow *window;
    std::vector<Model *> models;
    std::vector<Matrix *> matrices;
    VertexShader *vertex_shader_matrix;
    FragmentShader *fragment_shader_matrix;
    ShaderProgram *shader_program_matrix;
};

class ForestScene final : public Scene {
public:
    explicit ForestScene(GLFWwindow *window);

    ~ForestScene() override = default;

    void init() override;
};

class BushesScene final : public Scene {
public:
    explicit BushesScene(GLFWwindow *window);

    ~BushesScene() override = default;

    void init() override;
};

#endif //SCENE_H
