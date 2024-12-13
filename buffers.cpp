#include "buffers.h"

#include <cstdio>

VBO::VBO(const void *data, const GLsizei size) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::~VBO() {
    glDeleteBuffers(1, &vbo);
}

VAO::VAO(const VBO *vbo, const IBO *ibo, const GLsizei stride) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo->vbo);

    if (ibo) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->ibo);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void *) (8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void VAO::add_texture_coords(const VBO &tex_coord) const {
    glBindVertexArray(vao);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, tex_coord.vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &vao);
}

void VAO::bind_vao() const {
    glBindVertexArray(vao);
}

IBO::IBO(const unsigned int *data, const GLsizei size) {
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

IBO::~IBO() {
    glDeleteBuffers(1, &ibo);
}
