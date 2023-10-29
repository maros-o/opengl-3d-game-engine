#pragma once

#include "OpenGLContext.h"
#include <iostream>
#include <vector>

class VBO {
public:
    VBO(const std::vector<GLfloat> &vertices, GLsizei values_per_vertex);

    void bind() const;

    static void unbind();

    void destroy();

    [[nodiscard]] inline GLsizei get_vertex_count() const {
        return this->vertex_count;
    }

private:
    GLuint id = 0;
    GLsizei vertex_count = 0;
};