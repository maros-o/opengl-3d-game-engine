#pragma once

#include "Observer/IObserver.h"
#include "Observer/IObservable.h"
#include "glm/glm.hpp"

class RenderObject;

class Light : IObservable {
public:
    Light() = default;

    void subscribe(IObserver *observer) override;

    void unsubscribe(IObserver *observer) override;

    void notify(int event) override;

    void move(const glm::vec3 &offset);

    void set_render_object(RenderObject *render_object);

    void set_position(const glm::vec3 &new_position);

    void set_color(const glm::vec3 &new_color);

    void set_ambient_strength(float value);

    void set_diffuse_strength(float value);

    void set_specular_strength(float value);

    [[nodiscard]]  glm::vec3 get_position() const;

    [[nodiscard]]  glm::vec3 get_color() const;

    [[nodiscard]]  float get_ambient_strength() const;

    [[nodiscard]]  float get_diffuse_strength() const;

    [[nodiscard]]  float get_specular_strength() const;

private:
    glm::vec3 position{glm::vec3(0.0f, 10.0f, 5.0f)};
    glm::vec3 color{glm::vec3(1.0f, 1.0f, 1.0f)};

    float ambient_strength = 0.1f;
    float diffuse_strength = 0.5f;
    float specular_strength = 1.0f;

    RenderObject *render_object = nullptr;

    std::vector<IObserver *> observers;
};

