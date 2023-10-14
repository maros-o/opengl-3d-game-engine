#include "VAO.h"

VAO::VAO(VBO *vbo) : vbo(vbo) {
    glGenVertexArrays(1, &this->id);
}

VAO::VAO(VBO *vbo, EBO *ebo) : vbo(vbo), ebo(ebo) {
    glGenVertexArrays(1, &this->id);
}

void
VAO::link_attributes(GLuint layout, GLint num_components, GLenum type, GLsizei stride,
                     void *offset) const {

    this->vbo->bind();
    this->bind();
    if (this->has_ebo()) {
        this->ebo->bind();
    }
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, num_components, type, GL_FALSE, stride, offset);
    VBO::unbind();
    VAO::unbind();
    if (this->has_ebo()) {
        EBO::unbind();
    }
}

VAO::~VAO() {
    this->destroy();
}

void VAO::bind() const {
    glBindVertexArray(this->id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::destroy() {
    glDeleteVertexArrays(1, &this->id);
}

const VBO *VAO::get_vbo() const {
    return this->vbo;
}

const EBO *VAO::get_ebo() const {
    return this->ebo;
}

bool VAO::has_ebo() const {
    return this->ebo != nullptr;
}

