//
// Created by Brave Fang on 2023/11/22.
//

#include <windows.h>
#include "table_manager.h"
#include "shader.h"
#include "defs.h"
#include "canvas.h"

double max(double a, double b) {
    return (a > b) ? a : b;
}

double min(double a, double b) {
    return (a < b) ? a : b;
}


TableManager::TableManager() {
    Init();
}

void TableManager::Init() {
    double a = ACCLAR;
    Sphere *newSphere;

    newSphere = new Sphere("cue", glm::vec3(0.0, 0.95, 0.5));
//    newSphere = new Sphere("cue", glm::vec3(0.0, 0.0, 0.0));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("solid_1", glm::vec3(0.0, 0.95, -0.7));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);


    newSphere = new Sphere("solid_3", glm::vec3(0.1, 0.95, -0.7));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("solid_2", glm::vec3(-0.1, 0.95, -0.7));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("stripe_15", glm::vec3(0.2, 0.95, -0.7));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("stripe_13", glm::vec3(-0.2, 0.95, -0.7));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    double delta = RADIUS * sqrtf(3);

    newSphere = new Sphere("solid_4", glm::vec3(0.05, 0.95, -0.7 + delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("stripe_14", glm::vec3(-0.05, 0.95, -0.7 + delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("stripe_12", glm::vec3(0.15, 0.95, -0.7 + delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("stripe_10", glm::vec3(-0.15, 0.95, -0.7 + delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("solid_8", glm::vec3(0, 0.95, -0.7 + 2 * delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("solid_5", glm::vec3(0.1, 0.95, -0.7 + 2 * delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("stripe_9", glm::vec3(-0.1, 0.95, -0.7 + 2 * delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("stripe_11", glm::vec3(0.05, 0.95, -0.7 + 3 * delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("solid_6", glm::vec3(-0.05, 0.95, -0.7 + 3 * delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere("solid_7", glm::vec3(0, 0.95, -0.7 + 4 * delta));
    newSphere->setVelocity(glm::vec3(0, 0, 0));
    newSphere->setAcceleration(a);
    newSphere->setAngularVelocity(0.0);
    newSphere->setAngle(0.0);
    newSphere->setIfInHole(false);
    _billiards.push_back(newSphere);

    Edge *newEdge = new Edge();
    newEdge->setStartPos(glm::vec3(-0.52, 0.95, -1.255));
    newEdge->setEndPos(glm::vec3(0.52, 0.95, -1.255));
    _edges.push_back(newEdge);

    newEdge = new Edge();
    newEdge->setStartPos(glm::vec3(-0.52, 0.95, 1.255));
    newEdge->setEndPos(glm::vec3(0.52, 0.95, 1.255));
    _edges.push_back(newEdge);

    newEdge = new Edge();
    newEdge->setStartPos(glm::vec3(-0.61, 0.95, -0.075));
    newEdge->setEndPos(glm::vec3(-0.61, 0.95, -1.18));
    _edges.push_back(newEdge);

    newEdge = new Edge();
    newEdge->setStartPos(glm::vec3(-0.61, 0.95, 0.075));
    newEdge->setEndPos(glm::vec3(-0.61, 0.95, 1.18));
    _edges.push_back(newEdge);

    newEdge = new Edge();
    newEdge->setStartPos(glm::vec3(0.61, 0.95, -0.075));
    newEdge->setEndPos(glm::vec3(0.61, 0.95, -1.18));
    _edges.push_back(newEdge);

    newEdge = new Edge();
    newEdge->setStartPos(glm::vec3(0.61, 0.95, 0.075));
    newEdge->setEndPos(glm::vec3(0.61, 0.95, 1.18));
    _edges.push_back(newEdge);

    Hole *newHole = new Hole();
    double R = 0.09;
    newHole->setRadius(R);
    newHole->setPos(glm::vec3(0.615, 0.95, 0));
    _holes.push_back(newHole);

    newHole = new Hole();
    newHole->setRadius(R);
    newHole->setPos(glm::vec3(-0.615, 0.95, 0));
    _holes.push_back(newHole);

    newHole = new Hole();
    newHole->setRadius(R);
    newHole->setPos(glm::vec3(0.615, 0.95, 1.265));
    _holes.push_back(newHole);

    newHole = new Hole();
    newHole->setRadius(R);
    newHole->setPos(glm::vec3(0.615, 0.95, -1.265));
    _holes.push_back(newHole);

    newHole = new Hole();
    newHole->setRadius(R);
    newHole->setPos(glm::vec3(-0.615, 0.95, 1.265));
    _holes.push_back(newHole);

    newHole = new Hole();
    newHole->setRadius(R);
    newHole->setPos(glm::vec3(-0.615, 0.95, -1.265));
    _holes.push_back(newHole);
}

void TableManager::render() {
    for (auto &_billiard: _billiards) {
        if (_billiard->getIfInHole())
            continue;
        _billiard->render();
    }
}

void TableManager::UpdateTable() {
    BallColPair.clear();
    EdgeColPair.clear();
    for (auto &b: _billiards) {
        glm::vec3 pos = b->getPosition();
        if (pos.x > 0.675 || pos.x < -0.675 || pos.z > 1.33 || pos.z < -1.33)
            b->setIfInHole(true);
    }

    for (auto &b: _billiards) {
        if (b->getIfInHole()) continue;
        for (auto &h: _holes) {
            if (IfCollisionHole(b, h))
                b->setIfInHole(true);
        }
    }
    for (auto &b1: _billiards) {
        if (b1->getIfInHole()) continue;
        for (auto &b2: _billiards) {
            if (b2->getIfInHole()) continue;
            if (b1->getName() == b2->getName()) continue;

            std::pair<Sphere *, Sphere *> origin = {b1, b2};
            std::pair<Sphere *, Sphere *> transpose = {b2, b1};

            if (IfCollisionBall(b1, b2)) {
                bool flag = true;
                for (auto &p: BallColPair) {
                    if (p == transpose) flag = false;
                }
                if (flag) BallColPair.push_back(origin);
            }
        }
    }

    for (auto &p: BallColPair) {
        BallBallCollision(p.first, p.second);
    }

    for (auto &b: _billiards) {
        if (b->getIfInHole()) continue;
        for (auto &e: _edges) {
            if (IfCollisionEdge(b, e)) {
                std::pair<Sphere *, Edge *> origin = {b, e};
                EdgeColPair.push_back(origin);
            }
        }
    }

    for (auto &p: EdgeColPair) {
        EdgeBallCollision(p.first, p.second);
    }
    for (auto &b: _billiards) {
        b->update(Canvas::getTimeSinceLastFrame());
    }
}

bool TableManager::IfCollisionHole(Sphere *billiard, Hole *hole) {
    glm::vec3 del = billiard->getPosition() - hole->getPosition();
    double distance = sqrtf(del.x * del.x + del.y * del.y + del.z * del.z);
    if (distance < hole->getRadius())
        return true;
    return false;
}

bool TableManager::IfCollisionBall(Sphere *billiard1, Sphere *billiard2) {
    glm::vec3 del = (billiard1->getPosition() - billiard2->getPosition());
    double distance = sqrtf(del.x * del.x + del.y * del.y + del.z * del.z);
    if (distance <= 2 * RADIUS) {
        double backdis = (2 * RADIUS - distance) / 2.0;
        double b1_x = billiard1->getPosition().x + backdis * del.x / distance;
        double b1_y = billiard1->getPosition().y;
        double b1_z = billiard1->getPosition().z + backdis * del.z / distance;
        billiard1->setPosition(glm::vec3(b1_x, b1_y, b1_z));

        double b2_x = billiard2->getPosition().x - backdis * del.x / distance;
        double b2_y = billiard2->getPosition().y;
        double b2_z = billiard2->getPosition().z - backdis * del.z / distance;
        billiard2->setPosition(glm::vec3(b2_x, b2_y, b2_z));
        return true;
    } else
        return false;
}

bool TableManager::IfCollisionEdge(Sphere *billiard, Edge *edge) {
    double x0 = billiard->getPosition().x;
    double y0 = billiard->getPosition().y;
    double z0 = billiard->getPosition().z;

    double x1 = edge->getStartPos().x;
    double y1 = edge->getStartPos().y;
    double z1 = edge->getStartPos().z;

    double x2 = edge->getEndPos().x;
    double y2 = edge->getEndPos().y;
    double z2 = edge->getEndPos().z;

    double d;
    if (x2 == x1) {
        d = abs(x0 - x1);
        if ((d <= RADIUS) && (z0 <= max(z1, z2)) && (z0 >= min(z1, z2))) {
            double dx = RADIUS - d;
            double newx;
            if (x0 > x1)
                newx = x0 + dx;
            else
                newx = x0 - dx;
            glm::vec3 newPos = glm::vec3(newx, y0, z0);
            billiard->setPosition(newPos);
            return true;
        }
    } else {
        d = abs(z0 - z1);
        if ((d <= RADIUS) && (x0 <= max(x1, x2)) && (x0 >= min(x1, x2))) {
            double dz = RADIUS - d;
            double newz;
            if (z0 > z1)
                newz = z0 + dz;
            else
                newz = z0 - dz;
            glm::vec3 newPos = glm::vec3(x0, y0, newz);
            billiard->setPosition(newPos);
            return true;
        }
    }
    return false;
}

void TableManager::BallBallCollision(Sphere *billiard1, Sphere *billiard2) {
    double b1_x = billiard1->getPosition().x;
    double b1_y = billiard1->getPosition().y;
    double b1_z = billiard1->getPosition().z;

    double v1_x = billiard1->getVelocity().x;
    double v1_y = billiard1->getVelocity().y;
    double v1_z = billiard1->getVelocity().z;

    double b2_x = billiard2->getPosition().x;
    double b2_y = billiard2->getPosition().y;
    double b2_z = billiard2->getPosition().z;

    double v2_x = billiard2->getVelocity().x;
    double v2_y = billiard2->getVelocity().y;
    double v2_z = billiard2->getVelocity().z;

    double distance = sqrtf((b1_z - b2_z) * (b1_z - b2_z) + (b1_x - b2_x) * (b1_x - b2_x));
    double nx = (b2_x - b1_x) / distance;
    double nz = (b2_z - b1_z) / distance;
    double dvx = v1_x - v2_x;
    double dvz = v1_z - v2_z;
    double k = nx * dvx + nz * dvz;

    v1_x -= k * nx;
    v1_z -= k * nz;
    v2_x += k * nx;
    v2_z += k * nz;

    billiard1->setVelocity(glm::vec3(v1_x, v1_y, v1_z));
    billiard2->setVelocity(glm::vec3(v2_x, v2_y, v2_z));
}

void TableManager::EdgeBallCollision(Sphere *billiard, Edge *edge) {
    double x0 = billiard->getPosition().x;
    double y0 = billiard->getPosition().y;
    double z0 = billiard->getPosition().z;

    double vx0 = billiard->getVelocity().x;
    double vy0 = billiard->getVelocity().y;
    double vz0 = billiard->getVelocity().z;

    double x1 = edge->getStartPos().x;
    double y1 = edge->getStartPos().y;
    double z1 = edge->getStartPos().z;

    double x2 = edge->getEndPos().x;
    double y2 = edge->getEndPos().y;
    double z2 = edge->getEndPos().z;

    if (x1 == x2) {
        vx0 = -vx0;
        vz0 = vz0;
    }
    if (z1 == z2) {
        vx0 = vx0;
        vz0 = -vz0;
    }
    billiard->setVelocity(glm::vec3(vx0, vy0, vz0));
}

TableManager::~TableManager() = default;