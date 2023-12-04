#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "OpenGLContext.h"


class InputManager {
public:
    static InputManager &get_instance();

    InputManager &init();

    void register_key_press_callback(int key, const std::function<void()> &callback);

    void register_key_down_callback(int key, const std::function<void()> &callback);

    void register_cursor_position_callback(const std::function<void(unsigned short, unsigned short)> &callback);

    void register_window_resize_callback(const std::function<void(unsigned short, unsigned short)> &callback);

    static void set_cursor_position(unsigned short pos_x, unsigned short pos_y);

    void update();

    static void poll_events();

    [[nodiscard]] bool is_key_down(int key) const;

    [[nodiscard]] unsigned short get_mouse_x() const;

    [[nodiscard]] unsigned short get_mouse_y() const;

private:
    std::unordered_map<int, std::vector<std::function<void()>>> key_press_callbacks;
    std::unordered_map<int, std::vector<std::function<void()>>> key_down_callbacks;
    std::vector<std::function<void(unsigned short, unsigned short)>> cursor_position_callbacks;
    std::vector<std::function<void(unsigned short, unsigned short)>> window_resize_callbacks;

    unsigned short mouse_x = 0.0f;
    unsigned short mouse_y = 0.0f;

    unsigned short last_mouse_x = 0.0f;
    unsigned short last_mouse_y = 0.0f;

    int window_width = 0;
    int window_height = 0;

    InputManager() = default;

    static InputManager instance;

    std::unordered_set<int> pressed_keys;

    void notify_key_down_callbacks(int key);

    void notify_cursor_position_callbacks();
};
