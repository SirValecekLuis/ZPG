//
// Created by tobiasjanca on 10/3/24.
//

#include "buffers.h"

VBO::VBO(const void *data, const GLsizeiptr size) {
    glGenBuffers(1, &vbo); // generate the VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

VAO::VAO(const VBO &vbo) {
    glGenVertexArrays(1, &vao); //generate the VAO
    glBindVertexArray(vao); //bind the VAO
    glEnableVertexAttribArray(0); //enable vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo.vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}


void VAO::bind_vao() const {
    glBindVertexArray(vao);
}
