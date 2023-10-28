#pragma once

#include <optional>
#include "ShaderProgram.h"
#include "VAO.h"
#include "Texture.h"
#include "Material.h"


class Model {
public:
    Model(std::string name, VAO *vao) : name(std::move(name)), vao(vao) {}

    inline VAO *get_vao() {
        return this->vao;
    }

    inline const std::string &get_name() {
        return this->name;
    }

private:
    std::string name;
    VAO *vao;
};