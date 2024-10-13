//
// Created by tobiasjanca on 10/3/24.
//

#include "buffers.h"

VBO::VBO(const void *data, const GLsizeiptr size) {
    glGenBuffers(1, &vbo); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VBO::~VBO() {
    glDeleteBuffers(1, &vbo);
}

VAO::VAO(const VBO &vbo) {
    glGenVertexArrays(1, &vao); //generate the VAO
    glBindVertexArray(vao); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glEnableVertexAttribArray(1); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<GLvoid *>(3 * sizeof(float)));
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &vao);
}

void VAO::bind_vao() const {
    glBindVertexArray(vao);
}
