#pragma once

#include "glm/glm.hpp"
#include "Light.h"


class DirectionalLight : public Light {
public:
    DirectionalLight() = default;

    void set_direction(const glm::vec3 &new_direction);

    void set_color(const glm::vec3 &new_color);

    void set_strength(float new_strength);

    [[nodiscard]]  glm::vec3 get_color() const;

    [[nodiscard]]  glm::vec3 get_direction() const;

    [[nodiscard]]  float get_strength() const;

private:
    glm::vec3 direction{glm::vec3(1.f, -1.0f, 1.0f)};
    glm::vec3 color{glm::vec3(1.0f, 1.0f, 1.0f)};
    float strength = 0.5f;
};

