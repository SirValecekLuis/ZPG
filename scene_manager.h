#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <unordered_map>
#include <string>
#include "scene.h"
#include <GLFW/glfw3.h>

class SceneManager {
public:
    explicit SceneManager(GLFWwindow *window);

    ~SceneManager();

    void add_scene(const std::string &name, Scene *scene);

    void switch_scene(const std::string &name);

    void switch_to_next_scene();

    void render_current_scene();

    void run();

private:
    std::unordered_map<std::string, Scene *> scenes;
    std::string current_scene_name;
    GLFWwindow *window;

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif // SCENE_MANAGER_H
