#include "RenderObject.h"

unsigned int RenderObject::id_counter = 0;

Model *RenderObject::get_model() {
    return this->model;
}

Transform *RenderObject::get_transform() {
    return this->transform;
}

void RenderObject::set_transform(Transform *new_transform) {
    this->transform = new_transform;
}

Material &RenderObject::get_material() {
    return this->material;
}

ShaderProgram *RenderObject::get_shader() {
    return this->shader;
}

unsigned int RenderObject::get_id() const {
    return this->id;
}

