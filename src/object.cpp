//
// Created by Brave Fang on 2023/11/22.
//

#include <string>
#include <utility>
#include "texture.h"
#include "object.h"
#include "shader.h"
#include "defs.h"
#include "camera.h"
#include "light.h"
#include "tiny_obj_loader.h"

/* object */
Object::Object() {
    _position = glm::vec3(0.0f, 0.0f, 0.0f);
    _velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    _acceleration = 0;
    _angular_velocity = 0.0f;
    _angular_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

Object::Object(glm::vec3 position) : _position(position) {
    _velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    _acceleration = 0;
    _angular_velocity = 0.0f;
    _angular_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::vec3 Object::setPosition(glm::vec3 newPosition) {
    return _position = newPosition;

}

double Object::setAcceleration(double newAcceleration) { return _acceleration = newAcceleration; }

glm::vec3 Object::setVelocity(glm::vec3 newVelocity) { return _velocity = newVelocity; }

glm::vec3 Object::setAngularAcceleration(
        glm::vec3 newAngularAcceleration) { return _angular_acceleration = newAngularAcceleration; }

double Object::setAngularVelocity(double newAngularVelocity) { return _angular_velocity = newAngularVelocity; }

double Object::setAngle(double angle) { return _angle = angle; }

Object::~Object() = default;

/* sphere */
GLuint Sphere::VAO = 0;
GLuint Sphere::VBO = 0;
GLuint Sphere::EBO = 0;

Program *Sphere::_program = nullptr;

std::vector<Vertex> Sphere::_vertices{};
std::vector<unsigned int> Sphere::_indices{};


Sphere::Sphere(std::string name, glm::vec3 position) : Object(position) {
    _name = std::move(name);

    _model_matrix = glm::mat4(1.0f);
    _model_matrix = glm::translate(_model_matrix, _position);

    _program->bind();
    std::string textureSource = "resources/textures/" + _name + ".png";
    _diffuse_texture = new Texture(textureSource.c_str());
    _diffuse_texture->generate();

    _program->setInt("diffuse_texture", 0);
    _program->unbind();
}

void Sphere::update(double t) {
    glm::vec3 pos = getPosition();
    glm::vec3 v = getVelocity();

    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 axis = glm::normalize(-glm::cross(v, up));

    float angle = getAngle();

    double omega = getAngularVelocity();
    angle += omega * t;
    if (angle > 360.0)
        angle -= 360.0;
    double v_scale = (double) sqrtf(glm::dot(v, v));

    if (omega * RADIUS < v_scale) {
        omega += 2.5 * ACCLAR * 75.0 / (RADIUS * 1.0) * t;
    } else {
        omega = 75.0 * v_scale / RADIUS;
    }
    setAngle(angle);
    setAngularVelocity(omega);

    double a = getAcceleration();

    double px = pos.x;
    double py = pos.y;
    double pz = pos.z;

    double vx = v.x;
    double vy = v.y;
    double vz = v.z;


    if (sqrtf(glm::dot(v, v)) < EPS) {
        setVelocity(glm::vec3(0, 0, 0));
        setAngularVelocity(0.0f);
        return;
    }

    double ax = (vx / sqrtf(vx * vx + vz * vz)) * a;
    double az = (vz / sqrtf(vx * vx + vz * vz)) * a;

    if (abs(vx) < abs(ax * t) || abs(vz) < abs(az * t)) {
        px += vx * t - 0.5 * ax * t * t;
        pz += vz * t - 0.5 * az * t * t;
        vx -= 0;
        vz -= 0;
    } else {
        px += vx * t - 0.5 * ax * t * t;
        pz += vz * t - 0.5 * az * t * t;
        vx -= ax * t;
        vz -= az * t;
    }

    setPosition(glm::vec3(px, py, pz));
    setVelocity(glm::vec3(vx, vy, vz));


    glm::mat4 rotate_mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    glm::mat4 translate_mat = glm::translate(glm::mat4(1.0f), getPosition());
    _model_matrix = translate_mat * rotate_mat;
}

bool Sphere::setIfInHole(bool flag) { return if_in_hole = flag; }

std::string Sphere::setName(std::string newName) { return _name = newName; }

void Sphere::render() {
    _program->bind();
    _program->setMat4("model", _model_matrix);
    _program->setVec3("cameraPos", Camera::getCurrentCamera()->getPosition());

    glActiveTexture(GL_TEXTURE0);
    _diffuse_texture->bind();
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, (void *) 0);
    glBindVertexArray(0);
    _program->unbind();
}

void Sphere::initialize(Program *program) {
    _program = program;

    std::vector<Light *> lights = Light::getLights();

    _program->bind();
    _program->setVec3("lightPos", lights[0]->getPosition());
    _program->setVec3("lightColor", lights[0]->getColor());
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
            indicator++;

            v.n = glm::vec3(yPolar * x0, yPolar * y0, xPolar);
            v.v = v.n * RADIUS;
            v.u = glm::vec2((float) i / LATITUDE, (float) j / LONGITUDE);
            _vertices.push_back(v);
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
                          (void *) reinterpret_cast<void *>(offsetof(Vertex, n)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) reinterpret_cast<void *>(offsetof(Vertex, u)));
    glEnableVertexAttribArray(2);
}

/* mesh */

Model::Model(const std::string &name, const char *filename, const char *directory, Program *program) : _program(
        program) {
    _name = name;

    std::string _filename(filename);
    std::string _directory(directory);
    std::string rout = _directory + _filename;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;
    bool ok = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, rout.c_str(), directory);
    if (!warn.empty())std::cout << warn << std::endl;
    if (!err.empty())std::cout << err << std::endl;
    if (!ok)exit(0);


    std::vector<Vertex> _vertices;

    std::vector<tinyobj::real_t> vertices = attrib.vertices;
    std::vector<tinyobj::real_t> normals = attrib.normals;
    std::vector<tinyobj::real_t> texcoords = attrib.texcoords;

    for (auto &shapePtr: shapes) {
        std::vector<tinyobj::index_t> indices = shapePtr.mesh.indices;
        for (auto &index: indices) {
            Vertex vertex;
            vertex.v = glm::vec3(vertices[3 * index.vertex_index], vertices[3 * index.vertex_index + 1],
                                 vertices[3 * index.vertex_index + 2]);
            vertex.n = glm::vec3(normals[3 * index.normal_index], normals[3 * index.normal_index + 1],
                                 normals[3 * index.normal_index + 2]);
            vertex.c = glm::vec3(0.529, 0.808, 0.922);
            vertex.u = glm::vec2(texcoords[2 * index.texcoord_index], texcoords[2 * index.texcoord_index + 1]);
            _vertices.push_back(vertex);
        }

        Shape *shape = new Shape;

        glGenVertexArrays(1, &shape->VAO);
        glBindVertexArray(shape->VAO);

        glGenBuffers(1, &shape->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, shape->VBO);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), &_vertices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) reinterpret_cast<void *>(offsetof(Vertex, n)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *) reinterpret_cast<void *>(offsetof(Vertex, u)));
        glEnableVertexAttribArray(2);

        shape->count = _vertices.size();
        _shapes.push_back(shape);
        _vertices.clear();
    }

    _model_matrix = glm::mat4(1.0f);

    _diffuse_texture = new Texture("resources/models/table/LuxuryPoolTable_Diffuse_Map.png");
    _diffuse_texture->generate();

    _metalness_texture = new Texture("resources/models/table/LuxuryPoolTable_Metalness_Map.png");
    _metalness_texture->generate();

    _normal_texture = new Texture("resources/models/table/LuxuryPoolTable_Normal_Map.png");
    _normal_texture->generate();

    _roughness_texture = new Texture("resources/models/table/LuxuryPoolTable_Roughness_Map.png");
    _roughness_texture->generate();

    std::vector<Light *> lights = Light::getLights();

    _program->bind();
    _program->setVec3("lightPos", lights[0]->getPosition());
    _program->setVec3("lightColor", lights[0]->getColor());

    _program->setInt("diffuse_texture", 0);
    _program->setInt("metalness_texture", 1);
    _program->setInt("normal_texture", 2);
    _program->setInt("roughness_texture", 3);

    _program->unbind();

    if (_name == "stick") {
        _model_matrix = glm::mat4(1.0f);
        _model_matrix = glm::scale(_model_matrix, glm::vec3(0.025f, 0.025f, 0.025f));
        _model_matrix = glm::rotate(_model_matrix, glm::radians(22.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
        _model_matrix = glm::rotate(_model_matrix, glm::radians(-35.5f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

    }
}

void Model::render() {
    _program->bind();
    _program->setMat4("model", _model_matrix);
    _program->setVec3("cameraPos", Camera::getCurrentCamera()->getPosition());

    for (auto &shape: _shapes) {
        glActiveTexture(GL_TEXTURE0);
        _diffuse_texture->bind();
        glActiveTexture(GL_TEXTURE1);
        _metalness_texture->bind();
        glActiveTexture(GL_TEXTURE2);
        _normal_texture->bind();
        glActiveTexture(GL_TEXTURE3);
        _roughness_texture->bind();
        glBindVertexArray(shape->VAO);
        glDrawArrays(GL_TRIANGLES, 0, shape->count);
        glBindVertexArray(0);
    }

    _program->unbind();
}
