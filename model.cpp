//
// Created by tobiasjanca on 10/3/24.
//

#include "model.h"

Model::Model(void *data, const GLsizeiptr size, ShaderProgram* shader) {
    this->data = data;
    this->size = size;
    this->shader = shader;

    create_arrays();
}

void Model::create_arrays() {
    vbo = new VBO(data, size);
    vao = new VAO(*vbo);
}

void Model::draw() const {
    shader->set_shader();
    vao->bind_vao();

    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(size)); //mode,first,count
}
