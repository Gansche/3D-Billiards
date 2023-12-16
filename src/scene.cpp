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
    Shader *billiard_vertex_shader = new Shader("resources/shaders/billiard_vs.glsl", "VERTEX");
    Shader *billiard_fragment_shader = new Shader("resources/shaders/billiard_fs.glsl", "FRAGMENT");
    std::vector<Shader *> billiard_shaders;
    billiard_shaders.push_back(billiard_vertex_shader);
    billiard_shaders.push_back(billiard_fragment_shader);
    Program *billiard_shade_program = new Program(billiard_shaders);

    Shader *phongLighting_vertex_shader = new Shader("resources/shaders/table_vs.glsl", "VERTEX");
    Shader *phongLighting_fragment_shader = new Shader("resources/shaders/table_fs.glsl", "FRAGMENT");
    std::vector<Shader *> phongLighting_shaders;
    phongLighting_shaders.push_back(phongLighting_vertex_shader);
    phongLighting_shaders.push_back(phongLighting_fragment_shader);
    Program *phongLighting_shade_program = new Program(phongLighting_shaders);

    _camera = new Camera(glm::vec3(3, 3, 3), glm::vec3(-1, -1, -1));
    Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());

    Sphere::initialize(billiard_shade_program);
    _table_manager = new TableManager();

    _mesh = new Model("LuxuryPoolTable.obj", "resources/models/table/", phongLighting_shade_program);

}

void Scene::update(){
    _table_manager->UpdateTable();
}

void Scene::render() {
    _table_manager->render();
    _mesh->render();
}

Scene::~Scene() {
//    delete _camera;
//    delete _table_manager;
//    for (auto lightPtr: _lights) {
//        delete lightPtr;
//    }
}