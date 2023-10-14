#pragma once

#include <glm/glm.hpp>
#include "CameraObserver.h"
#include <vector>

class Camera {
public:
    [[maybe_unused]] explicit Camera(int width, int height);

    glm::mat4 get_view_matrix();

    glm::mat4 get_projection_matrix();

    void subscribe(CameraObserver *observer);

    void unsubscribe(CameraObserver *observer);

private:
    glm::mat4 projection_matrix{glm::mat4(1.0f)};
    glm::mat4 view_matrix{glm::mat4(1.0f)};

    std::vector<CameraObserver *> observers;

    void notify();
};