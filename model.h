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
    Model(const float* data, GLsizeiptr size, GLsizei stride = 6 * sizeof(GLfloat));
    Model(const float* vertices, GLsizeiptr vertSize,
          const float* texCoords, GLsizeiptr texSize,
          const char* texturePath, GLsizei stride = 6 * sizeof(GLfloat));
    ~Model();

    void draw() const;
    void bind_vao() const;
    void bind_texture() const;

    void set_material(float ra, float rd, float rs);
    void randomize_material();
    Material& get_material();

    bool hasTexture = false;
    const float* data;
    GLsizeiptr size;
private:
    VBO* vbo = nullptr;
    VAO* vao = nullptr;
    Material material;

    GLuint textureID = 0;
    VBO* texCoordVBO = nullptr;

    void create_arrays(GLsizei stride = 6 * sizeof(GLfloat));
    void load_texture(const char* path);
};

#endif