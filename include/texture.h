// TODO: Unchecked
// Created by Brave Fang on 2023/11/15.
//

#ifndef BILLIARDS_TEXTURE_H
#define BILLIARDS_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

class Texture {
public:
    Texture() {

    }

    void generate() {
        glGenTextures(1, &_texture_id);
    }

    void bind() {
        glBindTexture(GL_TEXTURE_2D, _texture_id);
    }

    ~Texture() = default;

private:
    unsigned int _texture_id;
};

#endif //BILLIARDS_TEXTURE_H
