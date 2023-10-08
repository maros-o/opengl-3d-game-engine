#pragma once

#include <GL/glew.h>

class EBO {
public:
    EBO(GLuint *indices, GLsizeiptr size);

    ~EBO();

    void bind() const;

    static void unbind();

    [[nodiscard]] inline GLsizei get_count() const {
        return this->count;
    }

private:
    GLuint id = 0;
    GLsizei count = 0;

    void destroy();
};