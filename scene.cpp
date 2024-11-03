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

auto vertex_shader_str_triag = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;

    out vec3 fragColor;

    uniform mat4 modelMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;

    void main()
    {
        vec4 pos = vec4(aPos.xy, 0.0, 1.0);

        gl_Position = pos;

        fragColor = aNormal;
    }
)glsl";

auto fragment_shader_str_triag = R"glsl(
    #version 330 core
    in vec3 fragColor;

    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(fragColor, 1.0);
    }
)glsl";


auto vertex_shader_str_phong = R"glsl(
    #version 330 core

    layout (location = 0) in vec3 in_Position;
    layout (location = 1) in vec3 in_Normal;

    uniform mat4 modelMatrix;
    uniform mat4 viewMatrix;
    uniform mat4 projectionMatrix;
    uniform mat3 normalMatrix;

    out vec4 ex_worldPosition;
    out vec3 ex_worldNormal;

    void main() {
        gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);
        ex_worldPosition = modelMatrix * vec4(in_Position, 1.0);
        ex_worldNormal = normalMatrix * in_Normal;
    }
)glsl";

auto fragment_shader_str_phong = R"glsl(
#version 330 core

in vec4 ex_worldPosition;
in vec3 ex_worldNormal;

out vec4 out_Color;

uniform vec3 viewPosition;

void main() {
    vec3 lightPosition = vec3(0.0, 0.0, 0.0);

    vec3 normal = normalize(ex_worldNormal);
    vec3 lightVector = normalize(lightPosition - ex_worldPosition.xyz);
    vec3 viewVector = normalize(viewPosition - ex_worldPosition.xyz);

    vec3 reflectVector = reflect(-lightVector, normal);

    float shininess = 64.0;
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0);

    float dot_product = max(dot(lightVector, normal), 0.0);
    float spec = pow(max(dot(viewVector, reflectVector), 0.0), shininess);

    vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);
    vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);
    vec4 specular = spec * specularColor;

    out_Color = ambient + diffuse + specular;
}
)glsl";

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

Scene::Scene(GLFWwindow *window) : camera(nullptr), window(window),
                                   vertex_shader(nullptr), fragment_shader(nullptr),
                                   shader_program(nullptr) {
}

Scene::~Scene() {
    Scene::cleanup();
}

void Scene::init() {
    vertex_shader = new VertexShader(vertex_shader_str_matrix);
    fragment_shader = new FragmentShader(fragment_shader_str_matrix);
    shader_program = new ShaderProgram(*vertex_shader, *fragment_shader);
}

void Scene::init_camera(const glm::vec3 position, float pitch) {
    camera = new Camera(position, pitch);
    camera->add_observer(shader_program);
    camera->update_camera_vectors();
    camera->notify_observers();
}

void Scene::render() {
    for (const auto &[model, matrices]: model_matrices_map) {
        for (const auto &matrix: matrices) {
            shader_program->set_model_mat(*matrix);
            shader_program->set_normal_matrix();
            model->draw();
        }
    }
}

void Scene::cleanup() {
    for (auto &[model, matrices]: model_matrices_map) {
        for (const auto *matrix: matrices) {
            delete matrix;
        }

        delete model;
    }

    model_matrices_map.clear();

    delete vertex_shader;
    delete fragment_shader;
    delete shader_program;
    delete camera;

    vertex_shader = nullptr;
    fragment_shader = nullptr;
    shader_program = nullptr;
    camera = nullptr;
}

void Scene::add_model(Model *model, const std::vector<Matrix *> &model_matrices) {
    model_matrices_map[model] = model_matrices;
}

void Scene::add_model(Model *model, Matrix *matrix) {
    std::vector<Matrix *> matrices;
    matrices.push_back(matrix);
    model_matrices_map[model] = matrices;
}

ForestScene::ForestScene(GLFWwindow *window) : Scene(window) {
}

void ForestScene::init() {
    Scene::init();
    init_camera();

    auto *tree_model = new Model(tree, sizeof(tree), *shader_program);
    auto *bush_model = new Model(bushes, sizeof(bushes), *shader_program);
    auto *plain_model = new Model(plain, sizeof(plain), *shader_program);

    const auto tree_matrices = generate_grid_matrices(8, 0.4, 4);
    const auto bush_matrices = generate_grid_matrices(18, 0.7, 1.5);


    add_model(tree_model, tree_matrices);
    add_model(bush_model, bush_matrices);
    add_model(plain_model, new Matrix(0, 0, 0, 0, 0, 20));
}

BasicScene::BasicScene(GLFWwindow *window) : Scene(window) {
}


void BasicScene::init() {
    vertex_shader = new VertexShader(vertex_shader_str_triag);
    fragment_shader = new FragmentShader(fragment_shader_str_triag);
    shader_program = new ShaderProgram(*vertex_shader, *fragment_shader);

    init_camera();

    constexpr float triangle_data[] = {
        0.0f, 0.5f, 0.0f, 0.f, 1.f, 0.f,
        0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
        -0.5f, -0.5f, 0.0f, 0.f, 1.f, 0.f,
    };

    auto *triangle = new Model(triangle_data, sizeof(triangle_data), *shader_program);
    add_model(triangle, new Matrix());
}

PhongScene::PhongScene(GLFWwindow *window) : Scene(window) {
}

void PhongScene::init() {
    vertex_shader = new VertexShader(vertex_shader_str_phong);
    fragment_shader = new FragmentShader(fragment_shader_str_phong);
    shader_program = new ShaderProgram(*vertex_shader, *fragment_shader);
    init_camera(glm::vec3(0.0, 0.0, 4.0), 0);

    std::vector<Matrix *> matrices;
    matrices.reserve(4);
    matrices.push_back(new Matrix(0, 0, 2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0,-2, 0, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0,-2, 0, 1));
    matrices.push_back(new Matrix(0, 0, 0, 2, 0, 1));

    auto* sphere_model = new Model(sphere, sizeof(sphere), *shader_program);
    add_model(sphere_model, matrices);
}
