//
// Created by Brave Fang on 2023/11/17.
//

#include "scene.h"
#include "camera.h"
#include "table_manager.h"
#include "light.h"

Scene::Scene() {
    _camera = nullptr;
    _table_manager = nullptr;
}

void Scene::generate() {
    _camera = new Camera();
    _table_manager = new TableManager();
    // TODO: lights
}

void Scene::render() {
    _table_manager->render();
}

Scene::~Scene() {
    delete _camera;
    delete _table_manager;
    for (auto lightPtr: _lights) {
        delete lightPtr;
    }
}