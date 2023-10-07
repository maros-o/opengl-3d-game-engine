#pragma once

#include "ShaderProgram.h"
#include "VAO.h"

class Model {
public:
    Model(std::string name, VAO *vao, ShaderProgram *shader) : name(std::move(name)), vao(vao), shader(shader) {};

    ShaderProgram *get_shader();

    VAO *get_vao();

    const std::string &get_name();

private:
    std::string name;
    ShaderProgram *shader;
    VAO *vao;
};