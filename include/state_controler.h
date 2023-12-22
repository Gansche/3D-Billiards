//
// Created by Brave Fang on 2023/11/19.
//

#ifndef BILLIARDS_STATE_CONTROLER_H
#define BILLIARDS_STATE_CONTROLER_H

class StateControler {
public:
    StateControler() {
        _state = OPEN;
    }

    ~StateControler() = default;

private:
    int _state;
    static const int OPEN = 0;
    static const int AIMING = 1;

};

#endif //BILLIARDS_STATE_CONTROLER_H
