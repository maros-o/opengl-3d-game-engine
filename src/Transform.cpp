#include "Transform/Transform.h"

#include <glm/gtc/matrix_transform.hpp>

Transform *Transform::scale(float scale) {
    return this->apply_transformation([scale](Transform &render_object) {
        render_object.measure *= scale;
    });
}

Transform *Transform::rotate(glm::vec3 axis) {
    return this->apply_transformation([axis](Transform &render_object) {
        render_object.rotation += axis;
        render_object.clamp_rotation();
    });
}

Transform *Transform::translate(glm::vec3 translation) {
    return this->apply_transformation([translation](Transform &render_object) {
        render_object.position += translation;
    });
}

Transform *Transform::scale(glm::vec3 scale) {
    return this->apply_transformation([scale](Transform &render_object) {
        render_object.measure *= scale;
    });
}

Transform *Transform::set_position(glm::vec3 new_position) {
    return this->apply_transformation([new_position](Transform &render_object) {
        render_object.position = new_position;
    });
}

Transform *Transform::set_rotation(glm::vec3 new_rotation) {
    return this->apply_transformation([new_rotation](Transform &render_object) {
        render_object.rotation = new_rotation;
    });
}

Transform *Transform::set_scale(glm::vec3 new_scale) {
    this->measure = new_scale;
    return this;
}

glm::mat4 Transform::get_model_matrix() {
    this->update_local_model_matrix();
    if (this->parent == nullptr) {
        return this->local_model_matrix;
    }
    return this->parent_model_matrix * this->local_model_matrix;
}

void Transform::set_parent(Transform *new_parent) {
    this->parent = new_parent;
}

void Transform::clamp_rotation() {
    if (this->rotation.x > 360.0f) this->rotation.x -= 360.0f;
    if (this->rotation.y > 360.0f) this->rotation.y -= 360.0f;
    if (this->rotation.z > 360.0f) this->rotation.z -= 360.0f;
    if (this->rotation.x < 0.0f) this->rotation.x += 360.0f;
    if (this->rotation.y < 0.0f) this->rotation.y += 360.0f;
    if (this->rotation.z < 0.0f) this->rotation.z += 360.0f;
}

void Transform::calculate_local_model_matrix() {
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

Transform *Transform::apply_transformation(const std::function<void(Transform &)> &transformation) {
    transformation(*this);
    local_model_matrix_needs_update = true;
    return this;
}
