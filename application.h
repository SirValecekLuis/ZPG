//
// Created by tobiasjanca on 10/2/24.
//

#ifndef APP_H
#define APP_H

#include <cstdio>
#include <memory>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "scene_manager.h"

class Application {
public:
    void start() const;

    void init();

private:
    GLFWwindow *window = nullptr;
};


#endif //APP_H
