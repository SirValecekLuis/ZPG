//
// Created by tobiasjanca on 10/2/24.
//

#include "shaders.h"

VertexShader::VertexShader(const char *shader_string){
    shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader_id, 1, &shader_string, nullptr);
    glCompileShader(shader_id);
}

FragmentShader::FragmentShader(const char *shader_string){
    shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader_id, 1, &shader_string, nullptr);
    glCompileShader(shader_id);
}

ShaderProgram::ShaderProgram(const VertexShader &vertex, const FragmentShader &fragment) {
    shader_id = glCreateProgram();
    glAttachShader(shader_id, vertex.shader_id);
    glAttachShader(shader_id, fragment.shader_id);
}

void ShaderProgram::set_shader() const {
    glUseProgram(shader_id);
}
