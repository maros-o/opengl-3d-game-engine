#pragma once

#include "OpenGLContext.h"
#include <iostream>

class VBO {
public:
    VBO(GLfloat *vertices, GLsizeiptr size);

    ~VBO();

    void bind() const;

    static void unbind();

private:
    GLuint id = 0;

    void destroy();
};