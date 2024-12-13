//
// Created by tobiasjanca on 10/2/24.
//

#include "application.h"

static void error_callback(int error, const char *description) { fputs(description, stderr); }

static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

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

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
}

void Application::start() const {
    auto *scene_manager = new SceneManager(window);

    auto *basic_scene = new BasicScene(window);
    auto *forest_scene = new ForestScene(window);
    auto *dark_forest_scene = new ForestSceneDark(window);
    auto *phong_scene = new PhongScene(window);
    auto *all_lights_scene = new AllLightsScene(window);
    auto *forest_texture_scene = new TextureForestScene(window);

    scene_manager->add_scene("basic", basic_scene);
    scene_manager->add_scene("all_lights", all_lights_scene);
    scene_manager->add_scene("phong", phong_scene);
    scene_manager->add_scene("texture_forest", forest_texture_scene);
    scene_manager->add_scene("forest", forest_scene);
    scene_manager->add_scene("dark_forest", dark_forest_scene);

    scene_manager->switch_scene("basic");
    scene_manager->run();

    delete scene_manager;

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
