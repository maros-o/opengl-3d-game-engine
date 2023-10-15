#pragma once

#include <vector>
#include "Transformable.h"

class TransformableComposite : public Transformable {
public:
    TransformableComposite() = default;

    explicit TransformableComposite(std::vector<Transformable *> children) : children(std::move(children)) {};

    Transformable *translate(glm::vec3 translation) final;

    Transformable *rotate(glm::vec3 axis) final;

    Transformable *scale(glm::vec3 scale) final;

    Transformable *scale(float scale) final;

    Transformable *set_position(glm::vec3 new_position) final;

    Transformable *set_rotation(glm::vec3 new_rotation) final;

    Transformable *set_scale(glm::vec3 new_scale) final;

    void attach_child(Transformable *child);

    void detach_child(Transformable *child);

private:
    std::vector<Transformable *> children;
};