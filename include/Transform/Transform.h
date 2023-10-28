#pragma once

#include <glm/glm.hpp>

class Transform {
    glm::mat4 get_model_matrix();

    Transform *translate(glm::vec3 translation);

    Transform *rotate(glm::vec3 axis);

    Transform *scale(glm::vec3 scale);

    Transform *scale(float scale);

    Transform *set_position(glm::vec3 new_position);

    Transform *set_rotation(glm::vec3 new_rotation);

    Transform *set_scale(glm::vec3 new_scale);

    void set_parent(Transform *parent);

    virtual void update_parent_model_matrix() = 0;

    virtual void update_local_model_matrix() = 0;

    virtual Transform *attach(Transform *child) = 0;

    virtual Transform *detach(Transform *child) = 0;
};