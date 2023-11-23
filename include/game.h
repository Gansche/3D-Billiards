//
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_GAME_H
#define BILLIARDS_GAME_H

#include "state_controler.h"
#include "scene.h"

class Game {
public:
    Game();

    void initialize();

    void update();

    void render();

    ~Game();

private:
    StateControler *_state;
    Scene *_scene;
};

#endif //BILLIARDS_GAME_H
