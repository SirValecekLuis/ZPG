#include "scene.h"
#include <iostream>
#include <random>

std::vector<Matrix *> generate_grid_matrices(const int grid_size = 7, const float scale = 0.05,
                                             const float spacing = 0.2f) {
    std::vector<Matrix *> matrices;
    std::random_device rd;
    std::mt19937 gen(rd());
    const float start_x = -((grid_size - 1) * spacing) / 2;
    const float start_z = -((grid_size - 1) * spacing) / 2;

    std::uniform_real_distribution<> rot_dis(-20.f, 20.f);
    std::uniform_real_distribution<> scale_dis(scale - 1 / 2.f * scale, scale + 1 / 2.f * scale);

    for (int row = 0; row < grid_size; ++row) {
        for (int col = 0; col < grid_size; ++col) {
            float x = start_x + col * spacing;
            float z = start_z + row * spacing;
            float y = 0.0f;
            float rotate_x = rot_dis(gen);
            float rotate_y = rot_dis(gen);
            float current_scale = scale_dis(gen);

            matrices.push_back(new Matrix(rotate_x, rotate_y, x, y, z, current_scale));
        }
    }
    return matrices;
}

Scene::Scene(GLFWwindow *window) : camera(nullptr), window(window) {
}

Scene::~Scene() {
    Scene::cleanup();
}

void Scene::init_camera(const glm::vec3 position, const float pitch) {
    camera = new Camera(position, pitch);
    for (auto *shader: shader_programs) {
        camera->add_observer(shader);
    }
    camera->update_camera_vectors();
    camera->notify_observers();
}

void Scene::render() {
    for (const auto &drawable_object: render_objects) {
        drawable_object->shader_program->use_shader();
        for (const auto &matrix: drawable_object->matrices) {
            drawable_object->shader_program->set_model_mat(*matrix);
            drawable_object->shader_program->set_normal_matrix();
            drawable_object->model->draw();
        }
    }
}

void Scene::cleanup() {
    for (const auto &render_object: render_objects) {
        for (const auto *matrix: render_object->matrices) {
            delete matrix;
        }

        delete render_object;
    }
    render_objects.clear();

    for (const auto *shader: shader_programs) {
        delete shader;
    }
    shader_programs.clear();

    delete camera;
    camera = nullptr;
}

void Scene::add_render_object(DrawableObject *render_object) {
    render_objects.push_back(render_object);
}

ForestScene::ForestScene(GLFWwindow *window) : Scene(window) {
}

void ForestScene::init() {
    auto *tree_shader = new ShaderProgram("../3d.vs", "../3d.fs");
    shader_programs.push_back(tree_shader);
    init_camera();

    auto *tree_model = new Model(tree, sizeof(tree));
    auto *bush_model = new Model(bushes, sizeof(bushes));
    auto *plain_model = new Model(plain, sizeof(plain));

    const auto tree_matrices = generate_grid_matrices(8, 0.4, 4);
    const auto bush_matrices = generate_grid_matrices(18, 0.7, 1.5);

    add_render_object(new DrawableObject(tree_model, tree_matrices, tree_shader));
    add_render_object(new DrawableObject(bush_model, bush_matrices, tree_shader));
    add_render_object(new DrawableObject(plain_model, new Matrix(0, 0, 0, 0, 0, 20), tree_shader));


}

BasicScene::BasicScene(GLFWwindow *window) : Scene(window) {
}

void BasicScene::init() {
    auto *basic_shader = new ShaderProgram("../2d.vs", "../2d.fs");
    shader_programs.push_back(basic_shader);

    init_camera(glm::vec3(0.0, 5.0, 0.0), 30);

    constexpr float triangle_data[] = {
        0.0f, 0.5f, 0.0f, 0.f, 1.f, 0.f,
        0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
    };
    auto *triangle = new Model(triangle_data, sizeof(triangle_data));
    add_render_object(new DrawableObject(triangle, {new Matrix()}, basic_shader));
}

PhongScene::PhongScene(GLFWwindow *window) : Scene(window) {
}

void PhongScene::init() {
    auto *phong_shader = new ShaderProgram("../phong.vs", "../phong.fs");
    shader_programs.push_back(phong_shader);
    init_camera(glm::vec3(0.0, 0.0, 4.0), 0);

    std::vector<Matrix *> matrices;
    matrices.push_back(new Matrix(0, 0, 2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, -2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, -2, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, 2, 0, 1));

    auto *sphere_model = new Model(sphere, sizeof(sphere));
    add_render_object(new DrawableObject(sphere_model, matrices, phong_shader));
}

AllLightsScene::AllLightsScene(GLFWwindow *window) : Scene(window) {
}

void AllLightsScene::init() {
    shader_programs.push_back(new ShaderProgram("../constant_light.vs", "../constant_light.fs"));
    shader_programs.push_back(new ShaderProgram("../lambert.vs", "../lambert.fs"));
    shader_programs.push_back(new ShaderProgram("../phong.vs", "../phong.fs"));
    shader_programs.push_back(new ShaderProgram("../blinn.vs", "../blinn.fs"));

    init_camera(glm::vec3(0.0, 0.0, 4.0), 0);

    std::vector<Matrix *> matrices;
    matrices.push_back(new Matrix(0, 0, 2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, -2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, -2, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, 2, 0, 1));

    auto *sphere_model = new Model(sphere, sizeof(sphere));
    add_render_object(new DrawableObject(sphere_model, matrices[0], shader_programs[0]));
    add_render_object(new DrawableObject(sphere_model, matrices[1], shader_programs[1]));
    add_render_object(new DrawableObject(sphere_model, matrices[2], shader_programs[2]));
    add_render_object(new DrawableObject(sphere_model, matrices[3], shader_programs[3]));

}
