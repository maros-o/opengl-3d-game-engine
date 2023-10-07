#include "EBO.h"

EBO::EBO(GLuint *indices, GLsizeiptr size) {
    this->count = size / sizeof(GLuint);
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

EBO::~EBO() {
    this->destroy();
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

GLsizei EBO::get_count() const {
    return this->count;
}
