// TODO: Unchecked
// Created by Brave Fang on 2023/11/15.
//

#ifndef BILLIARDS_SHADER_H
#define BILLIARDS_SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
public:
    explicit Shader(const char *filename, std::string shader_type);

    GLuint getShaderID() const { return _shader_id; }

    std::string getShaderType() const { return _shader_type; }

    ~Shader();

private:
    GLuint _shader_id;
    std::string _shader_type;
    std::string _shader_string;

    static void checkCompileErrors(unsigned int object, const std::string &type);

};

#endif //BILLIARDS_SHADER_H
