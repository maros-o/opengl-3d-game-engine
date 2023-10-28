#include <algorithm>

#include "Light.h"
#include "RenderObject.h"

void Light::move(const glm::vec3 &offset) {
    this->position += offset;
    if (this->render_object != nullptr) {
        this->render_object->get_transform()->set_position(this->position);
    }
    this->notify((int) LightEvent::POSITION);
}

void Light::set_render_object(RenderObject *new_render_object) {
    this->render_object = new_render_object;
    this->render_object->get_transform()->set_position(this->position);
}

void Light::set_position(const glm::vec3 &new_position) {
    this->position = new_position;
    if (this->render_object != nullptr) {
        this->render_object->get_transform()->set_position(new_position);
    }
    this->notify((int) LightEvent::POSITION);
}

void Light::set_color(const glm::vec3 &new_color) {
    this->color = new_color;
    this->notify((int) LightEvent::COLOR);
}

float normalize_strength(float value) {
    if (value < 0.0f) {
        return 0.0f;
    }
    if (value > 1.0f) {
        return 1.0f;
    }
    return value;
}

glm::vec3 Light::get_position() const {
    return this->position;
}

glm::vec3 Light::get_color() const {
    return this->color;
}

void Light::set_constant_strength(float new_constant_strength) {
    this->constant_strength = normalize_strength(new_constant_strength);
    this->notify((int) LightEvent::STRENGTH);
}

void Light::set_linear_strength(float new_linear_strength) {
    this->linear_strength = normalize_strength(new_linear_strength);
    this->notify((int) LightEvent::STRENGTH);
}

void Light::set_quadratic_strength(float new_quadratic_strength) {
    this->quadratic_strength = normalize_strength(new_quadratic_strength);
    this->notify((int) LightEvent::STRENGTH);
}

float Light::get_constant_strength() const {
    return this->constant_strength;
}

float Light::get_linear_strength() const {
    return this->linear_strength;
}

float Light::get_quadratic_strength() const {
    return this->quadratic_strength;
}
