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

    void close();

    static void set_viewport(unsigned short width, unsigned short height);

    static void print_version_info();

    [[nodiscard]] int get_width() const;

    [[nodiscard]] int get_height() const;

    [[nodiscard]] GLFWwindow *get_window() const;

private:
    GLFWwindow *window;
};
