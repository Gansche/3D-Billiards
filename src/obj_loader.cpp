//
// Created by Brave Fang on 2023/12/29.
//

#include "obj_loader.h"

#include <fstream>
#include <sstream>
#include <cstring>
#include <cassert>
#include "defs.h"

bool fixIndex(int idx, int n, int *ret, bool allow_zero) {
    if (!ret)
        return false;
    if (idx > 0) {
        (*ret) = idx - 1;
        return true;
    }
    if (idx == 0) {
        (*ret) = idx - 1;
        return allow_zero;
    }
    if (idx < 0) {
        (*ret) = n + idx;  // negative value = relative
        if ((*ret) < 0) {
            return false;  // invalid relative index
        }
        return true;
    }
    return false;
}

float parseReal(const char **token) {
    (*token) += strspn((*token), " \t");
    const char *end = (*token) + strcspn((*token), " \t\r");
    float val = std::stof(std::string(*token));
    (*token) = end;
    return val;
}

bool parseTriple(const char **token, int vsize, int vnsize, int vtsize,
                 Index *ret) {
    if (!ret)return false;
    Index vi;
    if (!fixIndex(atoi((*token)), vsize, &vi.vi, false)) {
        return false;
    }
    (*token) += strcspn((*token), "/ \t\r");
    if ((*token)[0] != '/') {
        (*ret) = vi;
        return true;
    }
    (*token)++;
    // i//k
    if ((*token)[0] == '/') {
        (*token)++;
        if (!fixIndex(atoi((*token)), vnsize, &vi.vn, true)) {
            return false;
        }
        (*token) += strcspn((*token), "/ \t\r");
        (*ret) = vi;
        return true;
    }
    // i/j/k or i/j
    if (!fixIndex(atoi((*token)), vtsize, &vi.vt, true)) {
        return false;
    }
    (*token) += strcspn((*token), "/ \t\r");
    if ((*token)[0] != '/') {
        (*ret) = vi;
        return true;
    }
    // i/j/k
    (*token)++;  // skip '/'
    if (!fixIndex(atoi((*token)), vnsize, &vi.vn, true)) {
        return false;
    }
    (*token) += strcspn((*token), "/ \t\r");
    (*ret) = vi;
    return true;
}

bool load(Vertices *vertices, Mesh *mesh, const char *filename) {
    std::ifstream ifs(filename);
    std::string name;

    std::string line;
    while (std::getline(ifs, line)) {
        // Skip if empty line.
        if (line.empty()) continue;

        // Trim newline '\r\n' or '\n'
        if (!line.empty()) {
            if (line[line.size() - 1] == '\n')
                line.erase(line.size() - 1);
        }
        if (!line.empty()) {
            if (line[line.size() - 1] == '\r')
                line.erase(line.size() - 1);
        }

        // Skip leading space.
        const char *token = line.c_str();
        token += strspn(token, " \t");

        assert(token);
        if (token[0] == '\0') continue;  // empty line

        if (token[0] == '#') continue;  // comment line

        // vertex
        if (token[0] == 'v' && IS_SPACE((token[1]))) {
            token += 2;
            float x, y, z;
            x = parseReal(&token);
            y = parseReal(&token);
            z = parseReal(&token);
            vertices->vertices.push_back(x);
            vertices->vertices.push_back(y);
            vertices->vertices.push_back(z);
            continue;
        }
        // normal
        if (token[0] == 'v' && token[1] == 'n' && IS_SPACE((token[2]))) {
            token += 3;
            float x, y, z;
            x = parseReal(&token);
            y = parseReal(&token);
            z = parseReal(&token);
            vertices->normals.push_back(x);
            vertices->normals.push_back(y);
            vertices->normals.push_back(z);
            continue;
        }
        // texcoord
        if (token[0] == 'v' && token[1] == 't' && IS_SPACE((token[2]))) {
            token += 3;
            float x, y;
            x = parseReal(&token);
            y = parseReal(&token);
            vertices->texcoords.push_back(x);
            vertices->texcoords.push_back(y);
            continue;
        }
        // face
        if (token[0] == 'f' && IS_SPACE((token[1]))) {
            token += 2;
            token += strspn(token, " \t");

            Mesh face;

            while (!IS_NEW_LINE(token[0])) {
                Index vi;
                parseTriple(&token, static_cast<int>(vertices->vertices.size() / 3),
                            static_cast<int>(vertices->normals.size() / 3),
                            static_cast<int>(vertices->texcoords.size() / 2), &vi);

                face.indices.push_back(vi);
                size_t n = strspn(token, " \t\r");
                token += n;
            }

            if (face.indices.size() == 3) {
                mesh->indices.push_back(face.indices[0]);
                mesh->indices.push_back(face.indices[1]);
                mesh->indices.push_back(face.indices[2]);
            } else if (face.indices.size() == 4) {
                mesh->indices.push_back(face.indices[0]);
                mesh->indices.push_back(face.indices[1]);
                mesh->indices.push_back(face.indices[2]);

                mesh->indices.push_back(face.indices[0]);
                mesh->indices.push_back(face.indices[2]);
                mesh->indices.push_back(face.indices[3]);
            } else if (face.indices.size() == 5) {
                mesh->indices.push_back(face.indices[0]);
                mesh->indices.push_back(face.indices[1]);
                mesh->indices.push_back(face.indices[2]);

                mesh->indices.push_back(face.indices[0]);
                mesh->indices.push_back(face.indices[2]);
                mesh->indices.push_back(face.indices[3]);

                mesh->indices.push_back(face.indices[0]);
                mesh->indices.push_back(face.indices[3]);
                mesh->indices.push_back(face.indices[4]);
            }
            continue;
        }
    }
    return true;
}