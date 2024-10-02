//
// Created by tobiasjanca on 10/2/24.
//

#ifndef MAIN_H
#define MAIN_H

// Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <cstdlib>
#include <cstdio>


class Shader {
public:
    virtual ~Shader() = default;

    GLuint shader_id = 0;
};

class VertexShader final : public Shader {
public:
    explicit VertexShader(const char *shader_string);
};

class FragmentShader final : public Shader {
public:
    explicit FragmentShader(const char *shader_string);
};

class ShaderProgram {
public:
    ShaderProgram(const VertexShader &vertex_shader, const FragmentShader &fragment_shader);

    void set_shader() const;

private:
    GLuint shader_id;
};

#endif //MAIN_H
