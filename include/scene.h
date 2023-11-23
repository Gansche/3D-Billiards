//
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_SCENE_H
#define BILLIARDS_SCENE_H

#include <vector>

class Camera;

class Light;

class TableManager;

class Scene {
public:
    Scene();

    void generate();

    void render();

    Camera *getCamera() const { return _camera; }

    TableManager *getTableManager() const { return _table_manager; }

    std::vector<Light *> getLights() { return _lights; }

    ~Scene();

private:
    Camera *_camera;
    TableManager *_table_manager;
    std::vector<Light *> _lights{};
};

#endif //BILLIARDS_SCENE_H
