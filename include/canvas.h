//
// Created by Brave Fang on 2023/11/17.
//

#ifndef BILLIARDS_CANVAS_H
#define BILLIARDS_CANVAS_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "game.h"

class Canvas {
public:
    Canvas();

    void initialize();

    ~Canvas();

private:
    Game *_billiards;

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

#endif //BILLIARDS_CANVAS_H
