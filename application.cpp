//
// Created by tobiasjanca on 10/2/24.
//

#include "application.h"

static void error_callback(int error, const char *description) { fputs(description, stderr); }

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::start() const {
    auto *scene_manager = new SceneManager(window);
    auto *basic_scene = new BasicScene(window);
    auto *forest_scene = new ForestScene(window);

    scene_manager->add_scene("basic", basic_scene);
    scene_manager->add_scene("forest", forest_scene);

    scene_manager->switch_scene("forest");

    scene_manager->run();

    delete scene_manager;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
