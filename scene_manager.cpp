#include "scene_manager.h"

#include <chrono>
#include <thread>


float lastX = 400, lastY = 300;
bool firstMouse = true;
Camera* camera;

SceneManager::SceneManager(GLFWwindow *window) : window(window) {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
}

SceneManager::~SceneManager() {
    for (const auto &scene: scenes) {
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
        camera = scenes[name]->camera;
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
    constexpr int TARGET_FPS = 60;
    const std::chrono::duration<double> TARGET_FRAMETIME(1.0 / TARGET_FPS);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto frameStart = std::chrono::high_resolution_clock::now();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->process_keyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->process_keyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->process_keyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->process_keyboard(RIGHT, deltaTime);

        render_current_scene();

        glfwSwapBuffers(window);
        glfwPollEvents();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::duration<double> >(frameEnd - frameStart);

        if (frameDuration < TARGET_FRAMETIME) {
            std::this_thread::sleep_for(TARGET_FRAMETIME - frameDuration);
        }
    }
}

void SceneManager::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    auto *manager = static_cast<SceneManager *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        manager->switch_to_next_scene();
    }
}

void SceneManager::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    float xpos_f = static_cast<float>(xpos);
    float ypos_f = static_cast<float>(ypos);

    if (firstMouse) {
        lastX = xpos_f;
        lastY = ypos_f;
        firstMouse = false;
    }

    float xoffset = xpos_f - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos_f;
    lastY = ypos_f;

    camera->process_mouse_movement(xoffset, yoffset);
}
