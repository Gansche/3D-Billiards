//
// Created by Brave Fang on 2023/11/22.
//

#include <string>
#include "object.h"
#include "shader.h"
#include "defs.h"
#include "camera.h"
#include "tiny_obj_loader.h"

/* object */
Object::Object() {

}

Object::Object(glm::vec3 position, glm::vec3 direction) :
        _position(position), _direction(direction) {
}

glm::vec3 Object::setPosition(glm::vec3 newPosition) { return _position = newPosition; }

glm::vec3 Object::setDirection(glm::vec3 newDirection) { return _direction = newDirection; }

glm::vec3 Object::setAcceleration(glm::vec3 newAcceleration) { return _acceleration = newAcceleration; }

glm::vec3 Object::setVelocity(glm::vec3 newVelocity) { return _velocity = newVelocity; }

glm::vec3 Object::setAngularAcceleration(
        glm::vec3 newAngularAcceleration) { return _angular_acceleration = newAngularAcceleration; }

glm::vec3 Object::setAngularVelocity(glm::vec3 newAngularVelocity) { return _angular_velocity = newAngularVelocity; }

Object::~Object() = default;

/* sphere */
GLuint Sphere::VAO = 0;
GLuint Sphere::VBO = 0;
GLuint Sphere::EBO = 0;

Program *Sphere::_program = nullptr;

std::vector<Vertex> Sphere::_vertices{};
std::vector<unsigned int> Sphere::_indices{};


Sphere::Sphere(glm::vec3 position, glm::vec3 direction) : Object(position, direction) {
    _model_matrix = glm::mat4(1.0f);
    _model_matrix = glm::translate(_model_matrix, _position);

}

//todo:debug
void Sphere::render() {
    _program->bind();
    _program->setMat4("model", _model_matrix);
    _program->setVec3("viewPos", Camera::getCurrentCamera()->getPosition());

    glBindVertexArray(VAO);
    // Draw the triangle !
    //glDrawArrays(GL_TRIANGLES, 0, 6); // 3 _indices starting at 0 -> 1 triangle

    // could be modified to use GL_TRIANGLE_FAN and GL_TRIANGLE_STRIP
    // to draw the poles and the rest of the sphere,
    // but this will be 2 draw call. Should we??? (draw cal optimizations and such things...)
    glDrawElements(
            GL_TRIANGLES,      // mode
            _indices.size(),    // count
            GL_UNSIGNED_INT,   // type
            (void *) 0           // element array buffer offset
    );//*/
    glBindVertexArray(0);
    _program->unbind();
}

void Sphere::initialize(Program *program) {
    _program = program;

    _program->bind();
    _program->setVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    _program->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    _program->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    _program->unbind();

    Vertex v;
    //Vertex 0
    v.c = glm::vec3(0.529, 0.808, 0.922);

    int indicator = 1;
    float deltaPhi = -glm::pi<float>() / LONGITUDE;
    float deltaTheta = 2.0f * glm::pi<float>() / LATITUDE;
    for (int i = 0; i <= LATITUDE; i++) {
        float theta = float(i) * deltaTheta;
        float thetaPrime = theta + deltaTheta;

        // projection of the first theta in the horizontal plane
        float x0 = std::cos(theta);
        float y0 = std::sin(theta);

        // projection of the second theta in the horizontal plane
        float x1 = std::cos(thetaPrime);
        float y1 = std::sin(thetaPrime);

        for (int j = 0; j <= LONGITUDE; j++) {

            v.c = glm::vec3(0.529, 0.808, 0.922);

            // polar angle
            float phi = j * deltaPhi;

            // polar vector in a vertical plane
            float xPolar = std::cos(phi);
            float yPolar = std::sin(phi);

            // vertex #2 (theta , phiPrime)
            v.n = glm::vec3(yPolar * x1, yPolar * y1, xPolar);
            v.v = v.n * RADIUS;
            v.u = glm::vec2((float) i / LATITUDE, (float) j / LONGITUDE);
            _vertices.push_back(v);
            //_indices.push_back(indicator);
            indicator++;

            v.n = glm::vec3(yPolar * x0, yPolar * y0, xPolar);
            v.v = v.n * RADIUS;
            v.u = glm::vec2((float) i / LATITUDE, (float) j / LONGITUDE);
            _vertices.push_back(v);
            //_indices.push_back(indicator);
            indicator++;

            _indices.push_back(indicator - 3);
            _indices.push_back(indicator);
            _indices.push_back(indicator - 1);

            _indices.push_back(indicator - 2);
            _indices.push_back(indicator);
            _indices.push_back(indicator - 3);

        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) reinterpret_cast<void *>(offsetof(Vertex, c)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) reinterpret_cast<void *>(offsetof(Vertex, n)));
    glEnableVertexAttribArray(2);

}

/* mesh */

Model::Model(const char *filename, const char *directory, Program *program) : _program(program) {
    std::string _filename(filename);
    std::string _directory(directory);
    std::string name = _directory + _filename;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;
    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, name.c_str(), directory);
    if (!warn.empty())std::cout << warn << std::endl;
    if (!err.empty())std::cout << err << std::endl;
    if (!ok)exit(0);

    std::vector<tinyobj::index_t> indices = shapes[5].mesh.indices;
    std::vector<tinyobj::real_t> vertices = attrib.vertices;
    std::vector<tinyobj::real_t> normals = attrib.normals;
    for (auto &index: indices) {
        Vertex vertex;
        vertex.v = glm::vec3(vertices[3 * index.vertex_index], vertices[3 * index.vertex_index + 1],
                             vertices[3 * index.vertex_index + 2]);
        vertex.n = glm::vec3(normals[3 * index.normal_index], normals[3 * index.normal_index + 1],
                             normals[3 * index.normal_index + 2]);
        vertex.c = glm::vec3(0.529, 0.808, 0.922);
        vertex.u = glm::vec2(0.0f, 0.0f);
        _vertices.push_back(vertex);
    }

    initialize();
//    std::cout << _vertices[0].v.x << std::endl;
//    std::cout << _vertices[0].v.y << std::endl;
//    std::cout << _vertices[0].v.z << std::endl;

    _model_matrix = glm::mat4(1.0f);
//    _model_matrix = glm::translate(_model_matrix, _position);
}

void Model::render() {
    _program->bind();
    _program->setMat4("model", _model_matrix);
    _program->setVec3("viewPos", Camera::getCurrentCamera()->getPosition());

    glBindVertexArray(VAO);
    // Draw the triangle !
    //glDrawArrays(GL_TRIANGLES, 0, 6); // 3 _indices starting at 0 -> 1 triangle

    // could be modified to use GL_TRIANGLE_FAN and GL_TRIANGLE_STRIP
    // to draw the poles and the rest of the sphere,
    // but this will be 2 draw call. Should we??? (draw cal optimizations and such things...)
//    glDrawElements(
//            GL_TRIANGLES,      // mode
//            _vertices.size(),    // count
//            GL_UNSIGNED_INT,   // type
//            (void *) 0           // element array buffer offset
//    );
    glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
    glBindVertexArray(0);
    _program->unbind();
}

void Model::initialize() {
    _program->bind();
    _program->setVec3("lightPos", glm::vec3(1.2f, 1.0f, 2.0f));
    _program->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    _program->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    _program->unbind();

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

//    glGenBuffers(1, &EBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) reinterpret_cast<void *>(offsetof(Vertex, c)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) reinterpret_cast<void *>(offsetof(Vertex, n)));
    glEnableVertexAttribArray(2);
}