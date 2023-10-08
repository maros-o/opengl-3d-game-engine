#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

class OpenGLContext {
public:
    OpenGLContext(int width, int height, const char *title);

    ~OpenGLContext();

    bool should_close();

    void swap_buffers();

    static void print_version_info();

    static void poll_events();

    [[nodiscard]] int get_width() const;

    [[nodiscard]] int get_height() const;

private:
    GLFWwindow *window;
};
