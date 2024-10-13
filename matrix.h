//
// Created by tobiasjanca on 10/12/24.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>


class Matrix {
public:
    Matrix(float rotate_x, float rotate_y, float translate_x, float translate_y, float translate_z, float scale);

    glm::mat4 mat{};
};


#endif //MATRIX_H
