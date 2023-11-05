#include "Light/SpotLight.h"
#include <iostream>

void SpotLight::set_direction(const glm::vec3 &new_direction) {
    this->transform->set_rotation(new_direction);
    this->notify((int) LightEvent::ALL);
}

void SpotLight::set_cut_off(float new_cut_off) {
    this->cut_off = new_cut_off;
    this->notify((int) LightEvent::ALL);
}

glm::vec3 SpotLight::get_direction() const {
    if (this->camera == nullptr) {
        return this->transform->get_world_rotation();
    }
    auto camera_position = this->camera->get_position();
    auto light_position = this->transform->get_world_position();

    return glm::normalize(camera_position - light_position);
}

float SpotLight::get_cut_off() const {
    return this->cut_off;
}

