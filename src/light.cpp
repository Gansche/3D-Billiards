//
// Created by Brave Fang on 2023/11/19.
//

#include "light.h"

std::vector<Light *> Light::_lights{};

Light::Light(glm::vec3 position, glm::vec3 color) : Object(position), _color(color) {
    _lights.push_back(this);
}

Light::~Light() {
    for (auto &light: _lights) {
        delete light;
    }
}