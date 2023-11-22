//
// Created by Brave Fang on 2023/11/19.
//

#ifndef BILLIARDS_STATE_CONTROLER_H
#define BILLIARDS_STATE_CONTROLER_H

class StateControler {
public:
    StateControler() {

    }

    ~StateControler() = default;

private:
    int _state;
    static const int IDEL = 0;
};

#endif //BILLIARDS_STATE_CONTROLER_H
