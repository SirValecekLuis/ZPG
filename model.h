#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include "buffers.h"
#include "shaders.h"
#include "material.h"

class Model {
public:
    Model(const float *data, GLsizeiptr size);
    ~Model();

    void draw() const;
    void bind_vao() const;

    void set_material(float ra, float rd, float rs);

    void randomize_material();

    [[nodiscard]] Material &get_material();

private:
    const float *data;
    GLsizeiptr size;
    VBO *vbo = nullptr;
    VAO *vao = nullptr;
    Material material;

    void create_arrays();
};

#endif // MODEL_H
