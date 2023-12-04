#include "Light/SpotLight.h"
#include "Observer/ObserverEvents.h"


void SpotLight::set_direction(const glm::vec3 &new_direction) {
    this->transform->set_rotation(new_direction);
    this->notify((int) LightEvent::ALL);
}

void SpotLight::set_cut_off(float new_cut_off) {
    this->cut_off = new_cut_off;
    this->notify((int) LightEvent::ALL);
}

glm::vec3 SpotLight::get_direction() const {
    if (this->transform->get_parent() == nullptr) {
        return this->transform->get_local_rotation();
    }
    return glm::normalize(this->transform->get_parent()->get_world_position() - this->transform->get_world_position());
}

float SpotLight::get_cut_off() const {
    return this->cut_off;
}

