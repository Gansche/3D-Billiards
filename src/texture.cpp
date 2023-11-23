//
// Created by Brave Fang on 2023/11/23.
//

#include "texture.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

Texture::Texture(const char *textureSource) : _textureSource(textureSource) {
    _texture_id = 0;
    _textureInfo = nullptr;
}

void Texture::generate() {
    /* load image */
    _textureInfo = new TextureInfo();
    unsigned char *data = stbi_load(_textureSource, &(_textureInfo->width), &(_textureInfo->height),
                                    &(_textureInfo->nrChannels), 0);

    /* generate texture */
    glGenTextures(1, &_texture_id);
    glBindTexture(GL_TEXTURE_2D, _texture_id);
    // TODO: changeable
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _textureInfo->width, _textureInfo->height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        std::cout << "ERROR::TEXTURE::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, _texture_id);
}

Texture::~Texture() {
    delete _textureInfo;
}