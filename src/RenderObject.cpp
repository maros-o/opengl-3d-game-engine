#include "RenderObject.h"
#include <algorithm>

RenderObject *RenderObject::translate(glm::vec3 translation) {
    this->translations.push_back(translation);
    return this;
}

RenderObject *RenderObject::rotate_rad(float angle, glm::vec3 axis) {
    this->rotations.push_back({angle, axis});
    return this;
}

RenderObject *RenderObject::scale(glm::vec3 scale) {
    this->scales.push_back(scale);
    return this;
}

void RenderObject::attach_child(RenderObject *child) {
    this->children.push_back(child);
}

void RenderObject::detach_child(RenderObject *child) {
    auto it = std::find(this->children.begin(), this->children.end(), child);
    if (it != this->children.end()) {
        this->children.erase(it);
    }
}

void RenderObject::update_model_matrix() {
    // @TODO rotate and scale children object around its root parent, not own center

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

    for (auto &child: this->children) {
        child->update_model_matrix(this->model_matrix, rotate_matrix, translate_matrix, scale_matrix);
    }
}

void RenderObject::update_model_matrix(glm::mat4 root_parent_matrix, glm::mat4 rotation, glm::mat4 translation,
                                       glm::mat4 scale) {
    //auto parent_position = root_parent_matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    auto matrix_position = this->model_matrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    this->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-matrix_position)) * this->model_matrix;
    //this->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(parent_position)) * this->model_matrix;
    this->model_matrix = rotation * this->model_matrix;
    //this->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(-parent_position)) * this->model_matrix;

    this->model_matrix = scale * this->model_matrix;

    this->model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(matrix_position)) * this->model_matrix;
    this->model_matrix = translation * this->model_matrix;

    for (auto &child: this->children) {
        child->update_model_matrix(this->model_matrix, rotation, translation, scale);
    }
}