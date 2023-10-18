#pragma once

#include "design_patterns/observer/IObservable.h"
#include "glm/vec3.hpp"

class Light : IObservable {
public:
    void subscribe(IObserver *observer) override;

    void unsubscribe(IObserver *observer) override;

    void notify() override;

    glm::vec3 get_position() const;

private:
    glm::vec3 position{glm::vec3(0.0f, 0.0f, 3.0f)};

    std::vector<IObserver *> observers;
};

