//
// Created by tobiasjanca on 10/3/24.
//

#include "model.h"

Model::Model(const float *data, const GLsizeiptr size) {
    this->data = data;
    this->size = size;

    create_arrays();
}

Model::~Model() {
    delete vbo;
    delete vao;
}


void Model::create_arrays() {
    vbo = new VBO(data, size);
    vao = new VAO(*vbo);
}

void Model::draw() const {
    vao->bind_vao();

    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(size)); //mode,first,count
}
