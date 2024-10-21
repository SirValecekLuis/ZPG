//
// Created by tobiasjanca on 10/2/24.
//

#include "application.h"

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

void Application::init()     {
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::start() const {

    auto *scene_manager = new SceneManager(window);
    auto *forest_scene = new ForestScene(window);
    auto *bushes_scene = new BushesScene(window);

    scene_manager->add_scene("forest", forest_scene);
    scene_manager->add_scene("bushes", bushes_scene);
    scene_manager->switch_scene("forest");

    scene_manager->run();

    delete scene_manager;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
