#pragma once

#include "Transformable.h"

class TransformableComposite : public Transformable {
public:
    TransformableComposite() = default;

    explicit TransformableComposite(std::vector<Transformable *> children) : children(std::move(children)) {};

    Transformable *translate(glm::vec3 translation) override;

    Transformable *rotate(float angle, glm::vec3 axis) override;

    Transformable *scale(glm::vec3 scale) override;

    void attach_child(Transformable *child);

    void detach_child(Transformable *child);

    void update_model_matrix() override;

private:
    std::vector<Transformable *> children;
    std::vector<glm::vec3> translations;
    std::vector<Rotation> rotations;
    std::vector<glm::vec3> scales;
};