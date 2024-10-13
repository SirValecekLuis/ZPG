//
// Created by tobiasjanca on 10/2/24.
//

#include <cstdio>
#include <cstdlib>

#include "shaders.h"
#include "application.h"

#include "buffers.h"
#include "model.h"
#include "sphere.h"
#include "tree.h"

#include <vector>
#include <random>

std::vector<Matrix *> generate_tree_matrices(int tree_num = 15) {
    std::vector<Matrix *> matrices;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> xDis(-0.8f, 0.8f);
    std::uniform_real_distribution<> yDis(-0.5f, 0.5f);
    std::uniform_real_distribution<> zDis(-0.4f, 0.4f);
    std::uniform_real_distribution<> rotDis(-20.0, 20.0);
    std::uniform_real_distribution<> scaleDis(0.03, 0.07);

    for (int i = 0; i < tree_num; ++i) {
        float x = xDis(gen);
        float y = yDis(gen);
        float z = zDis(gen);
        float rotateX = rotDis(gen);
        float rotateY = rotDis(gen);
        float scale = scaleDis(gen);

        matrices.push_back(new Matrix(rotateX, rotateY, x, y, z, scale));
    }

    return matrices;
}

static void error_callback(int error, const char *description) { fputs(description, stderr); }

// float z_rotate = 1.f;
//
//
// static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
//     if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, GL_TRUE);
//     printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
//     if (action == GLFW_PRESS) {
//         z_rotate *= -1.f;
//     }
// }
//
// static void window_focus_callback(GLFWwindow *window, int focused) { printf("window_focus_callback \n"); }
//
// static void window_iconify_callback(GLFWwindow *window, int iconified) { printf("window_iconify_callback \n"); }
//
// static void window_size_callback(GLFWwindow *window, int width, int height) {
//     printf("resize %d, %d \n", width, height);
//     glViewport(0, 0, width, height);
// }
//
// static void cursor_callback(GLFWwindow *window, double x, double y) { printf("cursor_callback \n"); }
//
// static void button_callback(GLFWwindow *window, int button, int action, int mode) {
//     if (action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", button, action, mode);
// }

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

void Application::init() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(1200, 900, "ZPG", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
}

void Application::start() const {
    std::vector<Matrix *> tree_matrices = generate_tree_matrices();

    auto *vertex_shader_matrix = new VertexShader(vertex_shader_str_matrix);

    auto *fragment_shader_matrix = new FragmentShader(fragment_shader_str_matrix);

    auto *shader_program_matrix = new ShaderProgram(*vertex_shader_matrix, *fragment_shader_matrix);

    auto *tree_model = new Model(tree, sizeof(tree), *shader_program_matrix);

    while (!glfwWindowShouldClose(window)) {
        // clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto &mat: tree_matrices) {
            shader_program_matrix->change_matrix(*mat);
            tree_model->draw();
        }

        // update other events like input handling
        glfwPollEvents();
        // put the stuff we’ve been drawing onto the display
        glfwSwapBuffers(window);
    }

    for (const Matrix *mat: tree_matrices) {
        delete mat;
    }
    tree_matrices.clear();

    delete vertex_shader_matrix;
    delete fragment_shader_matrix;
    delete shader_program_matrix;
    delete tree_model;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
