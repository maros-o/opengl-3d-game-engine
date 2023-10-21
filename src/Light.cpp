#include <algorithm>

#include "Light.h"
#include "RenderObject/RenderObject.h"

void Light::subscribe(IObserver *observer) {
    this->observers.push_back(observer);
}

void Light::unsubscribe(IObserver *observer) {
    this->observers.erase(std::remove(this->observers.begin(), this->observers.end(), observer), this->observers.end());
}

void Light::notify(int event) {
    for (auto &observer: this->observers) {
        observer->update(event);
    }
}

void Light::move(const glm::vec3 &offset) {
    this->position += offset;
    if (this->render_object != nullptr) {
        this->render_object->set_position(this->position);
    }
    this->notify((int) LightEvent::POSITION);
}

void Light::set_render_object(RenderObject *new_render_object) {
    this->render_object = new_render_object;
    this->render_object->set_position(this->position);
}

void Light::set_position(const glm::vec3 &new_position) {
    this->position = new_position;
    if (this->render_object != nullptr) {
        this->render_object->set_position(new_position);
    }
    this->notify((int) LightEvent::POSITION);
}

void Light::set_color(const glm::vec3 &new_color) {
    this->color = new_color;
    this->notify((int) LightEvent::COLOR);
}

void Light::set_ambient_strength(float value) {
    this->ambient_strength = value;
    this->notify((int) LightEvent::STRENGTH);
}

void Light::set_diffuse_strength(float value) {
    this->diffuse_strength = value;
    this->notify((int) LightEvent::STRENGTH);
}

void Light::set_specular_strength(float value) {
    this->specular_strength = value;
    this->notify((int) LightEvent::STRENGTH);
}

glm::vec3 Light::get_position() const {
    return this->position;
}

glm::vec3 Light::get_color() const {
    return this->color;
}

float Light::get_ambient_strength() const {
    return this->ambient_strength;
}

float Light::get_diffuse_strength() const {
    return this->diffuse_strength;
}

float Light::get_specular_strength() const {
    return this->specular_strength;
}
