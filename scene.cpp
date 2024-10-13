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

        void main()
        {
            gl_Position = modelMatrix * vec4(aPos, 1.0);
            fragColor = aColor;
        }
)glsl";

auto fragment_shader_str_matrix = R"glsl(
        #version 330 core
        in vec3 fragColor;  // Receive the color from the vertex shader

        out vec4 FragColor;

        void main()
        {
            FragColor = vec4(fragColor, 1.0);  // Use the passed color, with full opacity
        }
)glsl";


std::vector<Matrix *> generate_random_matrices(const int tree_num = 15, float scale = 0.05) {
    std::vector<Matrix *> matrices;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> x_dis(-0.8f, 0.8f);
    std::uniform_real_distribution<> y_dis(-0.5f, 0.5f);
    std::uniform_real_distribution<> z_dis(-0.4f, 0.4f);
    std::uniform_real_distribution<> rot_dis(-20.0, 20.0);
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

ForestScene::ForestScene(GLFWwindow *window) : window(window), vertex_shader_matrix(nullptr),
                                               fragment_shader_matrix(nullptr), shader_program_matrix(nullptr),
                                               tree_model(nullptr) {
}

ForestScene::~ForestScene() {
    cleanup();
}

void ForestScene::init() {
    tree_matrices = generate_random_matrices();

    vertex_shader_matrix = new VertexShader(vertex_shader_str_matrix);
    fragment_shader_matrix = new FragmentShader(fragment_shader_str_matrix);
    shader_program_matrix = new ShaderProgram(*vertex_shader_matrix, *fragment_shader_matrix);
    tree_model = new Model(tree, sizeof(tree), *shader_program_matrix);
}

void ForestScene::render() {
    for (const auto &mat: tree_matrices) {
        shader_program_matrix->change_matrix(*mat);
        tree_model->draw();
    }
}

void ForestScene::cleanup() {
    for (const Matrix *mat: tree_matrices) {
        delete mat;
    }
    tree_matrices.clear();

    delete vertex_shader_matrix;
    delete fragment_shader_matrix;
    delete shader_program_matrix;
    delete tree_model;

    vertex_shader_matrix = nullptr;
    fragment_shader_matrix = nullptr;
    shader_program_matrix = nullptr;
    tree_model = nullptr;
}


BushesScene::BushesScene(GLFWwindow *window) : window(window), vertex_shader_matrix(nullptr),
                                               fragment_shader_matrix(nullptr), shader_program_matrix(nullptr),
                                               bush_model(nullptr) {
}

BushesScene::~BushesScene() {
    cleanup();
}

void BushesScene::init() {
    bush_matrices = generate_random_matrices(20, 0.4);

    vertex_shader_matrix = new VertexShader(vertex_shader_str_matrix);
    fragment_shader_matrix = new FragmentShader(fragment_shader_str_matrix);
    shader_program_matrix = new ShaderProgram(*vertex_shader_matrix, *fragment_shader_matrix);
    bush_model = new Model(bushes, sizeof(bushes), *shader_program_matrix);
}

void BushesScene::render() {
    for (const auto &mat: bush_matrices) {
        shader_program_matrix->change_matrix(*mat);
        bush_model->draw();
    }
}

void BushesScene::cleanup() {
    for (const Matrix *mat: bush_matrices) {
        delete mat;
    }
    bush_matrices.clear();

    delete vertex_shader_matrix;
    delete fragment_shader_matrix;
    delete shader_program_matrix;
    delete bush_model;

    vertex_shader_matrix = nullptr;
    fragment_shader_matrix = nullptr;
    shader_program_matrix = nullptr;
    bush_model = nullptr;
}
