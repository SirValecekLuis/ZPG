//
// Created by tobiasjanca on 10/13/24.
//

#include "scene.h"

auto vertex_shader_str_matrix = R"glsl(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec3 aColor;

        out vec3 fragColor;

        uniform mat4 modelMatrix;
        uniform mat4 viewMatrix;
        uniform mat4 projectionMatrix;

        void main()
        {
            gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
            fragColor = aColor;
        }
)glsl";

auto fragment_shader_str_matrix = R"glsl(
        #version 330 core
        in vec3 fragColor;

        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(fragColor, 1.0);
        }
)glsl";

std::vector<Matrix *> generate_random_matrices(const int tree_num = 15, const float scale = 0.05) {
    std::vector<Matrix *> matrices;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> x_dis(-0.4f, 0.f);
    std::uniform_real_distribution<> y_dis(-0.4f, 0.f);
    std::uniform_real_distribution<> z_dis(-40.f, 0.f);
    std::uniform_real_distribution<> rot_dis(-20.f, 20.f);
    std::uniform_real_distribution<> scale_dis(scale - 1 / 2.f * scale, scale + 1 / 2.f * scale);

    for (int i = 0; i < tree_num; ++i) {
        float x = x_dis(gen);
        float y = y_dis(gen);
        float z = z_dis(gen);
        float rotateX = rot_dis(gen);
        float rotateY = rot_dis(gen);
        float scale = scale_dis(gen);

        matrices.push_back(new Matrix(rotateX, rotateY, x, y, z, scale));
    }

    return matrices;
}

Scene::Scene(GLFWwindow *window) : window(window), vertex_shader_matrix(nullptr),
                                   fragment_shader_matrix(nullptr), shader_program_matrix(nullptr),
                                   camera(nullptr) {
}

Scene::~Scene() {
    Scene::cleanup();
}

void Scene::init() {
    vertex_shader_matrix = new VertexShader(vertex_shader_str_matrix);
    fragment_shader_matrix = new FragmentShader(fragment_shader_str_matrix);
    shader_program_matrix = new ShaderProgram(*vertex_shader_matrix, *fragment_shader_matrix);
    camera = new Camera();
    camera->add_observer(shader_program_matrix);
    camera->update_camera_vectors();
    camera->notify_observers();
}

void Scene::render() {
    for (const auto &matrix: matrices) {
        for (const auto &model: models) {
            shader_program_matrix->set_model_mat(*matrix);
            model->draw();
        }
    }
}

void Scene::cleanup() {
    for (const auto &matrix: matrices) {
        delete matrix;
    }

    for (const auto &model: models) {
        delete model;
    }
    matrices.clear();
    models.clear();

    delete vertex_shader_matrix;
    delete fragment_shader_matrix;
    delete shader_program_matrix;
    delete camera;

    vertex_shader_matrix = nullptr;
    fragment_shader_matrix = nullptr;
    shader_program_matrix = nullptr;
    camera = nullptr;
}

void Scene::add_model(Model *model) {
    models.emplace_back(model);
}

ForestScene::ForestScene(GLFWwindow *window) : Scene(window) {
}

void ForestScene::init() {
    Scene::init();
    matrices = generate_random_matrices(50, 0.4);
    auto *tree_model = new Model(tree, sizeof(tree), *shader_program_matrix);
    add_model(tree_model);
}

BushesScene::BushesScene(GLFWwindow *window) : Scene(window) {
}

void BushesScene::init() {
    Scene::init();
    matrices = generate_random_matrices(200, 0.4);
    auto *bush_model = new Model(bushes, sizeof(bushes), *shader_program_matrix);
    add_model(bush_model);
}
