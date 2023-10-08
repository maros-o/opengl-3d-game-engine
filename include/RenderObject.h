#pragma once

#include "Model.h"

struct Rotation {
    float angle;
    glm::vec3 axis;
};

class RenderObject {
public:
    explicit RenderObject(Model *model) : model(model) {};

    inline Model *get_model() {
        return this->model;
    }

    inline glm::mat4 &get_model_matrix() {
        return this->model_matrix;
    }

    RenderObject *translate(glm::vec3 translation);

    RenderObject *rotate_rad(float angle, glm::vec3 axis);

    RenderObject *scale(glm::vec3 scale);

    void attach_child(RenderObject *child);

    void detach_child(RenderObject *child);

    void update_model_matrix();

    void update_model_matrix(glm::mat4 root_parent_matrix, glm::mat4 rotation, glm::mat4 translation, glm::mat4 scale);

private:
    Model *model;
    glm::mat4 model_matrix = glm::mat4(1.0f);

    std::vector<glm::vec3> translations;
    std::vector<Rotation> rotations;
    std::vector<glm::vec3> scales;

    std::vector<RenderObject *> children;
};