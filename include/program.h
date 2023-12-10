//
// Created by Brave Fang on 2023/12/6.
//

//TODO 要改的地方有1.构造函数里面初始化矩阵2.更改矩阵

#ifndef BILLIARDS_PROGRAM_H
#define BILLIARDS_PROGRAM_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <list>

class Shader;

class Program {
public:
    explicit Program(const std::vector<Shader *> &shaders);

    GLuint bind();

    GLuint unbind();

    GLuint getAttrib(const GLchar *attribName) const;

    GLuint getUniform(const GLchar *uniformName) const;

    GLuint getProgramID() const { return _program_id; }

    void setBool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(_program_id, name.c_str()), (int) value);
    }

    void setInt(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(_program_id, name.c_str()), value);
    }

    void setFloat(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(_program_id, name.c_str()), value);
    }

    void setVec2(const std::string &name, const glm::vec2 &value) const {
        glUniform2fv(glGetUniformLocation(_program_id, name.c_str()), 1, &value[0]);
    }

    void setVec2(const std::string &name, float x, float y) const {
        glUniform2f(glGetUniformLocation(_program_id, name.c_str()), x, y);
    }

    void setVec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(_program_id, name.c_str()), 1, &value[0]);
    }

    void setVec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(_program_id, name.c_str()), x, y, z);
    }

    void setVec4(const std::string &name, const glm::vec4 &value) const {
        glUniform4fv(glGetUniformLocation(_program_id, name.c_str()), 1, &value[0]);
    }

    void setVec4(const std::string &name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(_program_id, name.c_str()), x, y, z, w);
    }

    void setMat2(const std::string &name, const glm::mat2 &mat) const {
        glUniformMatrix2fv(glGetUniformLocation(_program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat3(const std::string &name, const glm::mat3 &mat) const {
        glUniformMatrix3fv(glGetUniformLocation(_program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(_program_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    static Program *getCurrentProgram() { return _current_program; }

    static void updateProjectionMatrix(glm::mat4 projectionMatrix);

    static void updateViewMatrix(glm::mat4 viewMatrix);

    ~Program();

private:
    GLuint _program_id;
    std::vector<Shader *> _shaders;

    static Program *_current_program;
    static std::list<Program *> _active_programs;

    static void checkCompileErrors(unsigned int object);
};

#endif //BILLIARDS_PROGRAM_H
