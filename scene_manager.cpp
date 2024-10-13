#include "scene_manager.h"

SceneManager::SceneManager(GLFWwindow *window) : window(window) {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, keyCallback);
}

SceneManager::~SceneManager() {
    for (const auto &scene : scenes) {
        delete scene.second;
    }
}

void SceneManager::add_scene(const std::string &name, Scene *scene) {
    scenes[name] = scene;
    if (current_scene_name.empty()) {
        current_scene_name = name;
        scene->init();
    }
}

void SceneManager::switch_scene(const std::string &name) {
    if (const auto it = scenes.find(name); it != scenes.end()) {
        if (!current_scene_name.empty()) {
            scenes[current_scene_name]->cleanup();
        }
        current_scene_name = name;
        scenes[name]->init();
    }
}

void SceneManager::switch_to_next_scene() {
    if (!scenes.empty()) {
        auto it = scenes.find(current_scene_name);
        if (it == scenes.end() || ++it == scenes.end()) {
            it = scenes.begin();
        }
        switch_scene(it->first);
    }
}

void SceneManager::render_current_scene() {
    if (!current_scene_name.empty()) {
        scenes[current_scene_name]->render();
    }
}

void SceneManager::run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render_current_scene();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void SceneManager::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *manager = static_cast<SceneManager *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        manager->switch_to_next_scene();
    }
}