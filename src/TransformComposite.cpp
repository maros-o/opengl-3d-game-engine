#include <algorithm>
#include "Transform/TransformComposite.h"

Transform *TransformComposite::attach(Transform *child) {
    child->set_parent(this);
    this->children.push_back(child);
    return this;
}

Transform *TransformComposite::detach(Transform *child) {
    child->set_parent(nullptr);
    this->children.erase(std::remove(this->children.begin(), this->children.end(), child), this->children.end());
    return this;
}

void TransformComposite::update_parent_model_matrix() {
    this->parent_model_matrix = this->parent->get_model_matrix();

    for (auto &child: this->children) {
        child->update_parent_model_matrix();
    }
}

void TransformComposite::update_local_model_matrix() {
    if (!this->local_model_matrix_needs_update) {
        return;
    }
    this->calculate_local_model_matrix();
    for (auto &child: this->children) {
        child->update_parent_model_matrix();
    }
}
