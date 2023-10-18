#pragma once

#include <optional>

#include "Model.h"

class RenderObject {
public:
    explicit RenderObject(Model *model) : model(model) {};

    Model *get_model();

    glm::mat4 get_model_matrix();

    RenderObject *translate(glm::vec3 translation);

    RenderObject *rotate(glm::vec3 axis);

    RenderObject *scale(glm::vec3 scale);

    RenderObject *scale(float scale);

    RenderObject *set_position(glm::vec3 new_position);

    RenderObject *set_rotation(glm::vec3 new_rotation);

    RenderObject *set_scale(glm::vec3 new_scale);

    void set_parent(RenderObject *parent);

    virtual void update_parent_model_matrix() = 0;

    virtual void update_local_model_matrix() = 0;

    virtual RenderObject *attach(RenderObject *child) = 0;

    virtual RenderObject *detach(RenderObject *child) = 0;

protected:
    Model *model;

    RenderObject *parent = nullptr;

    glm::mat4 local_model_matrix = glm::mat4(1.0f);
    glm::mat4 parent_model_matrix = glm::mat4(1.0f);

    bool local_model_matrix_needs_update = true;

    glm::vec3 position{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::vec3 rotation{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::vec3 measure{glm::vec3(1.0f, 1.0f, 1.0f)};

    void clamp_rotation();

    void calculate_local_model_matrix();

    RenderObject *apply_transform(const std::function<void(RenderObject &)> &transform);
};