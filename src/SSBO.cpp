#include "./buffers/SSBO.h"

SSBO::SSBO(GLsizei binding_point) : binding_point(binding_point) {
    glGenBuffers(1, &this->id);
}

void SSBO::bind() const {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->id);
}

void SSBO::bind_base() const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, this->binding_point, this->id);
}

void SSBO::allocate_data(GLsizeiptr size, const void *data, GLenum usage) const {
    this->bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, usage);
}

void SSBO::unbind() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void SSBO::destroy() {
    glDeleteBuffers(1, &this->id);
}
