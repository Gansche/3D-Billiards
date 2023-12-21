//
// Created by Brave Fang on 2023/11/22.
//

#ifndef BILLIARDS_TABLE_MANAGER_H
#define BILLIARDS_TABLE_MANAGER_H

#include <vector>
#include <utility>
#include "defs.h"
#include "object.h"

class Edge {
public:
    Edge() = default;
    Edge(glm::vec3 StartPos, glm::vec3 EndPos) : start_position(StartPos), end_position(EndPos){}
    ~Edge() = default;

    glm::vec3 getStartPos() { return start_position; }
    glm::vec3 getEndPos() { return end_position; }

    virtual glm::vec3 setStartPos(glm::vec3 new_start_position) { return start_position = new_start_position; }
    virtual glm::vec3 setEndPos(glm::vec3 new_end_position) { return end_position = new_end_position; }

private:
    glm::vec3 start_position;
    glm::vec3 end_position;
};

class Hole {
public:
    Hole() = default;
    Hole(glm::vec3 Pos, double R) : position(Pos), radius(R){}
    ~Hole() = default;

    glm::vec3 getPosition() { return position; }
    double getRadius() const { return radius; }

    virtual glm::vec3 setPos(glm::vec3 new_position) { return position = new_position; }
    virtual double setRadius(double R) { return radius = R; }

private:
    glm::vec3 position;
    double radius;
};

class TableManager {
public:
    TableManager();

    void render();

    void UpdateTable();

    void Init();

    void BallBallCollision(Sphere* billiard1, Sphere* billiard2);

    void EdgeBallCollision(Sphere* billiard, Edge* edge);

    bool IfCollisionBall(Sphere* billiard1, Sphere* billiard2);

    bool IfCollisionEdge(Sphere* billiard, Edge* edge);

    bool IfCollisionHole(Sphere* billiard, Hole* hole);

    ~TableManager();

    std::vector<std::pair<Sphere*, Sphere*>> BallColPair;
    std::vector<std::pair<Sphere*, Edge*>> EdgeColPair;

private:
    std::vector<Sphere *> _billiards{};
    std::vector<Edge *> _edges{};
    std::vector<Hole *> _holes{};

};

#endif //BILLIARDS_TABLE_MANAGER_H
