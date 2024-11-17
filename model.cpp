#include "model.h"

Model::Model(const float *data, const GLsizeiptr size)
    : data(data), size(size) {
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

void Model::bind_vao() const {
    vao->bind_vao();
}

void Model::draw() const {
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(size)); // mode, first, count
}

void Model::set_material(const float ra, const float rd, const float rs) {
    material.set_coefficients(ra, rd, rs);
}

void Model::randomize_material() {
    material.randomize();
}

Material &Model::get_material() {
    return material;
}
