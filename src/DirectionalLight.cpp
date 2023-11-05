
#include "Light/DirectionalLight.h"
#include "Observer/ObserverEvents.h"

void DirectionalLight::set_direction(const glm::vec3 &new_direction) {
    this->direction = new_direction;
    this->notify((int) LightEvent::ALL);
}

void DirectionalLight::set_color(const glm::vec3 &new_color) {
    this->color = new_color;
    this->notify((int) LightEvent::ALL);
}

glm::vec3 DirectionalLight::get_color() const {
    return this->color;
}

glm::vec3 DirectionalLight::get_direction() const {
    return this->direction;
}

void DirectionalLight::set_strength(float new_strength) {
    this->strength = new_strength;
    this->notify((int) LightEvent::ALL);
}

float DirectionalLight::get_strength() const {
    return this->strength;
}
