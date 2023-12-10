//
// Created by Brave Fang on 2023/11/23.
//

#include "shader.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

Shader::Shader(const char *filename, std::string shader_type) : _shader_type(std::move(shader_type)) {
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream shaderStream;
        shaderFile.open(filename);
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        _shader_string = shaderStream.str();

    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    if (_shader_type == "VERTEX")_shader_id = glCreateShader(GL_VERTEX_SHADER);
    else if (_shader_type == "FRAGMENT")_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    else if (_shader_type == "GEOMETRY")_shader_id = glCreateShader(GL_GEOMETRY_SHADER);
    const GLchar *shader_source = _shader_string.c_str();
    glShaderSource(_shader_id, 1, &shader_source, nullptr);
    glCompileShader(_shader_id);
    checkCompileErrors(_shader_id, _shader_type);
}

Shader::~Shader() {
    glDeleteProgram(_shader_id);
}

void Shader::checkCompileErrors(unsigned int object, const std::string &type) {
    int success;
    char infoLog[1024];
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(object, 1024, nullptr, infoLog);
        std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                  << infoLog << "\n -- --------------------------------------------------- -- "
                  << std::endl;
    }

}

//void Shader::checkCompileErrors(unsigned int object, const std::string &type) {
//    int success;
//    char infoLog[1024];
//    if (type != "PROGRAM") {
//        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
//        if (!success) {
//            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
//            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
//                      << infoLog << "\n -- --------------------------------------------------- -- "
//                      << std::endl;
//        }
//    } else {
//        glGetProgramiv(object, GL_LINK_STATUS, &success);
//        if (!success) {
//            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
//            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
//                      << infoLog << "\n -- --------------------------------------------------- -- "
//                      << std::endl;
//        }
//    }
//}

//void Shader::compile() {
//
//
//    /* _program */
//    _shader_id = glCreateProgram();
//    glAttachShader(_shader_id, vertexShader);
//    glAttachShader(_shader_id, fragmentShader);
//    if (_geometryCode != nullptr) {
//        glAttachShader(_shader_id, geometryShader);
//    }
//    glLinkProgram(_shader_id);
//    checkCompileErrors(_shader_id, "PROGRAM");
//
//}