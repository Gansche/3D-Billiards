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
#include <string>
#include "game.h"
#include "program.h"
#include "stb_image.h"

typedef struct {
    glm::vec3 v;
    glm::vec3 n;
    glm::vec3 c;
    glm::vec2 u;
} Vertex;

typedef struct {
    int count;
    GLuint VAO;
    GLuint VBO;
} Shape;

class Texture;

class Program;

/* object */

class Object {
public:
    Object();

    Object(glm::vec3 position);

    glm::vec3 getPosition() { return _position; }

    glm::vec3 getVelocity() { return _velocity; }

    double getAcceleration() const { return _acceleration; }

    double getAngularVelocity() const { return _angular_velocity; }

    glm::vec3 getAngularAcceleration() { return _angular_acceleration; }

    virtual glm::vec3 setPosition(glm::vec3 newPosition);

    virtual double setAcceleration(double newAcceleration);

    virtual glm::vec3 setVelocity(glm::vec3 newVelocity);

    virtual glm::vec3 setAngularAcceleration(glm::vec3 newAngularAcceleration);

    virtual double setAngularVelocity(double newAngularVelocity);

    virtual double setAngle(double angle);

    float getAngle() const { return _angle; }

    virtual ~Object();

protected:
    glm::vec3 _position;
    glm::vec3 _velocity;
    double _angular_velocity;
    double _acceleration;
    double _angle;
    glm::vec3 _angular_acceleration;
};

/* sphere */

class Sphere : public Object {
public:
    explicit Sphere(std::string name, glm::vec3 position);

    void render();

    static void initialize(Program *program);

    bool getIfInHole() const { return if_in_hole; }

    std::string getName() const { return _name; }

    virtual bool setIfInHole(bool flag);

    virtual std::string setName(std::string newName);

    void update(double time);

    ~Sphere() = default;

private:
    bool if_in_hole;
    std::string _name;

    glm::mat4 _model_matrix;

    Texture *_diffuse_texture;

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

    Texture *_diffuse_texture;
    Texture *_metalness_texture;
    Texture *_normal_texture;
    Texture *_roughness_texture;
    Program *_program;

    std::vector<Shape *> _shapes{};
};


#endif //BILLIARDS_OBJECT_H