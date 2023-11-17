//
// Created by Brave Fang on 2023/11/17.
//
#include "canvas.h"

Canvas::Canvas() {
    _billiards = new Game();
}

//TODO: fix initialize()
void Canvas::initialize() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow *window = glfwCreateWindow(800, 600, "3D-Billiards", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    glfwSetKeyCallback(window,key_callback);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    _billiards->initialize();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return;
}

Canvas::~Canvas() {
    delete _billiards;
}

void Canvas::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

}

void Canvas::framebuffer_size_callback(GLFWwindow* window, int width, int height) {

}

void Canvas::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

}

void Canvas::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {

}
