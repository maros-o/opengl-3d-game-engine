#include "InputManager.h"
#include "OpenGLContext.h"

InputManager::InputManager(OpenGLContext *context) {
    auto window = context->get_window();
    glfwSetWindowUserPointer(window, this);

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *input_manager = static_cast<InputManager *>(glfwGetWindowUserPointer(window));

        if (action == GLFW_PRESS) {
            input_manager->pressed_keys.insert(key);

            for (const auto &callback: input_manager->key_press_callbacks[key]) {
                callback();
            }
        } else if (action == GLFW_RELEASE) {
            input_manager->pressed_keys.erase(key);
        }
    });
}

void InputManager::update() {
    for (const auto &[key, callbacks]: this->key_down_callbacks) {
        if (this->is_key_down(key)) {
            for (const auto &callback: callbacks) {
                callback();
            }
        }
    }
}

bool InputManager::is_key_down(int key) const {
    return this->pressed_keys.find(key) != this->pressed_keys.end();
}

void InputManager::poll_events() {
    glfwPollEvents();
}

void InputManager::register_key_down_callback(int key, const std::function<void()> &callback) {
    this->key_down_callbacks[key].push_back(callback);
}

void InputManager::register_key_press_callback(int key, const std::function<void()> &callback) {
    this->key_press_callbacks[key].push_back(callback);
}

