//
// Created by Brave Fang on 2023/11/17.
//

#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "table_manager.h"

Scene::Scene() {
    _camera = nullptr;
    _table_manager = nullptr;
}

void Scene::generate() {
    Shader *vertex_shader = new Shader("shaders/billiard_vs.glsl", "VERTEX");
    Shader *fragment_shader = new Shader("shaders/billiard_fs.glsl", "FRAGMENT");
    std::vector<Shader *> shaders;
    shaders.push_back(vertex_shader);
    shaders.push_back(fragment_shader);

    Program *program = new Program(shaders);

    _camera = new Camera(glm::vec3(3, 3, 3), glm::vec3(-1, -1, -1));
    Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());

    Sphere::initialize(program);
    _table_manager = new TableManager();

}

void Scene::render() {
    _table_manager->render();
}

Scene::~Scene() {
//    delete _camera;
//    delete _table_manager;
//    for (auto lightPtr: _lights) {
//        delete lightPtr;
//    }
}