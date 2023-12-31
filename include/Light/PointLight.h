#pragma once

#include "glm/glm.hpp"
#include "Observer/Observer.h"
#include "Observer/Observable.h"
#include "Light.h"
#include "Transform/Transform.h"
#include "Transform/TransformComposite.h"


class PointLight : public Light, public Observer {
public:
    PointLight() = default;

    void move(const glm::vec3 &offset);

    void set_position(const glm::vec3 &new_position);

    void set_color(const glm::vec3 &new_color);

    void set_constant_strength(float new_constant_strength);

    void set_linear_strength(float new_linear_strength);

    void set_quadratic_strength(float new_quadratic_strength);

    [[nodiscard]]  glm::vec3 get_position() const;

    [[nodiscard]] TransformComposite *get_transform() const;

    [[nodiscard]]  glm::vec3 get_color() const;

    [[nodiscard]]  float get_constant_strength() const;

    [[nodiscard]]  float get_linear_strength() const;

    [[nodiscard]]  float get_quadratic_strength() const;

    void update(int event) override;


protected:
    glm::vec3 color{glm::vec3(1.0f, 1.0f, 1.0f)};

    TransformComposite *transform = new TransformComposite();

    float constant_strength = 1.0f;
    float linear_strength = 0.008f;
    float quadratic_strength = 0.0032f;
};
