#pragma once

#include <GLFW/glfw3.h>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class InputManager {
public:
    explicit InputManager(GLFWwindow *window);

    void register_key_callback(int key, const std::function<void()> &callback);

    void update();

    static void poll_events();

    [[nodiscard]] bool is_key_down(int key) const;

private:
    std::unordered_map<int, std::vector<std::function<void()>>> key_callbacks;

    std::unordered_set<int> pressed_keys;
};
