#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>
#include "glm/vec3.hpp"
#include "Camera.h"

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

    static GLfloat get_pixel_depth(int mouse_x, int mouse_y);

    static GLuint get_pixel_stencil(int mouse_x, int mouse_y);

    static glm::vec3 get_pixel_world_position(int mouse_x, int mouse_y, Camera *camera);

private:
    GLFWwindow *window;

    static OpenGLContext instance;

    OpenGLContext() = default;

    ~OpenGLContext();
};
