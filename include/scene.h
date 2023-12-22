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

    TableManager *getTableManager() { return _table_manager; }

    ~Scene() = default;

private:
    TableManager *_table_manager;
    Model *_table;
    Model *_stick;
    Model *_room;
};

#endif //BILLIARDS_SCENE_H
