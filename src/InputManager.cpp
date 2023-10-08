#include "InputManager.h"

InputManager::InputManager(GLFWwindow *window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *input_manager = static_cast<InputManager *>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS) {
            input_manager->pressed_keys.insert(key);
        } else if (action == GLFW_RELEASE) {
            input_manager->pressed_keys.erase(key);
        }
    });
}

void InputManager::update() {
    for (const auto &[key, callbacks]: this->key_callbacks) {
        if (this->is_key_down(key)) {
            for (const auto &callback: callbacks) {
                callback();
            }
        }
    }
}

void InputManager::register_key_callback(int key, const std::function<void()> &callback) {
    this->key_callbacks[key].push_back(callback);
}

bool InputManager::is_key_down(int key) const {
    return this->pressed_keys.find(key) != this->pressed_keys.end();
}

void InputManager::poll_events() {
    glfwPollEvents();
}

