#include "scene.h"
#include <iostream>
#include <random>

#include "application.h"

void processInput(GLFWwindow *window, Skybox &skybox) {
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        printf("Switched\n");
        skybox.switch_follow();
    }
}

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
    flash_light = new FlashLight();

    for (auto *shader: shader_programs) {
        camera->add_observer(shader);
    }
    camera->add_observer(flash_light);

    camera->update_camera_vectors();
    camera->notify_observers();
}

void Scene::render() {
    if (skybox != nullptr) {
        processInput(window, *skybox);
        skybox->draw(camera->get_view_matrix(), camera->get_projection_matrix());
    }

    for (const auto &drawable_object: render_objects) {
        drawable_object->shader_program->use_shader();

        if (drawable_object->model->has_texture) {
            drawable_object->model->bind_texture();
            drawable_object->shader_program->set_use_texture(true);
            drawable_object->shader_program->set_texture_unit(0);
        } else {
            drawable_object->shader_program->set_use_texture(false);
        }

        drawable_object->shader_program->bind_texture_uniforms(0);

        if (const GLenum err = glGetError(); err != GL_NO_ERROR) {
            fprintf(stderr, "Error: Failed to use shader program. E: %u and str: %s\n", err, gluErrorString(err));
        }

        for (const auto &matrix: drawable_object->matrices) {
            drawable_object->model->bind_vao();

            drawable_object->shader_program->set_model_mat(matrix->mat);
            drawable_object->shader_program->set_normal_matrix();
            drawable_object->shader_program->update_all_matrices();
            drawable_object->shader_program->apply_material(drawable_object->model->get_material());
            drawable_object->model->draw();

            if (const GLenum err = glGetError(); err != GL_NO_ERROR) {
                fprintf(stderr, "Error: Failed to draw. E: %u and str: %s\n", err, gluErrorString(err));
            }
        }

        glUseProgram(0);
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

void Scene::add_render_object(DrawObject *render_object) {
    render_objects.push_back(render_object);
}

ForestScene::ForestScene(GLFWwindow *window) : Scene(window) {
}

void ForestScene::init() {
    auto *tree_shader = new ShaderProgram("../shaders/light.vs", "../shaders/phong.fs");
    shader_programs.push_back(tree_shader);
    init_camera();

    tree_shader->add_light(LightType::POINT, glm::vec3(0.0, 3.0, 0.0), glm::vec3(1, 1, 1), 0.5);
    tree_shader->add_light(LightType::SPOTLIGHT, glm::vec3(-10.0, 15.0, 0.0), glm::vec3(1, 1, 1), 1);
    tree_shader->add_light(LightType::DIRECTIONAL, glm::vec3(10.0, 0, 0.0), glm::vec3(1, 1, 1), 0.03);
    tree_shader->add_light(LightType::POINT, glm::vec3(0.0, 3.0, -10.0), glm::vec3(1, 1, 1), 0.5);
    tree_shader->add_light(LightType::POINT, glm::vec3(0.0, 3.0, 10.0), glm::vec3(1, 1, 1), 0.5);

    auto *bush_model = new Model(bushes, sizeof(bushes));
    auto *plain_model = new Model(plain, sizeof(plain));

    const auto tree_matrices = generate_grid_matrices(8, 0.4, 4);
    const auto bush_matrices = generate_grid_matrices(18, 0.7, 1.5);

    for (int i = 0; i < 8 * 8; i++) {
        auto *tree_model = new Model(tree, sizeof(tree));
        auto *object = new DrawObject(tree_model, tree_matrices[i], tree_shader);
        object->model->randomize_material();
        add_render_object(object);
    }

    add_render_object(new DrawObject(bush_model, bush_matrices, tree_shader));
    add_render_object(new DrawObject(plain_model, new Matrix(0, 0, 0, 0, 0, 20), tree_shader));
}

ForestSceneDark::ForestSceneDark(GLFWwindow *window) : Scene(window) {
}

void ForestSceneDark::init() {
    auto *tree_shader = new ShaderProgram("../shaders/light.vs", "../shaders/phong.fs");
    shader_programs.push_back(tree_shader);
    init_camera();

    tree_shader->add_light(flash_light);

    auto *tree_model = new Model(tree, sizeof(tree));
    auto *bush_model = new Model(bushes, sizeof(bushes));
    auto *plain_model = new Model(plain, sizeof(plain));

    const auto tree_matrices = generate_grid_matrices(8, 0.4, 4);
    const auto bush_matrices = generate_grid_matrices(18, 0.7, 1.5);

    add_render_object(new DrawObject(tree_model, tree_matrices, tree_shader));
    add_render_object(new DrawObject(bush_model, bush_matrices, tree_shader));
    add_render_object(new DrawObject(plain_model, new Matrix(0, 0, 0, 0, 0, 20), tree_shader));
}

BasicScene::BasicScene(GLFWwindow *window) : Scene(window) {
}

void BasicScene::init() {
    auto *basic_shader = new ShaderProgram("../shaders/2d.vs", "../shaders/2d.fs");
    shader_programs.push_back(basic_shader);
    basic_shader->set_use_texture(true);

    init_camera(glm::vec3(0.0, 5.0, 0.0), 30);

    constexpr float triangle_data[] = {
        0.0f, 0.5f, 0.0f, 0.f, 1.f, 0.f,
        0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
    };
    auto *triangle = new Model();
    triangle->create_2d_texture(triangle_data, sizeof(triangle_data), "../models/wooden_fence.png", 5);
    add_render_object(new DrawObject(triangle, {new Matrix()}, basic_shader));
}

PhongScene::PhongScene(GLFWwindow *window) : Scene(window) {
}

void PhongScene::init() {
    auto *phong_shader = new ShaderProgram("../shaders/light.vs", "../shaders/phong.fs");
    shader_programs.push_back(phong_shader);
    init_camera(glm::vec3(0.0, 0.0, 4.0), 0);

    phong_shader->add_light(LightType::POINT, glm::vec3(0.0, 0.0, 0.0));
    phong_shader->add_light(flash_light);

    std::vector<Matrix *> matrices;
    matrices.push_back(new Matrix(0, 0, 2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, -2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, -2, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, 2, 0, 1));

    auto *sphere_model = new Model(sphere, sizeof(sphere));
    add_render_object(new DrawObject(sphere_model, matrices, phong_shader));
}

AllLightsScene::AllLightsScene(GLFWwindow *window) : Scene(window) {
}

void AllLightsScene::init() {
    shader_programs.push_back(new ShaderProgram("../shaders/constant_light.vs", "../shaders/constant_light.fs"));
    shader_programs.push_back(new ShaderProgram("../shaders/light.vs", "../shaders/lambert.fs"));
    shader_programs.push_back(new ShaderProgram("../shaders/light.vs", "../shaders/phong.fs"));
    shader_programs.push_back(new ShaderProgram("../shaders/light.vs", "../shaders/blinn.fs"));

    init_camera(glm::vec3(0.0, 0.0, 4.0), 0);

    for (auto *shader_program: shader_programs) {
        shader_program->add_light(LightType::POINT, glm::vec3(0.0, 0.0, 0.0));
    }

    std::vector<Matrix *> matrices;
    matrices.push_back(new Matrix(0, 0, 2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, -2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, -2, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, 2, 0, 1));

    auto *sphere_model = new Model(sphere, sizeof(sphere));
    add_render_object(new DrawObject(sphere_model, matrices[0], shader_programs[0]));
    add_render_object(new DrawObject(sphere_model, matrices[1], shader_programs[1]));
    add_render_object(new DrawObject(sphere_model, matrices[2], shader_programs[2]));
    add_render_object(new DrawObject(sphere_model, matrices[3], shader_programs[3]));
}

TextureForestScene::TextureForestScene(GLFWwindow *window) : Scene(window) {
}

void TextureForestScene::init() {
    auto *tree_shader = new ShaderProgram("../shaders/light.vs", "../shaders/phong.fs");
    auto *house_shader = new ShaderProgram("../shaders/3d.vs", "../shaders/3d.fs");
    house_shader->set_use_texture(true);
    tree_shader->add_light(LightType::POINT, glm::vec3(0.0, 3.0, -10.0), glm::vec3(1, 1, 1), 1.5);
    tree_shader->add_light(LightType::POINT, glm::vec3(0.0, 3.0, 10.0), glm::vec3(1, 1, 1), 1.5);
    tree_shader->add_light(LightType::DIRECTIONAL, glm::vec3(10.0, 0, 0.0), glm::vec3(1, 1, 1), 0.4);
    shader_programs.push_back(tree_shader);
    shader_programs.push_back(house_shader);
    init_camera();

    tree_shader->add_light(flash_light);

    auto *bush_model = new Model(bushes, sizeof(bushes));
    auto *plain_model = new Model();
    plain_model->create_2d_texture(plain, sizeof(plain), "../models/grass.png", 5);

    // auto *login = new Model();
    // login->create_3d_texture("../models/login.obj", nullptr);

    auto *house = new Model();
    house->create_3d_texture("../models/house.obj", "../models/house.png");

    // auto *zombie = new Model();
    // zombie->create_3d_texture("../models/zombie.obj", "../models/zombie.png");

    const auto tree_matrices = generate_grid_matrices(8, 0.4, 4);
    const auto bush_matrices = generate_grid_matrices(18, 0.7, 1.5);

    const std::vector<std::string> faces = {
        "../models/posx.jpg",
        "../models/negx.jpg",
        "../models/posy.jpg",
        "../models/negy.jpg",
        "../models/posz.jpg",
        "../models/negz.jpg"
    };

    try {
        skybox = new Skybox(faces);
    } catch (const std::runtime_error &e) {
        std::cerr << "Failed to create skybox: " << e.what() << std::endl;
    }

    for (int i = 0; i < 8 * 8; i++) {
        auto *tree_model = new Model(tree, sizeof(tree));
        auto *object = new DrawObject(tree_model, tree_matrices[i], tree_shader);
        object->model->randomize_material();
        add_render_object(object);
    }

    add_render_object(new DrawObject(bush_model, bush_matrices, tree_shader));
    add_render_object(new DrawObject(plain_model, new Matrix(0, 0, 0, 0, 0, 50), tree_shader));
    add_render_object(new DrawObject(house, new Matrix(0, 0, 20, 0, 20, 1), house_shader));
    // add_render_object(new DrawObject(login, new Matrix(0, 0, 20, 20, 20, 0.3), house_shader));
    // add_render_object(new DrawObject(zombie, new Matrix(0, 0, 30, 0, 20, 3), house_shader));
}
