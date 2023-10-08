#pragma once

#include "ShaderProgram.h"
#include "VAO.h"
#include "Texture.h"

class Model {
public:
    Model(std::string name, VAO *vao, ShaderProgram *shader) : name(std::move(name)), vao(vao), shader(shader) {};

    Model(std::string name, VAO *vao, ShaderProgram *shader, Texture *texture) : name(std::move(name)), vao(vao),
                                                                                 shader(shader), texture(texture) {};

    inline ShaderProgram *get_shader() {
        return this->shader;
    }

    inline VAO *get_vao() {
        return this->vao;
    }

    inline const std::string &get_name() {
        return this->name;
    }

    inline Texture *get_texture() {
        return this->texture;
    }

private:
    std::string name;
    ShaderProgram *shader;
    VAO *vao;
    Texture *texture = nullptr;
};