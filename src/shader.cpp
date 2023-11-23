//
// Created by Brave Fang on 2023/11/23.
//

#include "shader.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char *vertexSource, const char *fragmentSource, const char *geometrySource) {
    std::string vertexCode, fragmentCode, geometryCode;
    std::ifstream vertexShaderFile, fragmentShaderFile, geometryShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream vertexShaderStream, fragmentShaderStream, geometryShaderStream;
        vertexShaderFile.open(vertexSource);
        vertexShaderStream << vertexShaderFile.rdbuf();
        vertexShaderFile.close();
        vertexCode = vertexShaderStream.str();
        fragmentShaderFile.open(fragmentSource);
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        fragmentShaderFile.close();
        fragmentCode = fragmentShaderStream.str();
        if (geometrySource != nullptr) {
            geometryShaderFile.open(geometrySource);
            geometryShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = geometryShaderStream.str();
        }

    } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
    _vertexCode = vertexSource;
    _fragmentCode = fragmentSource;
    if (geometrySource != nullptr) {
        _geometryCode = geometrySource;
    }
    _shader_id = 0;
}

void Shader::compile() {
    unsigned int vertexShader, fragmentShader, geometryShader;

    /* vertex shader */
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &_vertexCode, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    /* fragment shader */
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &_fragmentCode, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");

    /* geometry shader */
    if (_geometryCode != nullptr) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &_geometryCode, NULL);
        glCompileShader(geometryShader);
        checkCompileErrors(geometryShader, "GEOMETRY");
    }

    /* program */
    _shader_id = glCreateProgram();
    glAttachShader(_shader_id, vertexShader);
    glAttachShader(_shader_id, fragmentShader);
    if (_geometryCode != nullptr) {
        glAttachShader(_shader_id, geometryShader);
    }
    glLinkProgram(_shader_id);
    checkCompileErrors(_shader_id, "PROGRAM");

    /* delete shaders */
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (_geometryCode != nullptr) {
        glDeleteShader(geometryShader);
    }
}

Shader &Shader::use() {
    glUseProgram(_shader_id);
    return *this;
}

void Shader::setMatrix4f(const char *name, const glm::mat4 &matrix, bool useShader) {
    if (useShader) use();
    glUniformMatrix4fv(glGetUniformLocation(_shader_id, name), 1,
                       GL_FALSE, glm::value_ptr(matrix));
}

 void Shader::checkCompileErrors(unsigned int object, const std::string &type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    } else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, nullptr, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                      << infoLog << "\n -- --------------------------------------------------- -- "
                      << std::endl;
        }
    }
}