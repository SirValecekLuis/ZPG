#include "model.h"

Model::Model(const float *data, const GLsizeiptr size)
    : data(data), size(size) {
    create_arrays();
}

Model::~Model() {
    delete vbo;
    delete vao;
    delete texCoordVBO;

    if (hasTexture) {
        glDeleteTextures(1, &textureID);
    }
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

Model::Model(const float *vertices, const GLsizeiptr vertSize,
             const float *texCoords, const GLsizeiptr texSize,
             const char *texturePath)
    : data(vertices), size(vertSize) {

    vbo = new VBO(vertices, vertSize);
    vao = new VAO(*vbo, true);

    if (texCoords != nullptr && texSize > 0) {
        texCoordVBO = new VBO(texCoords, texSize, 2 * sizeof(float));
        vao->add_texture_coords(*texCoordVBO);
    }

    if (texturePath != nullptr) {
        load_texture(texturePath);
    }
}

void Model::load_texture(const char *path) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    // Explicitně načteme s RGBA formátem
    unsigned char *image = SOIL_load_image(path, &width, &height, &channels, SOIL_LOAD_RGBA);

    if (image == nullptr) {
        throw std::runtime_error("Failed to load texture: " + std::string(SOIL_last_result()));
    }

    // Použijeme GL_RGBA místo GL_RGB
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);

    hasTexture = true;
}

void Model::bind_texture() const {
    if (hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
}
