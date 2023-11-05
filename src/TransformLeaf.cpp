#include "Transform/TransformLeaf.h"
#include "Transform/TransformComposite.h"
#include <stdexcept>

Transform *TransformLeaf::attach(Transform *child) {
    Transform *composite = new TransformComposite();

    composite->attach(child);

    if (this->parent != nullptr) {
        this->parent->attach(composite);
        this->parent->detach(this);
    }

    composite->set_position(this->local_position);
    composite->set_rotation(this->local_rotation);
    composite->set_scale(this->local_measure);

    return composite;
}

Transform *TransformLeaf::detach(Transform *child) {
    throw std::runtime_error("TransformLeaf::detach: cannot detach from leaf");
}

void TransformLeaf::update_parent_model_matrix() {
    this->parent_model_matrix = this->parent->get_model_matrix();
}

void TransformLeaf::update_local_model_matrix() {
    if (!this->local_model_matrix_needs_update) {
        return;
    }
    this->calculate_local_model_matrix();
}

