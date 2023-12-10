//
// Created by Brave Fang on 2023/12/6.
//

#include <glm/gtc/type_ptr.hpp>
#include "camera.h"
#include "program.h"
#include "shader.h"

Program *Program::_current_program = nullptr;
std::list<Program *> Program::_active_programs;

Program::Program(const std::vector<Shader *> &shaders) : _shaders(shaders) {
    _program_id = glCreateProgram();
    for (auto &_shader: _shaders) {
        glAttachShader(_program_id, _shader->getShaderID());
    }
    glLinkProgram(_program_id);
    checkCompileErrors(_program_id);
    _active_programs.push_back(this);
}

GLuint Program::bind() {
    _current_program = this;
    glUseProgram(_program_id);
    return _program_id;
}

GLuint Program::unbind() {
    _current_program = nullptr;
    glUseProgram(0);
    return 0;
};

GLuint Program::getAttrib(const GLchar *attribName) const {
    return glGetAttribLocation(_program_id, attribName);
}

GLuint Program::getUniform(const GLchar *uniformName) const {
    return glGetUniformLocation(_program_id, uniformName);
}

void Program::updateProjectionMatrix(glm::mat4 projectionMatrix) {
    for (auto &_active_program: _active_programs) {
        _active_program->bind();
        GLuint uniProj = _active_program->getUniform("projection");
        glUniformMatrix4fv(uniProj, 1, GL_FALSE,
                           glm::value_ptr(projectionMatrix));
        _active_program->unbind();
    }
}

void Program::updateViewMatrix(glm::mat4 viewMatrix) {
    for (auto &_active_program: _active_programs) {
        _active_program->bind();
        GLuint viewProj = _active_program->getUniform("view");
        glUniformMatrix4fv(viewProj, 1, GL_FALSE,
                           glm::value_ptr(viewMatrix));
        _active_program->unbind();
    }
}

Program::~Program() {
    for (auto &_shader: _shaders) {
        glDetachShader(_program_id, _shader->getShaderID());
    }
    glDeleteProgram(_program_id);
    _active_programs.remove(this);
}

void Program::checkCompileErrors(unsigned int object) {
    int success;
    char infoLog[1024];
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(object, 1024, nullptr, infoLog);
        std::cout << "| ERROR::Shader: Link-time error: PROGRAM\n"
                  << infoLog << "\n -- --------------------------------------------------- -- "
                  << std::endl;
    }
}