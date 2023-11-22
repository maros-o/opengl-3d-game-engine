#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Observer/Observer.h"
#include "Observer/Observable.h"
#include "Observer/ObserverEvents.h"
#include "Transform/TransformComposite.h"

const float FOV = 60.0f;
const float SPEED = 1.f;
const float SENSITIVITY = 50.0f;
const float NEAR = 0.1f;
const float FAR = 1000.0f;

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera : public Observable {
public:
    explicit Camera(int width, int height);

    [[nodiscard]] glm::mat4 get_view_matrix() const;

    [[nodiscard]] glm::mat4 get_projection_matrix() const;

    [[nodiscard]] glm::vec3 get_position() const;

    [[nodiscard]] glm::vec3 get_euler_angles() const;

    void set_position(glm::vec3 position);

    void set_pitch_yaw(float pitch, float yaw);

    void move(CameraMovement direction);

    void rotate(unsigned short mouse_x, unsigned short mouse_y);

    void window_resize(int new_width, int new_height);

    [[nodiscard]] TransformComposite *get_transform_mount() const;

private:
    int width, height;

    glm::vec3 position{glm::vec3(0.0f, 0.0f, 10.0f)};
    glm::vec3 world_up{glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec3 front{glm::vec3(0.0f, 0.0f, -1.0f)};
    glm::vec3 up{glm::vec3(0.0f, 1.0f, 0.0f)};
    glm::vec3 right{glm::vec3(1.0f, 0.0f, 0.0f)};

    TransformComposite *transform_mount = new TransformComposite();

    float speed = SPEED;
    float sensitivity = SENSITIVITY;

    float pitch = 0.0f;
    float yaw = -90.0f;
};