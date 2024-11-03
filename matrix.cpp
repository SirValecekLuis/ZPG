//
// Created by tobiasjanca on 10/12/24.
//

#include "matrix.h"

Matrix::Matrix(const float rotate_x, const float rotate_y, const float translate_x, const float translate_y,
               const float translate_z, const float scale) {
    mat = glm::mat4(1.0f);

    mat = glm::translate(mat, glm::vec3(translate_x, translate_y, translate_z));

    mat = glm::rotate(mat, glm::radians(rotate_x), glm::vec3(1.0f, 0.0f, 0.0f));
    mat = glm::rotate(mat, glm::radians(rotate_y), glm::vec3(0.0f, 1.0f, 0.0f));

    mat = glm::scale(mat, glm::vec3(scale));
}
