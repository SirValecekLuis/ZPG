//
// Created by tobiasjanca on 10/2/24.
//

#include "shaders.h"
#include "application.h"


float z_rotate = 1.f;

static void error_callback(int error, const char *description) { fputs(description, stderr); }

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
    if (action == GLFW_PRESS) {
        z_rotate *= -1.f;
    }
}

static void window_focus_callback(GLFWwindow *window, int focused) { printf("window_focus_callback \n"); }

static void window_iconify_callback(GLFWwindow *window, int iconified) { printf("window_iconify_callback \n"); }

static void window_size_callback(GLFWwindow *window, int width, int height) {
    printf("resize %d, %d \n", width, height);
    glViewport(0, 0, width, height);
}

static void cursor_callback(GLFWwindow *window, double x, double y) { printf("cursor_callback \n"); }

static void button_callback(GLFWwindow *window, int button, int action, int mode) {
    if (action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", button, action, mode);
}


//GLM test

float points[] = {
    // Quads
    0.5f, 0.f, 0.f,
    0.f, 0.5f, 0.f,
    0.5f, 0.5f, 0.f, // Vrchol

    0.5f, 0.f, 0.f,
    0.f, 0.5f, 0.f,
    0.f, 0.f, 0.f,

    // Triangle
    -0.5f, 0.f, 0.f,
    0.f, -0.5f, 0.f,
    -0.5f, -0.5f, 0.f,
};

const char *vertex_shader_str =
        "#version 330\n"
        "layout(location=0) in vec3 vp;"
        "void main () {"
        "     gl_Position = vec4 (vp, 1.0);"
        "}";


const char *fragment_shader_str_yell =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main () {"
        "     frag_colour = vec4 (1.0, 1.0, 0.f, 1.0);"
        "}";

const char *fragment_shader_str_red =
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

    window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
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
    //vertex buffer object (VBO)
    GLuint VBO = 0;
    glGenBuffers(1, &VBO); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof (points), points, GL_STATIC_DRAW);

    //Vertex Array Object (VAO)
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO); //generate the VAO
    glBindVertexArray(VAO); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //create and compile shaders
    auto *vertex_shader = new VertexShader(vertex_shader_str);

    auto *fragment_shader_yell = new FragmentShader(fragment_shader_str_yell);
    auto *fragment_shader_red = new FragmentShader(fragment_shader_str_red);

    auto *shader_program = new ShaderProgram(*vertex_shader, *fragment_shader_yell);


    while (!glfwWindowShouldClose(window)) {
        // clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader_program->set_shader();
        glBindVertexArray(VAO);
        // draw triangles
        glDrawArrays(GL_TRIANGLES, 0, sizeof(points) / 3); //mode,first,count
        // update other events like input handling
        glfwPollEvents();
        // put the stuff we’ve been drawing onto the display
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
