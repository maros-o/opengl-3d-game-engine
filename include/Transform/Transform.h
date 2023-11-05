#pragma once

#include <glm/glm.hpp>
#include <functional>

class Transform {
public:
    glm::mat4 get_model_matrix();

    Transform *translate(glm::vec3 translation);

    Transform *rotate(glm::vec3 axis);

    Transform *scale(glm::vec3 scale);

    Transform *scale(float scale);

    Transform *set_position(glm::vec3 new_position);

    Transform *set_rotation(glm::vec3 new_rotation);

    Transform *set_rotation_x(float new_rotation_x);

    Transform *set_rotation_y(float new_rotation_y);

    Transform *set_scale(glm::vec3 new_scale);

    Transform *set_scale(float new_scale);

    [[nodiscard]] glm::vec3 get_local_position() const;

    [[nodiscard]] glm::vec3 get_local_rotation() const;

    [[nodiscard]] glm::vec3 get_local_scale() const;

    [[nodiscard]] glm::vec3 get_world_position();

    [[nodiscard]] glm::vec3 get_world_rotation();

    [[nodiscard]] glm::vec3 get_world_scale();

    virtual void update_parent_model_matrix() = 0;

    virtual void update_local_model_matrix() = 0;

    virtual Transform *attach(Transform *child) = 0;

    virtual Transform *detach(Transform *child) = 0;

    void set_parent(Transform *parent);

    void set_parent_matrix(glm::mat4 new_parent_model_matrix);

protected:
    Transform *parent = nullptr;

    glm::mat4 local_model_matrix = glm::mat4(1.0f);
    glm::mat4 parent_model_matrix = glm::mat4(1.0f);

    bool local_model_matrix_needs_update = true;

    glm::vec3 local_position{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::vec3 local_rotation{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::vec3 local_measure{glm::vec3(1.0f, 1.0f, 1.0f)};

    void clamp_rotation();

    void calculate_local_model_matrix();

    Transform *apply_transformation(const std::function<void(Transform &)> &transformation);
};