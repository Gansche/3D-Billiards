// TODO: Unchecked: Not finished yet
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_CAMERA_H
#define BILLIARDS_CAMERA_H

#include <glm/glm.hpp>
#include <list>
#include "object.h"
#include "defs.h"

class Camera : public Object {
public:
    Camera(glm::vec3 position, glm::vec3 direction);

    Camera *use();

    glm::mat4 getViewMatrix() { return _view_matrix; }

    void dollyCamera(float dist);

    void truckCamera(float dx, float dy);

    void rotateCamera(float rx, float ry);

    ~Camera();

    static Camera *getCurrentCamera() { return _current_camera; }

private:
    glm::vec3 _horizontal;
    glm::vec3 _up;

    glm::mat4 _view_matrix;

    static Camera *_current_camera;
    static std::list<Camera *> _active_cameras;

    void updateVectors();
    void updateViewMatrix();
};

#endif //BILLIARDS_CAMERA_H
