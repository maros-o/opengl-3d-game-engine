#pragma once

#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "OpenGLContext.h"

class InputManager {
public:
    explicit InputManager(OpenGLContext *context);

    void register_key_press_callback(int key, const std::function<void()> &callback);

    void register_key_down_callback(int key, const std::function<void()> &callback);

    void update();

    static void poll_events();

    [[nodiscard]] bool is_key_down(int key) const;

private:
    std::unordered_map<int, std::vector<std::function<void()>>> key_press_callbacks;
    std::unordered_map<int, std::vector<std::function<void()>>> key_down_callbacks;

    std::unordered_set<int> pressed_keys;
};
