//
// Created by Brave Fang on 2023/11/22.
//

#ifndef BILLIARDS_TABLE_MANAGER_H
#define BILLIARDS_TABLE_MANAGER_H

#include <vector>
#include "object.h"

class Edge {
public:
private:
};

class Hole {
public:
private:
};

class TableManager {
public:
    TableManager();

    void render();

    ~TableManager();

private:
    std::vector<Sphere *> _billiards{};
    std::vector<Edge *> _edges{};
    std::vector<Hole *> _holes{};
};

#endif //BILLIARDS_TABLE_MANAGER_H
