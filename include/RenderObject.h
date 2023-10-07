#pragma once

#include "Model.h"

class RenderObject {
public:
    explicit RenderObject(Model *model) : model(model) {};

    Model *get_model();

    glm::mat4 &get_model_matrix();

    void translate(glm::vec3 translation) {
        this->model_matrix = glm::translate(this->model_matrix, translation);
    }

    void rotate(float angle, glm::vec3 axis) {
        angle = glm::radians(angle);
        this->model_matrix = glm::rotate(this->model_matrix, angle, axis); //
    }

    void scale(glm::vec3 scale) {
        this->model_matrix = glm::scale(this->model_matrix, scale);
    }


private:
    Model *model;
    glm::mat4 model_matrix = glm::mat4(1.0f);
};