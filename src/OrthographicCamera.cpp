#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(OpenGLContext *context) {
    float width = (float) context->get_width() / (float) context->get_height();
    this->projection_matrix = glm::ortho(-width, width, -1.0f, 1.0f,
                                         -1.0f, 1.0f);
}

glm::mat4 OrthographicCamera::get_view_projection_matrix() const {
    return this->projection_matrix * this->view_matrix;
}

void OrthographicCamera::translate(glm::vec3 translation) {
    this->view_matrix = glm::translate(this->view_matrix, translation);
}

void OrthographicCamera::rotate(float angle, glm::vec3 axis) {
    this->view_matrix = glm::rotate(this->view_matrix, angle, axis);
}
