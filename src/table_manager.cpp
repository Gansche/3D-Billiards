//
// Created by Brave Fang on 2023/11/22.
//

#include <windows.h>
#include <unistd.h>
#include "table_manager.h"
#include "shader.h"
#include "canvas.h"

TableManager::TableManager() {
    Init();
}

void TableManager::Init() {
    _timeofthisframe = 0.1;
    Sphere *newSphere;
    newSphere = new Sphere(glm::vec3(-0.5, 0.95, -0.5));
    //newSphere->setPosition(glm::vec3(0.2, 0, -3));
    newSphere->setVelocity(glm::vec3(0.04, 0, 0.04));
    newSphere->setAcceleration(0);
    newSphere->setId(0);
    newSphere->setIfinHole(false);
    _billiards.push_back(newSphere);

    newSphere = new Sphere(glm::vec3(0.5, 0.95, 0.5));
    //newSphere->setPosition(glm::vec3(0, 0, 3));
    newSphere->setVelocity(glm::vec3(-0.04, 0, -0.04));
    newSphere->setAcceleration(0);
    newSphere->setId(1);
    newSphere->setIfinHole(false);
    _billiards.push_back(newSphere);
//
//    Edge * newedge = new Edge();
//    newedge->setStartPos(glm::vec3(0,0.95,1.1));
//    newedge->setEndPos(glm::vec3(1.1,0.95,0));
//    _edges.push_back(newedge);

}

void TableManager::render() {

    for (auto &_billiard: _billiards) {
        _billiard->render();
    }
}

void TableManager::UpdateTable() {
    BallColPair.clear();
    EdgeColPair.clear();
    for (auto &b1: _billiards) {
        if (b1->getIfinHole()) continue;
        for (auto &b2: _billiards) {
            if (b2->getIfinHole()) continue;
            if (b1->getId() == b2->getId()) continue;

            std::pair<Sphere *, Sphere *> origin = {b1, b2};
            std::pair<Sphere *, Sphere *> transponse = {b2, b1};

            if (IfCollisionBall(b1, b2)) {
                std::cout << "y" << std::endl;
                bool flag = true;
                for (auto &p: BallColPair) {
                    if (p == transponse) flag = false;
                }
                if (flag) BallColPair.push_back(origin);
            }
        }
    }

    for (auto &p: BallColPair) {
        BallBallCollision(p.first, p.second);
    }
//
//    for (auto &b: _billiards) {
//        if (b->getIfinHole()) continue;
//        for (auto &e: _edges) {
//            if (IfCollisionEdge(b, e)) {
//                std::pair<Sphere *, Edge *> origin = {b, e};
//                EdgeColPair.push_back(origin);
//            }
//        }
//    }
//
//    for (auto &p: EdgeColPair) {
//        EdgeBallCollision(p.first, p.second);
//    }
////
    for (auto &b: _billiards) {
//        if(b->getIfinHole()) continue;
        b->update(Canvas::getTimeSinceLastFrame());
        std::cout << b->getPosition().x << " " << b->getPosition().z << std::endl;

    }

}

bool TableManager::IfCollisionBall(Sphere *billiard1, Sphere *billiard2) {
    glm::vec3 del = (billiard1->getPosition() - billiard2->getPosition());
    double distance = sqrtf(del.x * del.x + del.y * del.y + del.z * del.z);
    //std::cout << "DEL" << distance << std::endl;
    if (distance < 2 * RADIUS) {
        double backdis = (2 * RADIUS - distance) / 2.0;
        //std::cout << "BD" << backdis << std::endl;
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
    //std::cout << 1 << std::endl;
    double x0 = billiard->getPosition().x;
    double y0 = billiard->getPosition().y;
    double z0 = billiard->getPosition().z;

    double x1 = edge->getStartPos().x;
    double y1 = edge->getStartPos().y;
    double z1 = edge->getStartPos().z;

    double x2 = edge->getEndPos().x;
    double y2 = edge->getEndPos().y;
    double z2 = edge->getEndPos().z;


    double k = (z2 - z1) / (x2 - x1);
    double b = (x2 * z1 - x1 * z2) / (x2 - x1);
    double d = abs((y0 - k * x0 - b) / sqrtf(1 + k * k));

    double xc = (z0 - b + k * x0) / (k * k + 1);
    double yc = y2;
    double zc = k * xc + b;

    glm::vec3 len = edge->getEndPos() - edge->getStartPos();
    glm::vec3 l1 = billiard->getPosition() - edge->getStartPos();
    glm::vec3 l2 = billiard->getPosition() - edge->getEndPos();

    double length = sqrtf(len.x * len.x + len.y * len.y + len.z * len.z);
    double length1 = sqrtf(l1.x * l1.x + l1.y * l1.y + l1.z * l1.z);
    double length2 = sqrtf(l2.x * l2.x + l2.y * l2.y + l2.z * l2.z);

    double maxlen = (length1 > length2) ? length1 : length2;
    double minlen = (length1 < length2) ? length1 : length2;

    if (d <= RADIUS && (maxlen * maxlen <= length * length + minlen * minlen)) {
        double size = (RADIUS - d) / RADIUS;
        glm::vec3 del = glm::vec3((x0 - xc) * size, (y0 - yc) * size, (z0 - zc) * size);
        glm::vec3 newPos = del + billiard->getPosition();
        billiard->setPosition(newPos);
        return true;
    }
    return false;
}


bool TableManager::IfCollisionHole(Sphere *billiard, Hole *hole) {
    glm::vec3 del = (billiard->getPosition() - hole->getPosition());
    double distance = sqrtf(del.x * del.x + del.y * del.y + del.z * del.z);
    if (distance < RADIUS + hole->getRadius()) {
        billiard->setIfinHole(true);
        return true;
    } else
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
    std::cout << 1 << std::endl;
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

    double k = (z2 - z1) / (x2 - x1);

    double vxn = (2 * k * vz0 - (k * k - 1) * vx0) / (k * k + 1);
    double vzn = (2 * k * vx0 + (k * k - 1) * vz0) / (k * k + 1);



    billiard->setVelocity(glm::vec3(vxn, vy0, vzn));
    return;
}

TableManager::~TableManager() = default;