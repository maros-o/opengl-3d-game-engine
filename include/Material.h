#pragma once

#include "glm/vec3.hpp"
#include "Texture.h"

class Material {
public:
    Material() = default;

    explicit Material(glm::vec3 object_color) : object_color(object_color) {};

    Material(glm::vec3 object_color, float ambient_strength, float diffuse_strength, float specular_strength,
             float shininess) :
            object_color(object_color), ambient_strength(ambient_strength), diffuse_strength(diffuse_strength),
            specular_strength(specular_strength), shininess(shininess) {};

    [[nodiscard]] inline glm::vec3 get_object_color() const {
        return this->object_color;
    }

    [[nodiscard]] inline float get_ambient_strength() const {
        return this->ambient_strength;
    }

    [[nodiscard]] inline float get_diffuse_strength() const {
        return this->diffuse_strength;
    }

    [[nodiscard]] inline float get_specular_strength() const {
        return this->specular_strength;
    }

    [[nodiscard]] inline float get_shininess() const {
        return this->shininess;
    }

    inline void set_object_color(glm::vec3 new_object_color) {
        this->object_color = new_object_color;
    }

    inline void set_ambient_strength(float new_ambient_strength) {
        this->ambient_strength = new_ambient_strength;
    }

    inline void set_diffuse_strength(float new_diffuse_strength) {
        this->diffuse_strength = new_diffuse_strength;
    }

    inline void set_specular_strength(float new_specular_strength) {
        this->specular_strength = new_specular_strength;
    }

    inline void set_shininess(float new_shininess) {
        this->shininess = new_shininess;
    }

    inline void set_all_strengths(float new_ambient_strength, float new_diffuse_strength, float new_specular_strength,
                                  float new_shininess) {
        this->ambient_strength = new_ambient_strength;
        this->diffuse_strength = new_diffuse_strength;
        this->specular_strength = new_specular_strength;
        this->shininess = new_shininess;
    }


private:
    glm::vec3 object_color{glm::vec3(1.0f, 1.0f, 1.0f)};

    float ambient_strength = 0.1f;
    float diffuse_strength = 0.5f;
    float specular_strength = 1.0f;
    float shininess = 32.0f;
};