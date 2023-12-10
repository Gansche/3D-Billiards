//
// Created by Brave Fang on 2023/11/17.
//

#include <glm/gtc/matrix_transform.hpp>
#include "program.h"
#include "canvas.h"
#include "game.h"

GLint Canvas::_width = 800;
GLint Canvas::_height = 600;
glm::mat4 Canvas::_projection_matrix = glm::mat4(1.0f);
Game *Canvas::_game = nullptr;
double Canvas::timeSinceLastFrame = 0;
double Canvas::lastFrameTime = 0;

void Canvas::initialize(Game *game) {
    _game = game;

    /* glfw initialize */
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

    /* glad initialize */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }
    _game->initialize();
    /* Full Screen: may be useful */
//    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
//    glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

    /* callback settings */
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* view port adn projection matrix */
    glViewport(0, 0, _width, _height);
    _projection_matrix = glm::perspective(90.0f, float(_width) / float(_height), 0.0f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    /* main loop */
    while (!glfwWindowShouldClose(window)) {
        timeSinceLastFrame = glfwGetTime() - lastFrameTime;
        lastFrameTime = glfwGetTime();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
        glfwPollEvents();
        //TODO
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
    _projection_matrix = glm::perspective(90.0f, float(_width) / float(_height), 0.0f, 100.0f);
    Program::updateProjectionMatrix(_projection_matrix);
}

void Canvas::mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
//    _game->processMouseEvent(xposIn, yposIn);
}

void Canvas::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
//    _game->processScrollEvent(xoffset, yoffset);
}
