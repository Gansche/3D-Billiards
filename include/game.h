//
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_GAME_H
#define BILLIARDS_GAME_H

//class StateControler;

class Scene;

class Game {
public:
    Game();

    void initialize();

    void update();

    void render();

    void processKeyEvent(int key, int scancode, int action, int mode);

    void processMouseEvent(double xposIn, double yposIn);

    void processScrollEvent(double xoffset, double yoffset);

    ~Game();

private:
//    StateControler *_state;
    Scene *_scene;
};

#endif //BILLIARDS_GAME_H
