//
// Created by Brave Fang on 2023/11/23.
//

#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "defs.h"

Camera *Camera::_current_camera = nullptr;
std::list<Camera *> Camera::_active_cameras{};

Camera::Camera(glm::vec3 position, glm::vec3 direction) : Object(position) {
    _direction = glm::normalize(direction);
    _horizontal = glm::normalize(glm::cross(_direction, Y_UNIT_VECTOR));
    _up = glm::normalize(glm::cross(_horizontal, _direction));
    _active_cameras.push_back(this);
    updateViewMatrix();
    if (_current_camera == nullptr)this->use();
}

Camera *Camera::use() {
    _current_camera = this;
    return this;
}

void Camera::dollyCamera(float dist) {
    _position += _direction * dist;
    updateViewMatrix();
}

void Camera::truckCamera(float dx, float dy) {
    _position += _horizontal * dx + _up * dy;
    updateViewMatrix();
}

void Camera::rotateCamera(float rx, float ry) {
    float angle = static_cast<float>(acos(glm::dot(Y_UNIT_VECTOR, _direction)));
    if (angle - ry > 3.13f)
        ry = angle - 3.13f;
    else if (angle - ry < 0.01f)
        ry = angle - 0.01f;

    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rx, Y_UNIT_VECTOR)
                       * glm::rotate(glm::mat4(1.0f), ry, _horizontal);
//    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), ry, _horizontal);
    glm::vec4 transformedPosition = rotMat * glm::vec4(_position.x, _position.y, _position.z, 1.0f);
    _position = glm::vec3(transformedPosition.x, transformedPosition.y, transformedPosition.z);
    glm::vec4 transformedDirection = rotMat * glm::vec4(_direction.x, _direction.y, _direction.z, 0.0f);
    _direction = glm::vec3(transformedDirection.x, transformedDirection.y, transformedDirection.z);
    _direction = glm::normalize(_direction);
    updateVectors();
    updateViewMatrix();
}

void Camera::updateVectors() {
    _horizontal = glm::normalize(glm::cross(_direction, Y_UNIT_VECTOR));
    _up = glm::normalize(glm::cross(_horizontal, _direction));
}

void Camera::updateViewMatrix() {
    _view_matrix = glm::lookAt(_position, _position + _direction, Y_UNIT_VECTOR);
}

Camera::~Camera() {
    _active_cameras.remove(this);
}
