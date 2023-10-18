#include "RenderObject/RenderObjectLeaf.h"
#include "RenderObject/RenderObjectComposite.h"

RenderObject *RenderObjectLeaf::attach(RenderObject *child) {
    RenderObject *composite = new RenderObjectComposite(this->model);

    composite->attach(child);

    if (this->parent != nullptr) {
        this->parent->attach(composite);
        this->parent->detach(this);
    }

    composite->set_position(this->position);
    composite->set_rotation(this->rotation);
    composite->set_scale(this->measure);

    return composite;
}

RenderObject *RenderObjectLeaf::detach(RenderObject *child) {
    throw std::runtime_error("RenderObjectLeaf::detach: cannot detach from leaf");
}

void RenderObjectLeaf::update_parent_model_matrix() {
    this->parent_model_matrix = this->parent->get_model_matrix();
}

void RenderObjectLeaf::update_local_model_matrix() {
    this->calculate_local_model_matrix();
}

