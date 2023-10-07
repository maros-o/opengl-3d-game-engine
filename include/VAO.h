#pragma once

#include "VBO.h"
#include "EBO.h"
#include <iostream>

class VAO {
public:
    VAO(VBO *vbo, EBO *ebo);

    ~VAO();

    void
    link_attributes(GLuint layout, GLint num_components, GLenum type, GLsizei stride,
                    void *offset) const;

    void bind() const;

    static void unbind();

    [[nodiscard]] VBO *get_vbo() const;

    [[nodiscard]] EBO *get_ebo() const;


private:
    GLuint id = 0;

    VBO *vbo = nullptr;
    EBO *ebo = nullptr;

    void destroy();
};
