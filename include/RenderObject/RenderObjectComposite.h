#pragma once

#include "RenderObject.h"

class RenderObjectComposite : public RenderObject {
public:
    explicit RenderObjectComposite(Model *model) : RenderObject(model) {};

    RenderObject *attach(RenderObject *child) override;

    RenderObject *detach(RenderObject *child) override;

    void update_parent_model_matrix() override;

    void update_local_model_matrix() override;

private:
    std::vector<RenderObject *> children;

};