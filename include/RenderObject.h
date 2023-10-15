#pragma once

#include "Transformable.h"

class RenderObject : public Transformable {
public:
    explicit RenderObject(Model *model) : model(model) {};

    Model *get_model();

    const glm::mat4 &get_model_matrix();

    Transformable *translate(glm::vec3 translation) override;

    Transformable *rotate(float angle, glm::vec3 axis) override;

    Transformable *scale(glm::vec3 scale) override;

    void update_model_matrix() override;


private:
    Model *model;
    glm::mat4 model_matrix = glm::mat4(1.0f);

    std::vector<glm::vec3> translations;
    std::vector<Rotation> rotations;
    std::vector<glm::vec3> scales;//
};