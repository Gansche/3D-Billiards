// TODO: Unchecked
// Created by Brave Fang on 2023/11/15.
//

#ifndef BILLIARDS_SHADER_H
#define BILLIARDS_SHADER_H

#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    explicit Shader(const char *vertexSource, const char *fragmentSource, const char *geometrySource = nullptr);

    void compile();

    Shader &use();

    /* TODO: May be some uniform tool functions */
    void setMatrix4f(const char *name, const glm::mat4 &matrix, bool useShader = false);

    unsigned int getShaderID() const { return _shader_id; }

    /* TODO: May delete string */
    ~Shader() = default;

private:
    unsigned int _shader_id;
    const char *_vertexCode;
    const char *_fragmentCode;
    const char *_geometryCode;

    static void checkCompileErrors(unsigned int object, const std::string &type);
};

#endif //BILLIARDS_SHADER_H
