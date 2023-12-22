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

Game::Game() {
    _scene = nullptr;
}

void Game::initialize() {
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
                _scene->getTableManager()->setCueBallVelocity(5.0f);
            }
            break;
        }
    }
}

void Game::processMouseEvent(double xoffset, double yoffset) {
    switch (_game_state) {
        case START: {
            /* do nothing */
            break;
        }
        case AIMING: {
            if (_camera_state == CUE) {
                Camera *cam = Camera::getCurrentCamera();
                if (Canvas::_left_key)cam->rotateCamera(0.005f * xoffset, 0.005 * yoffset);
                if (Canvas::_right_key)cam->dollyCamera(-0.005 * xoffset);
                Program::updateViewMatrix(Camera::getCurrentCamera()->getViewMatrix());
                break;
            } else if (_camera_state == GOD) {
                /* do nothing */
            }
        }
    }
}

void Game::processScrollEvent(double xoffset, double yoffset) {

}

Game::~Game() {
    delete _scene;
}
