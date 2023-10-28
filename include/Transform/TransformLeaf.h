#pragma once

#include "Transform.h"

class TransformLeaf : public Transform {
public:
    Transform *attach(Transform *child) override;

    Transform *detach(Transform *child) override;

    void update_parent_model_matrix() override;

    void update_local_model_matrix() override;
};