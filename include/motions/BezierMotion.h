#pragma once

#include <cstdio>
#include <vector>
#include <glm/gtx/norm.hpp>
#include "Transform/TransformComposite.h"

class BezierMotion {
public:
    explicit BezierMotion(const std::vector<glm::vec3> &control_points) : control_points(control_points) {
        if (control_points.size() % 4 != 0) {
            fprintf(stderr, "ERROR::BEZIER_MOTION::CONTROL_POINTS::SIZE\n");
            exit(EXIT_FAILURE);
        }

        max_distance = (float) control_points.size() / 4;

        this->transform_mount->set_position(this->control_points.front());
    }

    void move() {
        if (this->current_distance > max_distance) {
            this->speed *= -1;
            this->current_distance = max_distance;
        } else if (this->current_distance < 0.f) {
            this->speed *= -1;
            this->current_distance = 0.f;
        }
        this->current_distance += this->speed;

        auto start_index = ((int) current_distance) * 4;

        glm::vec3 position = calculate_bezier_point(
                std::fmod(current_distance, 1.0f),
                this->control_points[start_index],
                this->control_points[start_index + 1],
                this->control_points[start_index + 2],
                this->control_points[start_index + 3]
        );

        this->transform_mount->set_position(position);
    }

    void set_speed(float new_speed) {
        this->speed = new_speed;
    }

    TransformComposite *get_transform_mount() {
        return this->transform_mount;
    }

private:
    TransformComposite *transform_mount = new TransformComposite();
    std::vector<glm::vec3> control_points;

    float speed = 0.01f;
    float current_distance = 0.0f;
    float max_distance = 1.0f;

    static glm::vec3 calculate_bezier_point(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
        float u = 1.0f - t;
        float tt = t * t;
        float uu = u * u;
        float uuu = uu * u;
        float ttt = tt * t;

        glm::vec3 p = uuu * p0;
        p += 3 * uu * t * p1;
        p += 3 * u * tt * p2;
        p += ttt * p3;

        return p;
    }
};
