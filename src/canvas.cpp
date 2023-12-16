//
// Created by Brave Fang on 2023/11/17.
//

#include <glm/gtc/matrix_transform.hpp>
#include "program.h"
#include "canvas.h"
#include "game.h"
#include "defs.h"

GLint Canvas::_width = 800;
GLint Canvas::_height = 600;
glm::mat4 Canvas::_projection_matrix = glm::mat4(1.0f);
Game *Canvas::_game = nullptr;
double Canvas::timeSinceLastFrame = 0;
double Canvas::lastFrameTime = 0;

bool Canvas::_left_key = false;
bool Canvas::_right_key = false;
bool Canvas::_first_pos = true;
double Canvas::_xpos = 0;
double Canvas::_ypos = 0;

void Canvas::initialize(Game *game) {
    _game = game;

    /* glfw pre_initialize */
    glfwInit();

    /* window settings */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* create window */
    GLFWwindow *window = glfwCreateWindow(_width, _height, "3D-Billiards", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    /* glad pre_initialize */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to pre_initialize GLAD" << std::endl;
        return;
    }

    /* Full Screen: may be useful */
//    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
//    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    /* callback settings */
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glEnable(GL_DEPTH_TEST);

    _game->initialize();

    /* view port adn projection matrix */
    glViewport(0, 0, _width, _height);
    _projection_matrix = glm::perspective(FoV, float(_width) / float(_height), 0.1f, 100.0f);
    Program::updateProjectionMatrix(_projection_matrix);

    double startTime = glfwGetTime();
    int frameCount = 0;

    /* main loop */
    while (!glfwWindowShouldClose(window)) {
        /* calculate the FPS */
        frameCount++;
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - startTime;
        if (elapsedTime >= 1.0f) {
            double fps = frameCount / elapsedTime;
            std::cout << "\rFPS: " << fps;
            startTime = currentTime;
            frameCount = 0;
        }

        timeSinceLastFrame = glfwGetTime() - lastFrameTime;
        lastFrameTime = glfwGetTime();


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
        glfwPollEvents();

        _game->update();
        _game->render();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

//todo 这些回调函数都没有设置好，因为不知道采取什么方式处理，就先不管了
void Canvas::key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
//    _game->processKeyEvent(key, scancode, action, mode);
}

void Canvas::window_size_callback(GLFWwindow *window, int newWidth, int newHeight) {
    _width = newWidth;
    _height = newHeight;
    glViewport(0, 0, _width, _height);
    _projection_matrix = glm::perspective(FoV, float(_width) / float(_height), 0.1f, 1000.0f);
    Program::updateProjectionMatrix(_projection_matrix);
}

void Canvas::cursor_callback(GLFWwindow *window, double xposIn, double yposIn) {
    if (_first_pos) {
        _xpos = xposIn;
        _ypos = yposIn;
        _first_pos = false;
    }
    double xoffset = _xpos - xposIn;
    double yoffset = _ypos - yposIn;
    _xpos = xposIn;
    _ypos = yposIn;
    _game->processMouseEvent(xoffset, yoffset);
}

void Canvas::mouse_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)_left_key = action;
    if (button == GLFW_MOUSE_BUTTON_RIGHT)_right_key = action;
}

void Canvas::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    _game->processScrollEvent(xoffset, yoffset);
}
