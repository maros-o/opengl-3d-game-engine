#include "TransformableComposite.h"
#include <algorithm>

Transformable *TransformableComposite::translate(glm::vec3 translation) {
    this->translations.push_back(translation);
    return this;
}

Transformable *TransformableComposite::rotate(float angle, glm::vec3 axis) {
    this->rotations.push_back({angle, axis});
    return this;
}

Transformable *TransformableComposite::scale(glm::vec3 scale) {
    this->scales.push_back(scale);
    return this;
}

void TransformableComposite::attach_child(Transformable *child) {
    this->children.push_back(child);
}

void TransformableComposite::detach_child(Transformable *child) {
    auto it = std::find(this->children.begin(), this->children.end(), child);
    if (it != this->children.end()) {
        this->children.erase(it);
    }
}

void TransformableComposite::update_model_matrix() {
    if (!this->translations.empty()) {
        auto translation_sum = glm::vec3(0.0f);
        for (auto &translation: this->translations) {
            translation_sum += translation;
        }
        this->translations.clear();

        for (auto &child: this->children) {
            child->translate(translation_sum);
        }
    }

    if (!this->rotations.empty()) {
        glm::quat rotate_sum = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        for (auto &rotation: this->rotations) {
            glm::quat rotation_quat = glm::angleAxis(rotation.angle, rotation.axis);
            rotate_sum = rotation_quat * rotate_sum;
        }
        this->rotations.clear();

        for (auto &child: this->children) {
            child->rotate(glm::angle(rotate_sum), glm::axis(rotate_sum));
        }
    }

    if (!this->scales.empty()) {
        auto scale_sum = glm::vec3(1.0f);
        for (auto &scale: this->scales) {
            scale_sum *= scale;
        }
        this->scales.clear();

        for (auto &child: this->children) {
            child->scale(scale_sum);
        }
    }

    for (auto &child: this->children) {
        child->update_model_matrix();
    }
}