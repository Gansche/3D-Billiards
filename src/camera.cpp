//
// Created by Brave Fang on 2023/11/23.
//

#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "defs.h"

Camera::Camera(glm::vec3 center, glm::vec3 world_up, glm::vec3 direction) {
    _center = center;
    _world_up = world_up;
    _direction = glm::normalize(direction);
    _horizontal = glm::normalize(glm::cross(_direction, _world_up));
    _up = glm::normalize(glm::cross(_horizontal, _direction));
}

glm::mat4 Camera::getLookAtMatrix() const {
    return glm::lookAt(_center, _center + _direction, _up);
}

void Camera::rotateCamera() {

}