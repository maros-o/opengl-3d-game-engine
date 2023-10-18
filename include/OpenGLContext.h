#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

class OpenGLContext {
public:
    static OpenGLContext &get_instance();

    OpenGLContext &init(int screen_width, int screen_height, const char *title);

    bool should_close();

    void swap_buffers();

    void close();

    static void set_viewport(unsigned short screen_width, unsigned short screen_height);

    static void print_version_info();

    [[nodiscard]] int get_screen_width() const;

    [[nodiscard]] int get_screen_height() const;

    [[nodiscard]] GLFWwindow *get_window() const;

private:
    GLFWwindow *window;

    static OpenGLContext instance;

    OpenGLContext() = default;

    ~OpenGLContext();
};
