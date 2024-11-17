#include "scene_manager.h"

#include <chrono>
#include <thread>


float last_x = 400, last_y = 300;
bool first_mouse = true;
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
    float delta_time = 0.0f;
    float last_frame = 0.0f;


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto frameStart = std::chrono::high_resolution_clock::now();
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->process_keyboard(FORWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->process_keyboard(BACKWARD, delta_time);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->process_keyboard(LEFT, delta_time);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->process_keyboard(RIGHT, delta_time);

        render_current_scene();

        glfwSwapBuffers(window);
        glfwPollEvents();

        auto frame_end = std::chrono::high_resolution_clock::now();
        auto frame_duration = std::chrono::duration_cast<std::chrono::duration<double> >(frame_end - frameStart);

        if (frame_duration < TARGET_FRAMETIME) {
            std::this_thread::sleep_for(TARGET_FRAMETIME - frame_duration);
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
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) != GLFW_PRESS) {
        first_mouse = true;
        return;
    }

    float xpos_f = static_cast<float>(xpos);
    float ypos_f = static_cast<float>(ypos);

    if (first_mouse) {
        last_x = xpos_f;
        last_y = ypos_f;
        first_mouse = false;
    }

    float xoffset = xpos_f - last_x;
    float yoffset = last_y - ypos_f;

    last_x = xpos_f;
    last_y = ypos_f;

    camera->process_mouse_movement(xoffset, yoffset);
}

