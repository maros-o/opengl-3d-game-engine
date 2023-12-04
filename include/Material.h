#pragma once

#include "glm/vec3.hpp"
#include "Texture.h"


struct Material {
    explicit Material(glm::vec3 object_color) : object_color(object_color) {};

    explicit Material(Texture *texture) : texture(texture) {};

    glm::vec3 object_color{glm::vec3(1.0f, 1.0f, 1.0f)};

    float ambient_strength = 0.5f;
    float diffuse_strength = 0.8f;
    float specular_strength = 1.0f;
    float shininess = 1.f;

    Texture *texture = nullptr;
};