#pragma once

#include "OpenGLContext.h"
#include <iostream>
#include <vector>

class VBO {
public:
    explicit VBO(const std::vector<GLfloat> &vertices);

    ~VBO();

    void bind() const;

    static void unbind();

private:
    GLuint id = 0;

    void destroy();
};