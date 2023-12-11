//
// Created by Brave Fang on 2023/11/17.
//

#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "table_manager.h"
#include "light.h"

Scene::Scene() {
    _camera = nullptr;
    _sphere = nullptr;
//    _table_manager = nullptr;
}

void Scene::generate() {
    _camera = new Camera(glm::vec3(0, 0, 3), glm::vec3(0, 0, -1));
    Shader *vertex_shader = new Shader("shaders/billiard_vs.glsl", "VERTEX");
    Shader *fragment_shader = new Shader("shaders/billiard_fs.glsl", "FRAGMENT");
    std::vector<Shader *> shaders;
    shaders.push_back(vertex_shader);
    shaders.push_back(fragment_shader);

    Program *program = new Program(shaders);
    _sphere = new Sphere(program);

}

void Scene::render() {
    _sphere->render();
}

Scene::~Scene() {
//    delete _camera;
//    delete _table_manager;
//    for (auto lightPtr: _lights) {
//        delete lightPtr;
//    }
}