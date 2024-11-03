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

class Scene {
public:
    explicit Scene(GLFWwindow *window);
    virtual ~Scene();

    virtual void render();
    virtual void cleanup();
    virtual void init();
    void init_camera(glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f), float pitch = -90.f);

    void add_model(Model *model, const std::vector<Matrix*>& model_matrices);
    void add_model(Model *model, Matrix* matrix);

    Camera *camera;

protected:
    GLFWwindow *window;
    std::map<Model*, std::vector<Matrix*>> model_matrices_map;
    VertexShader *vertex_shader;
    FragmentShader *fragment_shader;
    ShaderProgram *shader_program;
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

#endif //SCENE_H
