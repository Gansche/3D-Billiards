//
// Created by Brave Fang on 2023/12/14.
//

#ifndef BILLIARDS_OBJ_LOADER_H
#define BILLIARDS_OBJ_LOADER_H

#include <vector>
#include <iostream>

struct Index {
    int vi;
    int vn;
    int vt;
};

struct Mesh {
    std::vector<Index> indices;
};

struct Vertices {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texcoords;
};

bool fixIndex(int idx, int n, int *ret, bool allow_zero);

float parseReal(const char **token);

bool parseTriple(const char **token, int vsize, int vnsize, int vtsize, Index *ret);

bool load(Vertices *vertices, Mesh *mesh, const char *filename);


#endif  //BILLIARDS_OBJ_LOADER_H