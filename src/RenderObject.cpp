#include "RenderObject.h"

Model *RenderObject::get_model() {
    return this->model;
}

glm::mat4 &RenderObject::get_model_matrix() {
    return this->model_matrix;
}
