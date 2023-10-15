#include "RenderObject.h"
#include <algorithm>

Model *RenderObject::get_model() {
    return this->model;
}

Transformable *RenderObject::scale(float scale) {
    this->measure *= scale;
    this->model_matrix_changed = true;
    return this;
}

Transformable *RenderObject::rotate(glm::vec3 axis) {
    this->rotation += axis;
    this->clamp_rotation();
    this->model_matrix_changed = true;
    return this;
}

Transformable *RenderObject::translate(glm::vec3 translation) {
    this->position += translation;
    this->model_matrix_changed = true;
    return this;
}

Transformable *RenderObject::scale(glm::vec3 scale) {
    this->measure *= scale;
    this->model_matrix_changed = true;
    return this;
}

Transformable *RenderObject::set_position(glm::vec3 new_position) {
    this->position = new_position;
    this->model_matrix_changed = true;
    return this;
}

Transformable *RenderObject::set_rotation(glm::vec3 new_rotation) {
    this->rotation = new_rotation;
    this->clamp_rotation();
    this->model_matrix_changed = true;
    return this;
}

Transformable *RenderObject::set_scale(glm::vec3 new_scale) {
    this->measure = new_scale;
    this->model_matrix_changed = true;
    return this;
}

glm::mat4 RenderObject::get_model_matrix() {
    if (this->model_matrix_changed) {
        this->model_matrix = glm::mat4(1.0f);
        this->model_matrix = glm::translate(this->model_matrix, this->position);
        this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.x),
                                         glm::vec3(1.0f, 0.0f, 0.0f));
        this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.y),
                                         glm::vec3(0.0f, 1.0f, 0.0f));
        this->model_matrix = glm::rotate(this->model_matrix, glm::radians(this->rotation.z),
                                         glm::vec3(0.0f, 0.0f, 1.0f));
        this->model_matrix = glm::scale(this->model_matrix, this->measure);

        this->model_matrix_changed = false;
    }

    if (this->parent != nullptr) {
        return glm::mat4{this->parent->get_model_matrix() * this->model_matrix};
    }
    return this->model_matrix;
}

void RenderObject::set_parent(RenderObject *new_parent) {
    if (new_parent == this) {
        fprintf(stderr, "RenderObject::set_parent: new_parent == this\n");
        exit(1);
    }
    if (new_parent->get_parent() == this) {
        fprintf(stderr, "RenderObject::set_parent: new_parent->get_parent() == this\n");
        exit(1);
    }
    this->parent = new_parent;
}

void RenderObject::clamp_rotation() {
    if (this->rotation.x > 360.0f) this->rotation.x -= 360.0f;
    if (this->rotation.y > 360.0f) this->rotation.y -= 360.0f;
    if (this->rotation.z > 360.0f) this->rotation.z -= 360.0f;
    if (this->rotation.x < 0.0f) this->rotation.x += 360.0f;
    if (this->rotation.y < 0.0f) this->rotation.y += 360.0f;
    if (this->rotation.z < 0.0f) this->rotation.z += 360.0f;
}

RenderObject *RenderObject::get_parent() {
    return this->parent;
}
