#include "RenderObject/RenderObject.h"

Model *RenderObject::get_model() {
    return this->model;
}

RenderObject *RenderObject::scale(float scale) {
    return this->apply_transform([scale](RenderObject &render_object) {
        render_object.measure *= scale;
    });
}

RenderObject *RenderObject::rotate(glm::vec3 axis) {
    return this->apply_transform([axis](RenderObject &render_object) {
        render_object.rotation += axis;
        render_object.clamp_rotation();
    });
}

RenderObject *RenderObject::translate(glm::vec3 translation) {
    return this->apply_transform([translation](RenderObject &render_object) {
        render_object.position += translation;
    });
}

RenderObject *RenderObject::scale(glm::vec3 scale) {
    return this->apply_transform([scale](RenderObject &render_object) {
        render_object.measure *= scale;
    });
}

RenderObject *RenderObject::set_position(glm::vec3 new_position) {
    return this->apply_transform([new_position](RenderObject &render_object) {
        render_object.position = new_position;
    });
}

RenderObject *RenderObject::set_rotation(glm::vec3 new_rotation) {
    return this->apply_transform([new_rotation](RenderObject &render_object) {
        render_object.rotation = new_rotation;
    });
}

RenderObject *RenderObject::set_scale(glm::vec3 new_scale) {
    this->measure = new_scale;
    return this;
}

glm::mat4 RenderObject::get_model_matrix() {
    if (this->local_model_matrix_needs_update) {
        this->update_local_model_matrix();
    }
    if (this->parent == nullptr) {
        return this->local_model_matrix;
    }
    return this->parent_model_matrix * this->local_model_matrix;
}

void RenderObject::set_parent(RenderObject *new_parent) {
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

void RenderObject::calculate_local_model_matrix() {
    this->local_model_matrix = glm::mat4(1.0f);
    this->local_model_matrix = glm::translate(this->local_model_matrix, this->position);
    this->local_model_matrix = glm::rotate(this->local_model_matrix, glm::radians(this->rotation.x),
                                           glm::vec3(1.0f, 0.0f, 0.0f));
    this->local_model_matrix = glm::rotate(this->local_model_matrix, glm::radians(this->rotation.y),
                                           glm::vec3(0.0f, 1.0f, 0.0f));
    this->local_model_matrix = glm::rotate(this->local_model_matrix, glm::radians(this->rotation.z),
                                           glm::vec3(0.0f, 0.0f, 1.0f));
    this->local_model_matrix = glm::scale(this->local_model_matrix, this->measure);

    this->local_model_matrix_needs_update = false;
}

RenderObject *RenderObject::apply_transform(const std::function<void(RenderObject &)> &transform) {
    transform(*this);
    local_model_matrix_needs_update = true;
    return this;
}
