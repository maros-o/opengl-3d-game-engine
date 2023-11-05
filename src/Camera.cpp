
#include "Camera.h"
#include "glm/detail/type_quat.hpp"
#include "glm/gtc/quaternion.hpp"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


Camera::Camera(int width, int height) : width(width), height(height) {}

glm::mat4 Camera::get_projection_matrix() const {
    return glm::perspective(glm::radians(FOV), (float) this->width / (float) this->height, NEAR, FAR);
}

glm::mat4 Camera::get_view_matrix() const {
    return glm::lookAt(this->position, this->position + this->front, this->world_up);
}

void Camera::move(CameraMovement direction) {
    switch (direction) {
        case CameraMovement::FORWARD:
            this->position += this->speed * this->front;
            break;
        case CameraMovement::BACKWARD:
            this->position -= this->speed * this->front;
            break;
        case CameraMovement::RIGHT:
            this->position += this->speed * this->right;
            break;
        case CameraMovement::LEFT:
            this->position -= this->speed * this->right;
            break;
        case CameraMovement::UP:
            this->position += this->speed * this->up;
            break;
        case CameraMovement::DOWN:
            this->position -= this->speed * this->up;
            break;
    }

    this->transform_mount->set_position(this->position);
    this->notify((int) CameraEvent::POSITION);
    this->notify((int) CameraEvent::VIEW);
}

void Camera::rotate(unsigned short mouse_x, unsigned short mouse_y) {
    float offset_x = (float) mouse_x - (float) this->width / 2.0f;
    float offset_y = (float) mouse_y - (float) this->height / 2.0f;

    offset_x /= this->sensitivity;
    offset_y /= this->sensitivity;

    this->yaw += offset_x;
    this->pitch -= offset_y;

    if (this->yaw > 360.0f) {
        this->yaw -= 360.0f;
    } else if (this->yaw < 0.0f) {
        this->yaw += 360.0f;
    }

    if (this->pitch > 89.0f) {
        this->pitch = 89.0f;
    } else if (this->pitch < -89.0f) {
        this->pitch = -89.0f;
    }

    glm::vec3 new_front;
    new_front.x = (float) cos(glm::radians(this->yaw)) * (float) cos(glm::radians(this->pitch));
    new_front.y = (float) sin(glm::radians(this->pitch));
    new_front.z = (float) sin(glm::radians(this->yaw)) * (float) cos(glm::radians(this->pitch));

    this->front = glm::normalize(new_front);
    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->front));

    this->transform_mount->set_rotation(-glm::degrees(this->get_euler_angles()));

    this->notify((int) CameraEvent::POSITION);
    this->notify((int) CameraEvent::VIEW);
}

void Camera::window_resize(int new_width, int new_height) {
    this->width = new_width;
    this->height = new_height;

    this->notify((int) CameraEvent::PROJECTION);
}

glm::vec3 Camera::get_position() const {
    return this->position;
}

void Camera::set_position(glm::vec3 new_position) {
    this->position = new_position;

    this->transform_mount->set_position(this->position);
    this->notify((int) CameraEvent::VIEW);
    this->notify((int) CameraEvent::POSITION);
}

void Camera::set_pitch_yaw(float new_pitch, float new_yaw) {
    this->pitch = new_pitch;
    this->yaw = new_yaw;

    this->notify((int) CameraEvent::VIEW);
}

TransformComposite *Camera::get_transform_mount() const {
    return this->transform_mount;
}

glm::vec3 Camera::get_euler_angles() const {
    auto matrix = glm::mat3(this->get_view_matrix());
    
    glm::quat quat_cast = glm::quat_cast(matrix);
    glm::vec3 euler_angles = glm::eulerAngles(quat_cast);
    return euler_angles;
}

