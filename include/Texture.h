#pragma once

#include <string>
#include "OpenGLContext.h"

class Texture {
public:
    Texture(const std::string &file_path);

    ~Texture();

    void bind(unsigned int slot = 0) const;

    void unbind() const;

    inline int get_width() const {
        return this->width;
    }

    inline int get_height() const {
        return this->height;
    }

private:
    GLuint id = 0;
    int width = 0, height = 0, bpp = 0;
    std::string file_path;
    GLuint local_buffer = 0;
};