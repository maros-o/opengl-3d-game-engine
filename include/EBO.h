#pragma once

#include <GL/glew.h>

class EBO {
public:
    EBO(GLuint *indices, GLsizeiptr size);

    ~EBO();

    void bind() const;

    static void unbind();

    [[nodiscard]] GLsizei get_count() const;

private:
    GLuint id = 0;
    GLsizei count = 0;

    void destroy();
};