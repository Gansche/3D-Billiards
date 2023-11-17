//
// Created by Brave Fang on 2023/11/15.
//

#include "shader.h"

Shader::Shader(const char *vertexSource, const char *fragmentSource, const char *geometrySource) {
    _vertexSource = vertexSource;
    _fragmentSource = fragmentSource;
    _geometrySource = geometrySource;
}


void Shader::compile() {

}