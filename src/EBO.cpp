#include "buffers/EBO.h"


EBO::EBO(const std::vector<GLuint> &indices) {
    this->indices_count = static_cast<GLsizei>(indices.size());

    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(GLfloat)), indices.data(),
                 GL_STATIC_DRAW);
}

void EBO::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::destroy() {
    glDeleteBuffers(1, &this->id);
}
