//
// Created by Brave Fang on 2023/11/15.
//

#ifndef BILLIARDS_SHADER_H
#define BILLIARDS_SHADER_H

#include <string>

class Shader {
public:
    Shader(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);

    void compile();


    ~Shader() = default;

private:
    unsigned int _shader_id;
    const char *_vertexSource;
    const char *_fragmentSource;
    const char *_geometrySource;

};

#endif //BILLIARDS_SHADER_H
