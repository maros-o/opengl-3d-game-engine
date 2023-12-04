#pragma once

#include <vector>

#include "Transform.h"


class TransformComposite : public Transform {
public:
    Transform *attach(Transform *child) override;

    Transform *detach(Transform *child) override;

    void update_parent_model_matrix() override;

    void update_local_model_matrix() override;

private:
    std::vector<Transform *> children;
};