// buffers.h
#ifndef BUFFERS_H
#define BUFFERS_H

#include <GL/glew.h>

class VBO {
public:
    VBO(const void *data, GLsizei size);

    ~VBO();

    GLuint vbo = 0;
};


class IBO {
public:
    explicit IBO(const unsigned int *data, GLsizei size);

    ~IBO();

    GLuint ibo = 0;
};

class VAO {
public:
    VAO(const VBO *vbo, const IBO *ibo, GLsizei stride = 6 * sizeof(float));

    ~VAO();

    void bind_vao() const;


    void add_texture_coords(const VBO &tex_coord) const;

private:
    GLuint vao = 0;

    GLsizei stride;
};


#endif //BUFFERS_H
