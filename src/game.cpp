//
// Created by Brave Fang on 2023/11/17.
//

#include "game.h"

Game::Game() {

}

void Game::initialize() {

}

void Game::update() {

}

void Game::render() {
    _scene->render();
}

Game::~Game() {
    delete _state;
    delete _scene;
}