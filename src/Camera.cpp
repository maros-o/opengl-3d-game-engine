
#include "Camera.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>


[[maybe_unused]] Camera::Camera(int width, int height) {
    this->projection_matrix = glm::perspective(glm::radians(90.f), (float) width / (float) height, 0.1f, 100.0f);
    this->view_matrix = glm::lookAt(
            glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

glm::mat4 Camera::get_view_matrix() {
    return this->view_matrix;
}

glm::mat4 Camera::get_projection_matrix() {
    return this->projection_matrix;
}

void Camera::subscribe(CameraObserver *observer) {
    this->observers.push_back(observer);
}

void Camera::unsubscribe(CameraObserver *observer) {
    this->observers.erase(std::remove(this->observers.begin(), this->observers.end(), observer), this->observers.end());
}

void Camera::notify() {
    for (auto &observer: this->observers) {
        observer->update();
    }
}
