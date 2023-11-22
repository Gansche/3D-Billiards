//
// Created by Brave Fang on 2023/11/22.
//

#ifndef BILLIARDS_TABLE_MANAGER_H
#define BILLIARDS_TABLE_MANAGER_H

#include <vector>
#include "object.h"

class TableManager {
public:
    TableManager();

    ~TableManager();

private:
    std::vector<Sphere *> _billiards;
};

#endif //BILLIARDS_TABLE_MANAGER_H
