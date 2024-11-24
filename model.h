// model.h
#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include "buffers.h"
#include "shaders.h"
#include "material.h"

class Model {
public:
    Model(const float* data, GLsizeiptr size);
    Model(const float* vertices, GLsizeiptr vertSize,
          const float* texCoords, GLsizeiptr texSize,
          const char* texturePath);
    ~Model();

    void draw() const;
    void bind_vao() const;
    void bind_texture() const;

    void set_material(float ra, float rd, float rs);
    void randomize_material();
    Material& get_material();

    bool hasTexture = false;
private:
    const float* data;
    GLsizeiptr size;
    VBO* vbo = nullptr;
    VAO* vao = nullptr;
    Material material;

    GLuint textureID = 0;
    VBO* texCoordVBO = nullptr;

    void create_arrays();
    void load_texture(const char* path);
};

#endif