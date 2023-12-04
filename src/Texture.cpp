#include <../libs/stb/stb_image.h>

#include "Texture.h"


Texture::Texture(std::string file_path, float new_repeat) : file_path(std::move(file_path)) {
    stbi_set_flip_vertically_on_load(1);
    this->local_buffer = stbi_load(this->file_path.c_str(), &this->width, &this->height, &this->channels, 4);
    if (!this->local_buffer) {
        fprintf(stderr, "ERROR::TEXTURE::stbi_load('%s')\n", this->file_path.c_str());
        exit(EXIT_FAILURE);
    }

    glGenTextures(1, &this->id);
    this->bind();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 this->local_buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (new_repeat > -1.f) {
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_S,
                        GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_T,
                        GL_REPEAT);
        this->repeat = new_repeat;
    } else {
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D,
                        GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    Texture::unbind();

    if (this->local_buffer) {
        stbi_image_free(this->local_buffer);
    }
}

Texture::Texture(const std::vector<std::string> &faces) : is_cube_map(true) {
    glGenTextures(1, &this->id);
    this->bind();

    stbi_set_flip_vertically_on_load(0);

    for (unsigned int i = 0; i < faces.size(); ++i) {
        this->local_buffer = stbi_load(faces[i].c_str(), &this->width, &this->height, &this->channels, 3);

        if (this->local_buffer) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                         this->local_buffer);

            stbi_image_free(this->local_buffer);
        } else {
            fprintf(stderr, "ERROR::TEXTURE::CUBE_MAP::stbi_load('%s')\n", faces[i].c_str());
            exit(EXIT_FAILURE);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    Texture::unbind();
}

Texture::~Texture() {
    this->destroy();
}

void Texture::bind(unsigned int slot) const {
    if (this->is_cube_map) {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
    } else {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, this->id);
    }
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::destroy() {
    glDeleteTextures(1, &this->id);
}

float Texture::get_repeat() const {
    return this->repeat;
}
