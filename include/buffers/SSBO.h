#pragma once

#include <GL/glew.h>
#include <vector>

class SSBO {
public:
    explicit SSBO(GLsizei binding_point);

    void bind() const;

    void bind_base() const;

    void allocate_data(GLsizeiptr size, const void *data, GLenum usage = GL_DYNAMIC_DRAW) const;

    static void unbind();

    void destroy();

private:
    GLuint id = 0;
    GLsizei binding_point = 0;
};