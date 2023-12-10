//
// Created by Brave Fang on 2023/11/17.
//

#include "state_controler.h"
#include "scene.h"
#include "game.h"
#include <iostream>

Game::Game() {
//    _state = nullptr;
    _scene = nullptr;
}

void Game::initialize() {
//    _state = new StateControler();
    _scene = new Scene();
    _scene->generate();
}

void Game::update() {

}

void Game::render() {
    _scene->render();
}

void Game::processKeyEvent(int key, int scancode, int action, int mode) {

}

void Game::processMouseEvent(double xposIn, double yposIn) {

}

void Game::processScrollEvent(double xoffset, double yoffset) {

}

Game::~Game() {
//    delete _state;
    delete _scene;
}
