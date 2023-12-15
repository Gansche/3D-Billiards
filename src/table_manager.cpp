//
// Created by Brave Fang on 2023/11/22.
//

#include "table_manager.h"
#include "shader.h"

TableManager::TableManager() {
    Sphere *newSphere;
//    for (int i = 0; i < 10; i++) {
//        newSphere = new Sphere(glm::vec3(i, 0, 0));
//        _billiards.push_back(newSphere);
//        newSphere = new Sphere(glm::vec3(0, i, 0));
//        _billiards.push_back(newSphere);
//        newSphere = new Sphere(glm::vec3(0, 0, i));
//        _billiards.push_back(newSphere);
//    }
    newSphere = new Sphere(glm::vec3(0, 0, 0));
    _billiards.push_back(newSphere);
}

void TableManager::render() {
    for (auto &_billiard: _billiards) {
        _billiard->render();
    }
}

TableManager::~TableManager() = default;