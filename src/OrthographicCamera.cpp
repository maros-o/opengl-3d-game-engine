#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(int width, int height) {
    float proj_width = (float) width / (float) height;
    this->projection_matrix = glm::ortho(-proj_width, proj_width, -1.0f, 1.0f,
                                         -1.0f, 1.0f);
}

glm::mat4 OrthographicCamera::get_view_projection_matrix() const {
    return this->projection_matrix * this->view_matrix;
}
