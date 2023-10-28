#pragma once

#include <optional>
#include "ShaderProgram.h"
#include "VAO.h"
#include "Texture.h"
#include "Material.h"

class Model {
public:
    Model(std::string name, VAO *vao, ShaderProgram *shader, Material *material) : name(std::move(name)), vao(vao),
                                                                                   shader(shader),
                                                                                   material(material) {};

    inline ShaderProgram *get_shader() {
        return this->shader;
    }

    inline VAO *get_vao() {
        return this->vao;
    }

    inline const std::string &get_name() {
        return this->name;
    }

    inline Material *get_material() {
        return this->material;
    }

private:
    std::string name;
    ShaderProgram *shader;
    VAO *vao;
    Material *material;
};