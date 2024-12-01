// model.h
#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include <assimp/Importer.hpp>// C++ importerinterface
#include <assimp/scene.h>// aiSceneoutputdata structure
#include <assimp/postprocess.h>// Post processingflags

#include "buffers.h"
#include "shaders.h"
#include "material.h"

struct Vertex {
    float Position[3];
    float Normal[3];
    float Texture[2];
    float Tangent[3];
};

class Model {
public:
    Model() = default;

    Model(const float *data, GLsizei size, GLsizei stride = 6 * sizeof(GLfloat));

    void create_2d_texture(const float *data, GLsizei data_size, const char *path, float uv_size);

    void create_3d_texture(const char *file_3d, const char *file_2d);

    ~Model();

    void draw() const;

    void bind_vao() const;

    void bind_texture() const;

    void set_material(float ra, float rd, float rs);

    void randomize_material();

    Material &get_material();

    bool has_texture = false;
    const float *data{};
    GLsizei size{};

private:
    bool is_set = false;

    VBO *vbo = nullptr;
    VAO *vao = nullptr;
    IBO *ibo = nullptr;
    Material material;

    GLuint textureID = 0;

    unsigned int *indices = nullptr;
    Vertex *vertices = nullptr;
    GLsizei indicies_count = 0;
};

#endif
