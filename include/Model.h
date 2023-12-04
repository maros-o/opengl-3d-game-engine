#pragma once

#include "VAO.h"


struct Model {
public:
    Model(std::string name, VAO *vao) : name(std::move(name)), vao(vao) {}

    std::string name;
    VAO *vao;
};