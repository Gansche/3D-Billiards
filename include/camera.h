// TODO: Unchecked: Not finished yet
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_CAMERA_H
#define BILLIARDS_CAMERA_H

#include <glm/glm.hpp>
#include <list>

class Camera {
public:
    explicit Camera(glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f),
                    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f));

    glm::mat4 getLookAtMatrix() { return _view_matrix; }

    // TODO: camera rotation and movement
    void rotateCamera();

    ~Camera() = default;

    static Camera *getCurrentCamera() { return _current_camera; }

private:
    glm::vec3 _center;
    glm::vec3 _horizontal;
    glm::vec3 _direction;
    glm::vec3 _up;
    glm::vec3 _world_up;

    glm::mat4 _view_matrix;

    static Camera *_current_camera;
    static std::list<Camera *> _active_cameras;

    void updateViewMatrix();
};

#endif //BILLIARDS_CAMERA_H
