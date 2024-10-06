//
// Created by tobiasjanca on 10/3/24.
//

#ifndef MODEL_H
#define MODEL_H
#include <GL/glew.h>

#include "buffers.h"
#include "shaders.h"


class Model {
public:
    Model(void *data, GLsizeiptr size, ShaderProgram* shader);

    void draw() const;

private:
    void *data;
    GLsizeiptr size;
    VBO *vbo = nullptr;
    VAO *vao = nullptr;
    ShaderProgram* shader;

    void create_arrays();
};


#endif //MODEL_H
