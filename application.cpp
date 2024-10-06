//
// Created by tobiasjanca on 10/2/24.
//

#include "shaders.h"
#include "application.h"

#include <cstdio>
#include <cstdlib>

#include "buffers.h"
#include "model.h"


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

float quad_arr[] = {
    // Quads
    0.5f, 0.f, 0.f,
    0.f, 0.5f, 0.f,
    0.5f, 0.5f, 0.f, // Top

    0.5f, 0.f, 0.f,
    0.f, 0.5f, 0.f,
    0.f, 0.f, 0.f,
};

float triag_arr[] = {
    // Triangle
    -0.5f, 0.f, 0.f,
    0.f, -0.5f, 0.f,
    -0.5f, -0.5f, 0.f,
};


auto vertex_shader_str =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "void main () {"
        "     gl_Position = vec4 (vp, 1.0);"
        "}";


auto fragment_shader_str_yell =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main () {"
        "     frag_colour = vec4 (1.0, 1.0, 0.f, 1.0);"
        "}";

auto fragment_shader_str_red =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main () {"
        "     frag_colour = vec4 (1.0, 0.f 0.f, 1.0);"
        "}";


void Application::init() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(800, 600, "ZPG", nullptr, nullptr);
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
}

void Application::start() {
    //create and compile shaders
    auto *vertex_shader = new VertexShader(vertex_shader_str);

    auto *fragment_shader_red = new FragmentShader(fragment_shader_str_red);
    auto *fragment_shader_yell = new FragmentShader(fragment_shader_str_yell);

    auto *shader_program_red = new ShaderProgram(*vertex_shader, *fragment_shader_red);
    auto *shader_program_yell = new ShaderProgram(*vertex_shader, *fragment_shader_yell);

    auto *triag = new Model(triag_arr, sizeof(triag_arr), shader_program_red);
    auto *quad = new Model(quad_arr, sizeof(quad_arr), shader_program_yell);

    while (!glfwWindowShouldClose(window)) {
        // clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        quad->draw();
        triag->draw();

        // update other events like input handling
        glfwPollEvents();
        // put the stuff we’ve been drawing onto the display
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
