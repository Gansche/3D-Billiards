// TODO: Unchecked: Not finished yet
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_CAMERA_H
#define BILLIARDS_CAMERA_H

#include <glm/glm.hpp>
#include <map>
#include "object.h"
#include "defs.h"

class Camera : public Object {
public:
    Camera(std::string name, glm::vec3 position, glm::vec3 direction,
           glm::vec3 rotate_center = glm::vec3(0.0f, 0.0f, 0.0f));

    std::string getName() { return _name; }

    glm::mat4 getViewMatrix() { return _view_matrix; }

    void setRotateCenter(glm::vec3 center);

    void dollyCamera(float dist);

    void truckCamera(float dx, float dy);

    void rotateCamera(float rx, float ry);

    ~Camera();

    static Camera *use(const std::string &name);

    static Camera *getCurrentCamera() { return _current_camera; }

private:
    glm::vec3 _direction;
    glm::vec3 _horizontal;
    glm::vec3 _up;

    glm::vec3 _rotate_center;

    glm::mat4 _view_matrix;

    static Camera *_current_camera;
    static std::map<std::string, Camera *> _active_cameras;

    bool hasCollision();

    void updateVectors();

    void updateViewMatrix();
};

#endif //BILLIARDS_CAMERA_H
