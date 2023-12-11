//
// Created by Brave Fang on 2023/11/22.
//

#ifndef BILLIARDS_OBJECT_H
#define BILLIARDS_OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game.h"
#include "program.h"
#include "stb_image.h"

typedef struct {
    glm::vec3 v;
    glm::vec3 n;
    glm::vec3 c;
    glm::vec2 u;
} Vertex;

class Program;

class Object {
public:
    Object();

    Object(glm::vec3 position, glm::vec3 direction);

    glm::vec3 getPosition() { return _position; }

    glm::vec3 getDirection() { return _direction; }

    glm::vec3 getVelocity() { return _velocity; }

    glm::vec3 getAcceleration() { return _acceleration; }

    glm::vec3 getAngularVelocity() { return _angular_velocity; }

    glm::vec3 getAngularAcceleration() { return _angular_acceleration; }

    virtual ~Object();

protected:
    glm::vec3 _position;
    glm::vec3 _direction;
    glm::vec3 _velocity;
    glm::vec3 _acceleration;
    glm::vec3 _angular_velocity;
    glm::vec3 _angular_acceleration;
};

class Sphere : public Object {
public:
    Sphere(Program *program);

    void render();

    ~Sphere() = default;

private:
    GLuint VAO;
    GLuint VBO[3];
    GLuint EBO;

    glm::vec3 m_Angle;
    glm::vec3 position;

    glm::mat4 _model_matrix;
    glm::mat4 _view_matrix;
    glm::mat4 _projection_matrix;

    float horizontalAngle;
    float verticalAngle;

    Program *_program;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void initialize();
};


#endif //BILLIARDS_OBJECT_H