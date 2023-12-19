//
// Created by Brave Fang on 2023/11/19.
//

#ifndef BILLIARDS_LIGHT_H
#define BILLIARDS_LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "object.h"


class Light : public Object {
public:
    explicit Light(glm::vec3 position, glm::vec3 color);

    glm::vec3 getColor() { return _color; }

    ~Light();

    static std::vector<Light *> getLights() { return _lights; }

private:
    glm::vec3 _color;

    static std::vector<Light *> _lights;
};

#endif //BILLIARDS_LIGHT_H
