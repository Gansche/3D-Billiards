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

/* object */

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

    virtual glm::vec3 setPosition(glm::vec3 newPosition);

    virtual glm::vec3 setDirection(glm::vec3 newDirection);

    virtual glm::vec3 setAcceleration(glm::vec3 newAcceleration);

    virtual glm::vec3 setVelocity(glm::vec3 newVelocity);

    virtual glm::vec3 setAngularAcceleration(glm::vec3 newAngularAcceleration);

    virtual glm::vec3 setAngularVelocity(glm::vec3 newAngularVelocity);

    virtual ~Object();

protected:
    glm::vec3 _position;
    glm::vec3 _direction;
    glm::vec3 _velocity;
    glm::vec3 _acceleration;
    glm::vec3 _angular_velocity;
    glm::vec3 _angular_acceleration;
};

/* sphere */

class Sphere : public Object {
public:
    explicit Sphere(glm::vec3 position, glm::vec3 direction = glm::vec3(0, 0, 0));

    void render();

    static void initialize(Program *program);

    ~Sphere() = default;

private:
    glm::mat4 _model_matrix;

    static GLuint VAO;
    static GLuint VBO;
    static GLuint EBO;

    static Program *_program;

    static std::vector<Vertex> _vertices;
    static std::vector<unsigned int> _indices;
};

/* mesh */

class Model : public Object {
public:
    Model(const char *filename, const char *directory, Program *program);

    void render();

    ~Model() = default;

private:
    glm::mat4 _model_matrix;

    GLuint VAO;
    GLuint VBO;
    GLuint EBO;

    Program *_program;

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;

    void initialize();
};


#endif //BILLIARDS_OBJECT_H