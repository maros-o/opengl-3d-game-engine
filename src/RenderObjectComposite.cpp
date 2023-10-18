#include <algorithm>

#include "RenderObject/RenderObjectComposite.h"

RenderObject *RenderObjectComposite::attach(RenderObject *child) {
    child->set_parent(this);
    this->children.push_back(child);
    return this;
}

RenderObject *RenderObjectComposite::detach(RenderObject *child) {
    child->set_parent(nullptr);
    this->children.erase(std::remove(this->children.begin(), this->children.end(), child), this->children.end());
    return this;
}

void RenderObjectComposite::update_parent_model_matrix() {
    this->parent_model_matrix = this->parent->get_model_matrix();

    for (auto &child: this->children) {
        child->update_parent_model_matrix();
    }
}

void RenderObjectComposite::update_local_model_matrix() {
    this->calculate_local_model_matrix();
    for (auto &child: this->children) {
        child->update_parent_model_matrix();
    }
}
