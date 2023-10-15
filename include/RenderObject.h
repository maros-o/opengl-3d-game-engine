#pragma once

#include "Transformable.h"
#include "Model.h"

class RenderObject : Transformable {
public:
    explicit RenderObject(Model *model) : model(model) {};

    Model *get_model();

    glm::mat4 get_model_matrix();

    void set_parent(RenderObject *parent);

    RenderObject *get_parent();

    Transformable *translate(glm::vec3 translation) final;

    Transformable *rotate(glm::vec3 axis) final;

    Transformable *scale(glm::vec3 scale) final;

    Transformable *scale(float scale) final;

    Transformable *set_position(glm::vec3 new_position) final;

    Transformable *set_rotation(glm::vec3 new_rotation) final;

    Transformable *set_scale(glm::vec3 new_scale) final;


private:
    Model *model;
    glm::mat4 model_matrix = glm::mat4(1.0f);
    bool model_matrix_changed = false;

    RenderObject *parent = nullptr;

    glm::vec3 position{glm::vec3(0.0f, 0.0f, 3.0f)};
    glm::vec3 rotation{glm::vec3(0.0f, 0.0f, 0.0f)};
    glm::vec3 measure{glm::vec3(1.0f, 1.0f, 1.0f)};

    void clamp_rotation();
};