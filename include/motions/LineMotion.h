#pragma once

#include <cstdio>
#include "Transform/TransformComposite.h"


class LineMotion {
public:
    LineMotion(glm::vec3 start_position, glm::vec3 end_position) : start_position(start_position),
                                                                   end_position(end_position) {
        this->transform_mount->set_position(this->start_position);
    }

    void move() {
        if (this->current_distance > 1.0f || this->current_distance < 0.0f) {
            this->speed *= -1;
        }
        this->current_distance += this->speed;
        printf("Current distance: %f\n", this->current_distance);
        this->transform_mount->set_position(glm::mix(this->start_position, this->end_position, this->current_distance));
    }

    void set_speed(float new_speed) {
        this->speed = new_speed;
    }

    TransformComposite *get_transform_mount() {
        return this->transform_mount;
    }

private:
    TransformComposite *transform_mount = new TransformComposite();

    glm::vec3 start_position;
    glm::vec3 end_position;

    float speed = 0.01f;
    float current_distance = 0.0f;
};