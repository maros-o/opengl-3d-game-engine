#include "Texture.h"

#include <../libs/stb/stb_image.h>

Texture::Texture(std::string file_path) : file_path(std::move(file_path)), local_buffer(nullptr), width(0), height(0),
                                          channels(0), id(0) {

    stbi_set_flip_vertically_on_load(1);
    this->local_buffer = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->channels, 4);

    glGenTextures(1, &this->id);
    this->bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T,
                    GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 this->local_buffer);
    Texture::unbind();

    if (this->local_buffer) {
        stbi_image_free(this->local_buffer);
    }
}

Texture::~Texture() {
    this->destroy();
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy() {
    glDeleteTextures(1, &this->id);
}
