#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <random>
#include "matrix.h"
#include "model.h"
#include "shaders.h"
#include "camera.h"
#include "tree.h"
#include "bushes.h"
#include "plain.h"
#include "sphere.h"
#include <GLFW/glfw3.h>

class DrawableObject {
public:
    Model *model;
    std::vector<Matrix *> matrices;
    ShaderProgram *shader_program;

    DrawableObject(Model *model, const std::vector<Matrix *> &matrices, ShaderProgram *shader_program)
        : model(model), matrices(matrices), shader_program(shader_program) {
    }

    DrawableObject(Model *model, Matrix *matrix, ShaderProgram *shader_program)
        : model(model), shader_program(shader_program) {
        matrices.push_back(matrix);
    }
};

class Scene {
public:
    explicit Scene(GLFWwindow *window);

    virtual ~Scene();

    virtual void render();

    virtual void cleanup();

    virtual void init() = 0;

    void init_camera(glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f), float pitch = -90.f);

    void add_render_object(DrawableObject *render_object);

    Camera *camera;

protected:
    GLFWwindow *window;
    std::vector<DrawableObject *> render_objects;
    std::vector<ShaderProgram *> shader_programs;
};

class ForestScene final : public Scene {
public:
    explicit ForestScene(GLFWwindow *window);

    ~ForestScene() override = default;

    void init() override;
};

class BasicScene final : public Scene {
public:
    explicit BasicScene(GLFWwindow *window);

    ~BasicScene() override = default;

    void init() override;
};

class PhongScene final : public Scene {
public:
    explicit PhongScene(GLFWwindow *window);

    ~PhongScene() override = default;

    void init() override;
};

class AllLightsScene final : public Scene {
public:
    explicit AllLightsScene(GLFWwindow *window);

    void init() override;
};

#endif //SCENE_H