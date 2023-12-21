//
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_SCENE_H
#define BILLIARDS_SCENE_H

#include <vector>
#include "object.h"

class Camera;

class Light;

class Model;

class TableManager;

class Scene {
public:
    Scene();

    void generate();

    void update();

    void render();

    Camera *getCamera() const { return _camera; }

//    TableManager *getTableManager() const { return _table_manager; }

//    std::vector<Light *> getLights() { return _lights; }

    ~Scene() = default;

private:
    Camera *_camera;
    TableManager *_table_manager;
    Model * _mesh;
    Model * _mesh2;
};

#endif //BILLIARDS_SCENE_H
