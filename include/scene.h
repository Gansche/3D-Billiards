//
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_SCENE_H
#define BILLIARDS_SCENE_H

#include <vector>
#include "camera.h"
#include "table_manager.h"
#include "light.h"

class Scene {
public:
private:
    Camera *_camera;
    TableManager *_table_manager;
    std::vector<Light *> _lights;
};

#endif //BILLIARDS_SCENE_H
