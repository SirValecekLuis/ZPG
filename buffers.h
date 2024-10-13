//
// Created by tobiasjanca on 10/3/24.
//

#ifndef BUFFERS_H
#define BUFFERS_H

#include <GL/glew.h>

class VBO {
public:
    VBO(const void *data, GLsizeiptr size);
    ~VBO();

    GLuint vbo = 0;
};

class VAO {
public:
    explicit VAO(const VBO &vbo);
    ~VAO();

    void bind_vao() const;

private:
    GLuint vao = 0;
};

#endif //BUFFERS_H
