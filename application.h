//
// Created by tobiasjanca on 10/2/24.
//

#ifndef APP_H
#define APP_H
#include <GLFW/glfw3.h>


class Application {
public:
    void start() const;

    void init();

private:
    GLFWwindow *window = nullptr;
};


#endif //APP_H
