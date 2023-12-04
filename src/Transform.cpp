#include "Transform/Transform.h"
#include "glm/gtc/quaternion.hpp"


Transform *Transform::scale(float scale) {
    return this->apply_transformation([scale](Transform &render_object) {
        render_object.local_measure *= scale;
    });
}

Transform *Transform::rotate(glm::vec3 axis) {
    return this->apply_transformation([axis](Transform &render_object) {
        render_object.local_rotation += axis;
        render_object.clamp_rotation();
    });
}

Transform *Transform::translate(glm::vec3 translation) {
    return this->apply_transformation([translation](Transform &render_object) {
        render_object.local_position += translation;
    });
}

Transform *Transform::scale(glm::vec3 scale) {
    return this->apply_transformation([scale](Transform &render_object) {
        render_object.local_measure *= scale;
    });
}

Transform *Transform::set_position(glm::vec3 new_position) {
    return this->apply_transformation([new_position](Transform &render_object) {
        render_object.local_position = new_position;
    });
}

Transform *Transform::set_rotation(glm::vec3 new_rotation) {
    return this->apply_transformation([new_rotation](Transform &render_object) {
        render_object.local_rotation = new_rotation;
        render_object.clamp_rotation();
    });
}

Transform *Transform::set_scale(glm::vec3 new_scale) {
    this->local_measure = new_scale;
    return this;
}

glm::mat4 Transform::get_model_matrix() {
    this->update_local_model_matrix();
    return this->parent_model_matrix * this->local_model_matrix;
}

void Transform::set_parent(Transform *new_parent) {
    this->parent = new_parent;
}

void Transform::clamp_rotation() {
    if (this->local_rotation.x > 360.0f) this->local_rotation.x -= 360.0f;
    if (this->local_rotation.y > 360.0f) this->local_rotation.y -= 360.0f;
    if (this->local_rotation.z > 360.0f) this->local_rotation.z -= 360.0f;
    if (this->local_rotation.x < 0.0f) this->local_rotation.x += 360.0f;
    if (this->local_rotation.y < 0.0f) this->local_rotation.y += 360.0f;
    if (this->local_rotation.z < 0.0f) this->local_rotation.z += 360.0f;
}

void Transform::calculate_local_model_matrix() {
    this->local_model_matrix = glm::mat4(1.0f);
    this->local_model_matrix = glm::translate(this->local_model_matrix, this->local_position);
    this->local_model_matrix = glm::rotate(this->local_model_matrix, glm::radians(this->local_rotation.x),
                                           glm::vec3(1.0f, 0.0f, 0.0f));
    this->local_model_matrix = glm::rotate(this->local_model_matrix, glm::radians(this->local_rotation.y),
                                           glm::vec3(0.0f, 1.0f, 0.0f));
    this->local_model_matrix = glm::rotate(this->local_model_matrix, glm::radians(this->local_rotation.z),
                                           glm::vec3(0.0f, 0.0f, 1.0f));
    this->local_model_matrix = glm::scale(this->local_model_matrix, this->local_measure);

    this->local_model_matrix_needs_update = false;
}

Transform *Transform::apply_transformation(const std::function<void(Transform &)> &transformation) {
    transformation(*this);
    local_model_matrix_needs_update = true;
    return this;
}

glm::vec3 Transform::get_local_position() const {
    return this->local_position;
}

glm::vec3 Transform::get_local_rotation() const {
    return this->local_rotation;
}

glm::vec3 Transform::get_world_position() {
    auto model_matrix = this->get_model_matrix();
    return {model_matrix[3][0], model_matrix[3][1], model_matrix[3][2]};
}

Transform *Transform::get_parent() const {
    return this->parent;
}
