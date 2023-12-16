//
// Created by Brave Fang on 2023/11/17.
//

#include "state_controler.h"
#include "scene.h"
#include "game.h"
#include "camera.h"
#include "canvas.h"
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
    _scene->update();
}

void Game::render() {
    _scene->render();
}

void Game::processKeyEvent(int key, int scancode, int action, int mode) {

}

void Game::processMouseEvent(double xoffset, double yoffset) {
    Camera *cam = Camera::getCurrentCamera();
    if (Canvas::_left_key)cam->rotateCamera(0.005f * xoffset, 0.005 * yoffset);
    if (Canvas::_right_key)cam->dollyCamera(-0.005 * xoffset);
    Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());
}

void Game::processScrollEvent(double xoffset, double yoffset) {

}

Game::~Game() {
//    delete _state;
    delete _scene;
}
