//
// Created by Brave Fang on 2023/11/17.
//

#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "light.h"
#include "table_manager.h"

Scene::Scene() {
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

    Camera *camera;
    camera = new Camera("open", glm::vec3(3.0f, 1.5f, 0.0f), glm::vec3(-1.0f, -0.3f, 0.0f));
    camera = new Camera("cue", glm::vec3(1.5f, 1.5f, 1.5f), glm::vec3(-1, -1, -1), glm::vec3(0.0, 0.95, 0.5));
    camera = new Camera("god", glm::vec3(0, 3, 0), glm::vec3(0, -1, 0));

    Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());

    Light *light = new Light(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(5.0f, 5.0f, 5.0f));

    Sphere::initialize(billiard_shade_program);
    _table_manager = new TableManager();

    _table = new Model("table", "LuxuryPoolTable.obj", R"(resources\models\table\)", phongLighting_shade_program);
    _stick = new Model("stick", "CueStick.obj", R"(resources\models\stick\)", phongLighting_shade_program);
}

void Scene::update() {
    _table_manager->UpdateTable();
}

void Scene::render() {
    _table_manager->render();
    _table->render();
    if (Game::_game_state == AIMING || Game::_game_state == HITTING) {
        _stick->render();
    }
}