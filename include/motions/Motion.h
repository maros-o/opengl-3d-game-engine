#pragma once

#include <cstdio>

#include "Transform/TransformComposite.h"


class Motion {
public:
    virtual void move() = 0;

    TransformComposite *get_transform_mount() {
        return this->transform_mount;
    }

    void set_speed(float new_speed) {
        this->speed = new_speed;
    }

protected:
    TransformComposite *transform_mount = new TransformComposite();
    float speed = 0.01f;
    float current_distance = 0.0f;
};