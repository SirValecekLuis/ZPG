#include "buffers.h"

VBO::VBO(const void *data, const GLsizeiptr size, const GLsizei stride)
    : stride(stride) {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::~VBO() {
    glDeleteBuffers(1, &vbo);
}

VAO::VAO(const VBO &vbo) {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    if (vbo.stride <= 3 * sizeof(GLfloat)) {
        glEnableVertexAttribArray(0);
    } else {
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo);

    if (vbo.stride <= 3 * sizeof(GLfloat)) {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vbo.stride, nullptr);
    } else {
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vbo.stride, nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vbo.stride,
                              reinterpret_cast<GLvoid *>(3 * sizeof(float)));
    }
}

void VAO::add_texture_coords(const VBO &texCoordVBO) const {
    glBindVertexArray(vao);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO.vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &vao);
}

void VAO::bind_vao() const {
    glBindVertexArray(vao);
}
