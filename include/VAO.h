#pragma once

#include "buffers/VBO.h"
#include "buffers/EBO.h"
#include <iostream>

class VAO {
public:
    explicit VAO(VBO *vbo);

    VAO(VBO *vbo, EBO *ebo);

    ~VAO();

    void
    link_attributes(GLuint layout, GLint num_components, GLenum type, GLsizei stride,
                    void *offset) const;

    void bind() const;

    static void unbind();

    [[nodiscard]] const VBO *get_vbo() const;

    [[nodiscard]] const EBO *get_ebo() const;

    [[nodiscard]] bool has_ebo() const;


private:
    GLuint id = 0;

    const VBO *vbo = nullptr;
    const EBO *ebo = nullptr;

    void destroy();
};
