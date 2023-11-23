// TODO: Unchecked: Not finished yet
// Created by Brave Fang on 2023/11/15.
//

#ifndef BILLIARDS_TEXTURE_H
#define BILLIARDS_TEXTURE_H

typedef struct TextureInformation {
    int width;
    int height;
    int nrChannels;
} TextureInfo;

class Texture {
public:
    explicit Texture(const char *textureSource);

    void generate();

    void bind() const;

    /* used to debug */
    TextureInfo *getTextureInfo() { return _textureInfo; }

    ~Texture();

private:
    unsigned int _texture_id;
    const char *_textureSource;
    TextureInfo *_textureInfo;
};

#endif //BILLIARDS_TEXTURE_H
