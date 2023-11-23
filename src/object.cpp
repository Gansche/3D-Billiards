//
// Created by Brave Fang on 2023/11/22.
//

#include "object.h"
#include "shader.h"
#include "texture.h"

/* object */
Object::Object() {
    _shader = nullptr;
    _texture = nullptr;
}

Object::~Object() {
    delete _shader;
    delete _texture;
}

/* sphere */
Sphere::Sphere(glm::vec3 center) {
    _center = center;
}

void Sphere::render() {

}

Sphere::~Sphere() = default;
