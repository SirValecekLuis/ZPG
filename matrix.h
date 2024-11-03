//
// Created by tobiasjanca on 10/12/24.
//

#ifndef MATRIX_H
#define MATRIX_H

#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>


class Matrix {
public:
    explicit Matrix(float rotate_x = 0, float rotate_y = 0, float translate_x = 0, float translate_y = 0, float translate_z = 0,
           float scale = 1);

    glm::mat4 mat{};
};


#endif //MATRIX_H
