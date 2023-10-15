#pragma once

#include <GL/glew.h>
#include <vector>

class EBO {
public:
    explicit EBO(const std::vector<GLuint> &indices);

    ~EBO();

    void bind() const;

    static void unbind();

    [[nodiscard]] inline GLsizei get_indices_count() const {
        return this->indices_count;
    }

private:
    GLuint id = 0;
    GLsizei indices_count = 0;

    void destroy();
};