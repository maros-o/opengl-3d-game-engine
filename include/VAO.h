#pragma once

#include <iostream>
#include <optional>

#include "buffers/VBO.h"
#include "buffers/EBO.h"

class VAO {
public:
    explicit VAO(const VBO &vbo);

    VAO(const VBO &vbo, const EBO &ebo);

    ~VAO();

    void link_attributes(const std::vector<GLint> &component_sizes) const;

    void bind() const;

    static void unbind();

    [[nodiscard]] VBO get_vbo() const;

    [[nodiscard]] std::optional<EBO> get_ebo() const;

    [[nodiscard]] bool has_ebo() const;


private:
    GLuint id = 0;

    const VBO vbo;
    const std::optional<EBO> ebo;

    void destroy();

    void attrib_pointer(GLuint layout, GLint num_components, GLsizei stride, void *offset) const;
};
