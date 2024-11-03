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
    Model(const float *data, GLsizeiptr size);
    ~Model();

    void draw() const;

private:
    const float *data;
    GLsizeiptr size;
    VBO *vbo = nullptr;
    VAO *vao = nullptr;

    void create_arrays();
};


#endif //MODEL_H
