//
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_GAME_H
#define BILLIARDS_GAME_H

/* game state */
const int START = 0x00;
const int AIMING = 0x01;

/* camera state */
const int OPEN = 0x10;
const int CUE = 0x11;
const int GOD = 0x12;
const int FREE = 0x13;

class Scene;

class Game {
public:
    Game();

    void initialize();

    void update();

    void render();

    void processKeyEvent(int key, int scancode, int action, int mode);

    void processMouseEvent(double xoffset, double yoffset);

    void processScrollEvent(double xoffset, double yoffset);

    ~Game();

    static int _game_state;
    static int _camera_state;

private:
    Scene *_scene;
};

#endif //BILLIARDS_GAME_H
