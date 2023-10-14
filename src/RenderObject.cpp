#include "RenderObject.h"
#include <algorithm>

void RenderObject::update_model_matrix() {
    if (this->rotations.empty() && this->translations.empty() && this->scales.empty()) {
        return;
    }

    auto translate_matrix = glm::mat4(1.0f);
    auto rotate_matrix = glm::mat4(1.0f);
    auto scale_matrix = glm::mat4(1.0f);

    for (auto &translation: this->translations) {
        translate_matrix = glm::translate(translate_matrix, translation);
    }

    for (auto &rotation: this->rotations) {
        rotate_matrix = glm::rotate(rotate_matrix, rotation.angle, rotation.axis);
    }

    for (auto &scale: this->scales) {
        scale_matrix = glm::scale(scale_matrix, scale);
    }
    this->translations.clear();
    this->rotations.clear();
    this->scales.clear();

    auto matrix_position = this->model_matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    this->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-matrix_position)) * this->model_matrix;
    this->model_matrix = rotate_matrix * this->model_matrix;

    this->model_matrix = scale_matrix * this->model_matrix;

    this->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(matrix_position)) * this->model_matrix;
    this->model_matrix = translate_matrix * this->model_matrix;
}

Transformable *RenderObject::translate(glm::vec3 translation) {
    this->translations.push_back(translation);
    return this;
}

Transformable *RenderObject::rotate(float angle, glm::vec3 axis) {
    this->rotations.push_back({angle, axis});
    return this;
}

Transformable *RenderObject::scale(glm::vec3 scale) {
    this->scales.push_back(scale);
    return this;
}

const glm::mat4 &RenderObject::get_model_matrix() {
    return this->model_matrix;
}

Model *RenderObject::get_model() {
    return this->model;
}
