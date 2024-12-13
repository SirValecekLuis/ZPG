//
// Created by tobiasjanca on 12/13/24.
//

#ifndef DRAWOBJECT_H
#define DRAWOBJECT_H
#include "model.h"

class DrawObject {
public:
    Model *model;
    std::vector<Matrix *> matrices;
    ShaderProgram *shader_program;

    DrawObject(Model *model, const std::vector<Matrix *> &matrices, ShaderProgram *shader_program)
        : model(model), matrices(matrices), shader_program(shader_program) {
    }

    DrawObject(Model *model, Matrix *matrix, ShaderProgram *shader_program)
        : model(model), shader_program(shader_program) {
        matrices.push_back(matrix);
    }
};

#endif //DRAWOBJECT_H
