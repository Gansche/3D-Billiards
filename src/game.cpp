//
// Created by Brave Fang on 2023/11/17.
//

#include "scene.h"
#include "game.h"
#include "camera.h"
#include "canvas.h"
#include "table_manager.h"
#include <iostream>

int Game::_game_state = START;
int Game::_camera_state = OPEN;
double Game::_back = 0;

Game::Game() {
    _scene = nullptr;
}

void Game::initialize() {
    _scene = new Scene();
    _scene->generate();
}

void Game::update() {
    switch (_game_state) {
        case START:
        case AIMING: {
            _scene->update();
            break;
        }
        case HITTING: {
            _scene->update();
            _game_state = GOING;
            break;
        }
        case GOING: {
            _scene->update();
            if (_scene->getTableManager()->AllBallStatic()) {
                Camera::getActiveCameras()["cue"]->setRotateCenter(_scene->getTableManager()->getCueBallPosition());
                Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());
                _back = 0;
                _game_state = AIMING;
            }
            break;
        }
    }
}

void Game::render() {
    _scene->render();
}

void Game::processKeyEvent(int key, int scancode, int action, int mode) {
    switch (_game_state) {
        case START: {
            if (key == GLFW_KEY_ENTER && action == 1) {
                _game_state = AIMING;
                _camera_state = CUE;
                Camera::use("cue");
                Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());
            }
            break;
        }
        case AIMING: {
            if (key == GLFW_KEY_CAPS_LOCK && action == 1) {
                if (Camera::getCurrentCamera()->getName() == "god") {
                    Camera::use("cue");
                    _camera_state = CUE;
                } else if (Camera::getCurrentCamera()->getName() == "cue") {
                    Camera::use("god");
                    _camera_state = GOD;
                }
                Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());
            } else if (key == GLFW_KEY_SPACE && action == 1) {
                //TODO
                _scene->getTableManager()->setCueBallVelocity(-0.5 * Game::_back);
                _game_state = GOING;
            }
            break;
        }
        case HITTING:
        case GOING: {
            if (key == GLFW_KEY_CAPS_LOCK && action == 1) {
                if (Camera::getCurrentCamera()->getName() == "god") {
                    Camera::use("cue");
                    _camera_state = CUE;
                } else if (Camera::getCurrentCamera()->getName() == "cue") {
                    Camera::use("god");
                    _camera_state = GOD;
                }
                Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());
            }
            break;
        }
        default: {
            /* do nothing */
        }
    }
}

void Game::processMouseEvent(double xoffset, double yoffset) {
    switch (_game_state) {
        case START: {
            /* do nothing */
            break;
        }
        case AIMING:
        case HITTING:
        case GOING: {
            if (_camera_state == CUE) {
                Camera *cam = Camera::getCurrentCamera();
                if (Canvas::_left_key)cam->rotateCamera(0.005f * xoffset, 0.005 * yoffset);
                if (Canvas::_right_key)cam->dollyCamera(-0.005 * xoffset);
                Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());
            }
            break;
        }
        default: {
            /* do nothing */
        }
    }
}

void Game::processScrollEvent(double xoffset, double yoffset) {
    switch (_game_state) {
        case AIMING: {
            if (yoffset == -1.0f) {
                if (_back > -10.0f)_back += yoffset;
            } else if (yoffset == 1.0f) {
                if (_back < 0.0f)_back += yoffset;
            }
            break;
        }
        default: {
            /* do nothing */
        }
    }
}

Game::~Game() {
    delete _scene;
}
