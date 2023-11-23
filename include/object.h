//
// Created by Brave Fang on 2023/11/22.
//

#ifndef BILLIARDS_OBJECT_H
#define BILLIARDS_OBJECT_H

#include <glm/glm.hpp>

class Shader;

class Texture;

class Object {
public:
    Object();

    virtual void render() = 0;

    ~Object();

protected:
    Shader *_shader;
    Texture *_texture;
};

class Sphere : public Object {
public:
    explicit Sphere(glm::vec3 center);

    void render() override;

    ~Sphere();

private:
    glm::vec3 _center;

};

#endif //BILLIARDS_OBJECT_H
