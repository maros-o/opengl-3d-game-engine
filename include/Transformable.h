#pragma once

#include "Model.h"

struct Rotation {
    float angle;
    glm::vec3 axis;
};

class Transformable {
public:
    virtual Transformable *translate(glm::vec3 translation) = 0;

    virtual Transformable *rotate(float angle, glm::vec3 axis) = 0;

    virtual Transformable *scale(glm::vec3 scale) = 0;

    virtual void update_model_matrix() = 0;
};