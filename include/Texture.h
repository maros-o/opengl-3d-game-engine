#pragma once

#include <string>
#include <vector>

#include "OpenGLContext.h"

class Texture {
public:
    explicit Texture(std::string file_path, float repeat = -1.f);

    explicit Texture(const std::vector<std::string> &faces);

    ~Texture();

    void bind(unsigned int slot = 0) const;

    static void unbind();

    [[nodiscard]] inline int get_width() const {
        return this->width;
    }

    [[nodiscard]] inline int get_height() const {
        return this->height;
    }

    [[nodiscard]] float get_repeat() const;

    void destroy();

private:
    GLuint id = 0;
    int width = 0, height = 0, channels = 0;
    std::string file_path;
    bool is_cube_map = false;
    unsigned char *local_buffer = nullptr;
    float repeat = 1.f;
};