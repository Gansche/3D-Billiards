//
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_CANVAS_H
#define BILLIARDS_CANVAS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class Game;

class Canvas {
public:
    Canvas() = default;

    void initialize(Game *game);

    ~Canvas() = default;

    static glm::mat4 getProjectionMatrix() { return _projection_matrix; }

private:
    static GLint _width;
    static GLint _height;
    static glm::mat4 _projection_matrix;
    static Game *_game;
    static double timeSinceLastFrame;
    static double lastFrameTime;

    static bool _left_key;
    static bool _first_pos;
    static double _xpos;
    static double _ypos;

    static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

    static void window_size_callback(GLFWwindow *window, int newWidth, int newHeight);

    static void cursor_callback(GLFWwindow *window, double xposIn, double yposIn);

    static void mouse_callback(GLFWwindow* window, int button, int action, int mods);

    static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
};

#endif //BILLIARDS_CANVAS_H
