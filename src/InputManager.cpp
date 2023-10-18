#include "InputManager.h"
#include "OpenGLContext.h"

InputManager InputManager::instance;

InputManager &InputManager::init() {
    auto window = glfwGetCurrentContext();

    glfwSetWindowUserPointer(window, this);

    glfwGetWindowSize(window, &this->window_width, &this->window_height);

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

    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double pos_x, double pos_y) {
        auto *input_manager = static_cast<InputManager *>(glfwGetWindowUserPointer(window));

        if (pos_x > input_manager->window_width) pos_x = input_manager->window_width;
        else if (pos_x < 0) pos_x = 0;

        if (pos_y > input_manager->window_height) pos_y = input_manager->window_height;
        else if (pos_y < 0) pos_y = 0;

        input_manager->mouse_x = (unsigned short) pos_x;
        input_manager->mouse_y = (unsigned short) pos_y;

        if (!input_manager->first_click) {
            input_manager->first_click = true;
        }
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        auto *input_manager = static_cast<InputManager *>(glfwGetWindowUserPointer(window));

        input_manager->window_width = width;
        input_manager->window_height = height;

        for (const auto &callback: input_manager->window_resize_callbacks) {
            callback(width, height);
        }
    });

    return *this;
}

void InputManager::update() {
    for (const auto &[key, callbacks]: this->key_down_callbacks) {
        if (this->is_key_down(key)) {
            this->notify_key_down_callbacks(key);
        }
    }

    if (this->mouse_x != this->last_mouse_x || this->mouse_y != this->last_mouse_y) {
        this->notify_cursor_position_callbacks();
        this->last_mouse_x = this->mouse_x;
        this->last_mouse_y = this->mouse_y;
    }

    if (this->first_click && !this->mouse_mode) {
        glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        this->mouse_mode = true;
    }
}

bool InputManager::is_key_down(int key) const {
    return this->pressed_keys.find(key) != this->pressed_keys.end();
}

void InputManager::poll_events() {
    glfwPollEvents();
}

void InputManager::notify_cursor_position_callbacks() {
    for (const auto &callback: this->cursor_position_callbacks) {
        callback(this->mouse_x, this->mouse_y);
    }
}

void InputManager::notify_key_down_callbacks(int key) {
    for (const auto &callback: this->key_down_callbacks[key]) {
        callback();
    }
}

void InputManager::set_cursor_position(unsigned short pos_x, unsigned short pos_y) {
    glfwSetCursorPos(glfwGetCurrentContext(), pos_x, pos_y);
}

void InputManager::register_key_down_callback(int key, const std::function<void()> &callback) {
    this->key_down_callbacks[key].push_back(callback);
}

void InputManager::register_key_press_callback(int key, const std::function<void()> &callback) {
    this->key_press_callbacks[key].push_back(callback);
}

void
InputManager::register_cursor_position_callback(const std::function<void(unsigned short, unsigned short)> &callback) {
    this->cursor_position_callbacks.push_back(callback);
}

void
InputManager::register_window_resize_callback(const std::function<void(unsigned short, unsigned short)> &callback) {
    this->window_resize_callbacks.push_back(callback);
}

InputManager &InputManager::get_instance() {
    return InputManager::instance;
}
