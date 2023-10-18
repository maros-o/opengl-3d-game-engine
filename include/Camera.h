#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <functional>

#include "reusable_design_patterns/Observer/IObserver.h"
#include "reusable_design_patterns/Observer/IObservable.h"

const float POV = 60.0f;
const float SPEED = 0.1f;
const float SENSITIVITY = 50.0f;
const float NEAR = 0.1f;
const float FAR = 100.0f;

enum class CameraEvent {
    PROJECTION = 0,
    VIEW = 1
};

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera : IObservable {
public:
    explicit Camera(int width, int height);

    [[nodiscard]] glm::mat4 get_view_matrix() const;

    [[nodiscard]] glm::mat4 get_projection_matrix() const;

    void move(CameraMovement direction);

    void rotate(unsigned short mouse_x, unsigned short mouse_y);

    void subscribe(IObserver *observer) override;

    void unsubscribe(IObserver *observer) override;

    void window_resize(int new_width, int new_height);

private:
    int width, height;

    glm::vec3 position{glm::vec3(0.0f, 0.0f, 10.0f)};
    glm::vec3 world_up{glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec3 front{glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 up{glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec3 right{glm::vec3(1.0f, 0.0f, 0.0f)};

    float speed = SPEED;
    float sensitivity = SENSITIVITY;

    float pitch = 0.0f;
    float yaw = -90.0f;

    std::vector<IObserver *> observers;

    void notify(int event_type) override;
};