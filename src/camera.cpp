//
// Created by Brave Fang on 2023/11/23.
//

#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include "defs.h"

Camera *Camera::_current_camera = nullptr;
std::map<std::string, Camera *> Camera::_active_cameras{};

Camera::Camera(std::string name, glm::vec3 position, glm::vec3 direction, glm::vec3 rotate_center) :
        Object(position),
        _rotate_center(rotate_center) {
    _name = std::move(name);

    _direction = glm::normalize(direction);
    if (_name == "god") {
        _horizontal = glm::vec3(0.0f, 0.0f, -1.0f);
        _up = glm::vec3(-1.0f, 0.0f, 0.0f);
        _active_cameras[_name] = this;
        _view_matrix = glm::lookAt(_position, _position + _direction, _up);
    } else {
        _horizontal = glm::normalize(glm::cross(_direction, Y_UNIT_VECTOR));
        _up = glm::normalize(glm::cross(_horizontal, _direction));
        _position += _rotate_center;
        _active_cameras[_name] = this;
        updateViewMatrix();
    }
    if (_current_camera == nullptr)use(_name);
}

Camera *Camera::use(const std::string &name) {
    _current_camera = _active_cameras[name];
    return _current_camera;
}

void Camera::setRotateCenter(glm::vec3 center) {
    _direction = glm::normalize(_rotate_center - _position);
    _horizontal = glm::normalize(glm::cross(_direction, Y_UNIT_VECTOR));
    _up = glm::normalize(glm::cross(_horizontal, _direction));
    _position -= _rotate_center;
    _rotate_center = center;
    _position += _rotate_center;
    updateViewMatrix();
}

void Camera::dollyCamera(float dist) {
    glm::vec3 tempPosition = _position;
    _position += _direction * dist;
    if (hasCollision()) {
        _position = tempPosition;
    } else
        updateViewMatrix();
}

void Camera::truckCamera(float dx, float dy) {
    glm::vec3 tempPosition = _position;
    _position += _horizontal * dx + _up * dy;
    if (hasCollision()) {
        _position = tempPosition;
    } else
        updateViewMatrix();
}

void Camera::rotateCamera(float rx, float ry) {
    if (this->_name == "god") return;

    glm::vec3 tempPosition = _position;
    glm::vec3 tempDirection = _direction;

    float angle = static_cast<float>(acos(glm::dot(Y_UNIT_VECTOR, _direction)));
    if (angle - ry > 3.13f)
        ry = angle - 3.13f;
    else if (angle - ry < 0.01f)
        ry = angle - 0.01f;

    glm::mat4 trans_1 = glm::translate(glm::mat4(1.0f), -_rotate_center);
    glm::mat4 trans_2 = glm::translate(glm::mat4(1.0f), _rotate_center);
    glm::mat4 rotMat = glm::rotate(glm::mat4(1.0f), rx, Y_UNIT_VECTOR)
                       * glm::rotate(glm::mat4(1.0f), ry, _horizontal);
    glm::mat4 final_matrix = trans_2 * rotMat * trans_1;
    glm::vec4 transformedPosition = final_matrix * glm::vec4(_position.x, _position.y, _position.z, 1.0f);
    _position = glm::vec3(transformedPosition.x, transformedPosition.y, transformedPosition.z);

    glm::vec4 transformedDirection = rotMat * glm::vec4(_direction.x, _direction.y, _direction.z, 0.0f);
    _direction = glm::vec3(transformedDirection.x, transformedDirection.y, transformedDirection.z);

    if (hasCollision()) {
        _position = tempPosition;
        _direction = tempDirection;
    } else {
        updateVectors();
        updateViewMatrix();
    }
}

bool Camera::hasCollision() {
    if ((abs(_position.x) <= 1.0) && (_position.y <= 1.1 && _position.y >= 0.55) && (abs(_position.z) <= 1.7)
        || ((abs(_position.x) <= 1.0) && (_position.y < 0.55) && (abs(_position.z) <= 1.5))
        || _position.y <= 0.2
        || abs(_position.y) >= 7
        || sqrtf(_position.x * _position.x + _position.z * _position.z) >= 7
            ) {
        return true;
    }
    return false;
}

void Camera::updateVectors() {
    _direction = glm::normalize(_direction);
    _horizontal = glm::normalize(glm::cross(_direction, Y_UNIT_VECTOR));
    _up = glm::normalize(glm::cross(_horizontal, _direction));
}

void Camera::updateViewMatrix() {
    _view_matrix = glm::lookAt(_position, _position + _direction, Y_UNIT_VECTOR);
}

Camera::~Camera() {
    _active_cameras.erase(_name);
}
