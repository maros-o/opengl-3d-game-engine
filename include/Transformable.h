#pragma once

#include <glm/glm.hpp>

class Transformable {
public:
    virtual Transformable *translate(glm::vec3 translation) = 0;

    virtual Transformable *rotate(glm::vec3 axis) = 0;

    virtual Transformable *scale(glm::vec3 scale) = 0;

    virtual Transformable *scale(float scale) = 0;

    virtual Transformable *set_position(glm::vec3 new_position) = 0;

    virtual Transformable *set_rotation(glm::vec3 new_rotation) = 0;

    virtual Transformable *set_scale(glm::vec3 new_scale) = 0;
};