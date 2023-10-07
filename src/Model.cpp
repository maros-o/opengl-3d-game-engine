//
// Created by maros on 10/7/23.
//
#include "Model.h"

ShaderProgram *Model::get_shader() {
    return this->shader;
}

VAO *Model::get_vao() {
    return this->vao;
}

const std::string &Model::get_name() {
    return this->name;
}

