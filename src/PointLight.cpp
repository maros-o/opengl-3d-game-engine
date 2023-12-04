#include "Light/PointLight.h"
#include "RenderObject.h"


void PointLight::move(const glm::vec3 &offset) {
    this->transform->translate(offset);
    this->notify((int) LightEvent::ALL);
}

void PointLight::set_position(const glm::vec3 &new_position) {
    this->transform->set_position(new_position);
    this->notify((int) LightEvent::ALL);
}

void PointLight::set_color(const glm::vec3 &new_color) {
    this->color = new_color;
    this->notify((int) LightEvent::ALL);
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

glm::vec3 PointLight::get_position() const {
    return this->transform->get_world_position();
}

glm::vec3 PointLight::get_color() const {
    return this->color;
}

void PointLight::set_constant_strength(float new_constant_strength) {
    this->constant_strength = normalize_strength(new_constant_strength);
    this->notify((int) LightEvent::ALL);
}

void PointLight::set_linear_strength(float new_linear_strength) {
    this->linear_strength = normalize_strength(new_linear_strength);
    this->notify((int) LightEvent::ALL);
}

void PointLight::set_quadratic_strength(float new_quadratic_strength) {
    this->quadratic_strength = normalize_strength(new_quadratic_strength);
    this->notify((int) LightEvent::ALL);
}

float PointLight::get_constant_strength() const {
    return this->constant_strength;
}

float PointLight::get_linear_strength() const {
    return this->linear_strength;
}

float PointLight::get_quadratic_strength() const {
    return this->quadratic_strength;
}

void PointLight::update(int event) {
    if (event != (int) CameraEvent::VIEW) {
        return;
    }

    this->notify((int) LightEvent::ALL);
}

TransformComposite *PointLight::get_transform() const {
    return this->transform;
}