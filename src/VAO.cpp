#include "VAO.h"


VAO::VAO(const VBO &vbo) : vbo(vbo) {
    glGenVertexArrays(1, &this->id);
}

VAO::VAO(const VBO &vbo, const EBO &ebo) : vbo(vbo), ebo(ebo) {
    glGenVertexArrays(1, &this->id);
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

VBO VAO::get_vbo() const {
    return this->vbo;
}

std::optional<EBO> VAO::get_ebo() const {
    return this->ebo;
}

bool VAO::has_ebo() const {
    return this->ebo.has_value();
}

void VAO::link_attributes(const std::vector<GLint> &component_sizes) const {
    GLint vertex_size = 0;
    for (auto &component_size: component_sizes) {
        vertex_size += component_size;
    }

    GLuint layout = 0;
    int offset = 0;
    for (auto &component_size: component_sizes) {
        this->attrib_pointer(layout++, component_size, vertex_size * sizeof(GLfloat),
                             (void *) (offset * sizeof(GLfloat)));
        offset += component_size;
    }
}

void
VAO::attrib_pointer(GLuint layout, GLint num_components, GLsizei stride, void *offset) const {

    this->vbo.bind();
    this->bind();
    if (this->has_ebo()) {
        this->ebo->bind();
    }
    glEnableVertexAttribArray(layout);
    glVertexAttribPointer(layout, num_components, GL_FLOAT, GL_FALSE, stride, offset);
    VBO::unbind();
    VAO::unbind();
    if (this->has_ebo()) {
        EBO::unbind();
    }
}

