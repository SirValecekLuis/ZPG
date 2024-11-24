// buffers.h
#ifndef BUFFERS_H
#define BUFFERS_H

#include <GL/glew.h>

class VBO {
public:
    VBO(const void* data, GLsizeiptr size, GLsizei stride = 6 * sizeof(float));
    ~VBO();

    GLuint vbo = 0;
    GLsizei stride;
};

class VAO {
public:
    explicit VAO(const VBO& vbo, bool hasTexture = false);
    ~VAO();

    void bind_vao() const;
    void add_texture_coords(const VBO& texCoordVBO) const;

private:
    GLuint vao = 0;
};

#endif //BUFFERS_H