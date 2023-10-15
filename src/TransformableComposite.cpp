#include "TransformableComposite.h"
#include <algorithm>


void TransformableComposite::attach_child(Transformable *child) {
    this->children.push_back(child);
}

void TransformableComposite::detach_child(Transformable *child) {
    this->children.erase(std::remove(this->children.begin(), this->children.end(), child), this->children.end());
}

Transformable *TransformableComposite::translate(glm::vec3 translation) {
    for (auto &child: this->children) {
        child->translate(translation);
    }
    return this;
}

Transformable *TransformableComposite::rotate(glm::vec3 axis) {
    for (auto &child: this->children) {
        child->rotate(axis);
    }
    return this;
}

Transformable *TransformableComposite::scale(glm::vec3 scale) {
    for (auto &child: this->children) {
        child->scale(scale);
    }
    return this;
}

Transformable *TransformableComposite::scale(float scale) {
    for (auto &child: this->children) {
        child->scale(scale);
    }
    return this;
}

Transformable *TransformableComposite::set_position(glm::vec3 new_position) {
    for (auto &child: this->children) {
        child->set_position(new_position);
    }
    return this;
}

Transformable *TransformableComposite::set_rotation(glm::vec3 new_rotation) {
    for (auto &child: this->children) {
        child->set_rotation(new_rotation);
    }
    return this;
}

Transformable *TransformableComposite::set_scale(glm::vec3 new_scale) {
    for (auto &child: this->children) {
        child->set_scale(new_scale);
    }
    return this;
}