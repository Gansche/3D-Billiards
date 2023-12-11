//
// Created by Brave Fang on 2023/11/22.
//

#include "object.h"
#include "canvas.h"
#include "shader.h"
#include "texture.h"
#include "defs.h"
#include "camera.h"

/* object */
Object::Object() {

}

Object::Object(glm::vec3 position, glm::vec3 direction) :
        _position(position), _direction(direction) {

}

Object::~Object() = default;


Sphere::Sphere(Program *program) : _program(program) {
    initialize();
}

//todo:debug
void Sphere::render() {
    _program->bind();
    _program->setMat4("model", _model_matrix);
    _program->setVec3("viewPos",Camera::getCurrentCamera()->getPosition());

    glBindVertexArray(VAO);
    // Draw the triangle !
    //glDrawArrays(GL_TRIANGLES, 0, 6); // 3 indices starting at 0 -> 1 triangle

    // could be modified to use GL_TRIANGLE_FAN and GL_TRIANGLE_STRIP
    // to draw the poles and the rest of the sphere,
    // but this will be 2 draw call. Should we??? (draw cal optimizations and such things...)
    /**/glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_INT,   // type
            (void *) 0           // element array buffer offset
    );//*/
    glBindVertexArray(0);
    _program->unbind();
}

void Sphere::initialize() {
    _view_matrix = Camera::getCurrentCamera()->getViewMatrix();
    _projection_matrix = Canvas::getProjectionMatrix();
    _model_matrix = glm::mat4(1.0f);

    _program->bind();
    _program->setMat4("projection", _projection_matrix);
    _program->setMat4("view", _view_matrix);
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
            vertices.push_back(v);
            //indices.push_back(indicator);
            indicator++;

            v.n = glm::vec3(yPolar * x0, yPolar * y0, xPolar);
            v.v = v.n * RADIUS;
            v.u = glm::vec2((float) i / LATITUDE, (float) j / LONGITUDE);
            vertices.push_back(v);
            //indices.push_back(indicator);
            indicator++;

            indices.push_back(indicator - 3);
            indices.push_back(indicator);
            indices.push_back(indicator - 1);

            indices.push_back(indicator - 2);
            indices.push_back(indicator);
            indices.push_back(indicator - 3);

        }
    }

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(4, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // 1rst attribute buffer : vertices

    glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            sizeof(Vertex),     // stride
            (void *) 0            // array buffer offset
    );
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            sizeof(Vertex),                   // stride
            (void *) reinterpret_cast<void *>(offsetof(Vertex, c))                          // array buffer offset
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(
            2,                                // attribute. No particular reason for 2, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            sizeof(Vertex),                   // stride
            (void *) reinterpret_cast<void *>(offsetof(Vertex, n))                          // array buffer offset
    );
    glEnableVertexAttribArray(2);

}