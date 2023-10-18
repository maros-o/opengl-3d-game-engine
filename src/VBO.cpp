#include "buffers/VBO.h"

VBO::VBO(const std::vector<GLfloat> &vertices, GLsizei values_per_vertex) {
    this->vertex_count = static_cast<GLsizei>(vertices.size()) / values_per_vertex;

    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(vertices.size() * sizeof(GLfloat)), vertices.data(),
                 GL_STATIC_DRAW);
}

VBO::~VBO() {
    this->destroy();
}

void VBO::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::destroy() {
    glDeleteBuffers(1, &this->id);
}
