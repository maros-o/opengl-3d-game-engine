#pragma once

#include "glm/glm.hpp"
#include "Light.h"
#include "PointLight.h"

class RenderObject;

class SpotLight : public PointLight {
public:
    SpotLight() = default;

    void set_direction(const glm::vec3 &new_direction);

    void set_cut_off(float new_cut_off);

    [[nodiscard]]  glm::vec3 get_direction() const;

    [[nodiscard]]  float get_cut_off() const;

private:
    float cut_off = 1.f;
};


