#ifndef TINY_OBJ_LOADER_H_
#define TINY_OBJ_LOADER_H_

typedef float real_t;

struct index_t {
    int vertex_index;
    int normal_index;
    int texcoord_index;
};

struct mesh_t {
    std::vector<index_t> indices;
    std::vector<unsigned char> num_face_vertices;   // The number of _vertices per
};

struct shape_t {
    std::string name;
    mesh_t mesh;
};

struct attrib_t {
    std::vector<real_t> vertices;
    std::vector<real_t> normals;
    std::vector<real_t> texcoords;
};


bool LoadObj(attrib_t *attrib, std::vector<shape_t> *shapes, const char *filename, bool triangulate = true);

bool LoadObj(attrib_t *attrib, std::vector<shape_t> *shapes, std::istream *inStream, bool triangulate = true);


#endif  // TINY_OBJ_LOADER_H_

#include <fstream>
#include <sstream>


struct vertex_index_t {
    int v_idx, vt_idx, vn_idx;

    vertex_index_t() : v_idx(-1), vt_idx(-1), vn_idx(-1) {}

    explicit vertex_index_t(int idx) : v_idx(idx), vt_idx(idx), vn_idx(idx) {}

    vertex_index_t(int vidx, int vtidx, int vnidx)
            : v_idx(vidx), vt_idx(vtidx), vn_idx(vnidx) {}
};

// Internal data structure for face representation
// index + smoothing group.
struct face_t {
    std::vector<vertex_index_t> vertex_indices;  // face vertex _indices.
};

struct PrimGroup {
    std::vector<face_t> faceGroup;

    void clear() {
        faceGroup.clear();
    }

    bool IsEmpty() const {
        return faceGroup.empty();
    }

};

static std::istream &safeGetline(std::istream &is, std::string &t) {
    t.clear();

    std::istream::sentry se(is, true);
    std::streambuf *sb = is.rdbuf();

    if (se) {
        for (;;) {
            int c = sb->sbumpc();
            switch (c) {
                case '\n':
                    return is;
                case '\r':
                    if (sb->sgetc() == '\n') sb->sbumpc();
                    return is;
                case EOF:
                    // Also handle the case when the last line has no line ending
                    if (t.empty()) is.setstate(std::ios::eofbit);
                    return is;
                default:
                    t += static_cast<char>(c);
            }
        }
    }

    return is;
}

#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t'))
#define IS_DIGIT(x) \
  (static_cast<unsigned int>((x) - '0') < static_cast<unsigned int>(10))
#define IS_NEW_LINE(x) (((x) == '\r') || ((x) == '\n') || ((x) == '\0'))

// Make index zero-base, and also support relative index.
static inline bool fixIndex(int idx, int n, int *ret, bool allow_zero) {
    if (!ret) {
        return false;
    }

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

    return false;  // never reach here.
}

static bool tryParseDouble(const char *s, const char *s_end, double *result) {
    if (s >= s_end) {
        return false;
    }

    double mantissa = 0.0;

    int exponent = 0;

    // NOTE: THESE MUST BE DECLARED HERE SINCE WE ARE NOT ALLOWED
    // TO JUMP OVER DEFINITIONS.
    char sign = '+';
    char exp_sign = '+';
    char const *curr = s;

    // How many characters were read in a loop.
    int read = 0;
    // Tells whether a loop terminated due to reaching s_end.
    bool end_not_reached = false;
    bool leading_decimal_dots = false;

    /*
            BEGIN PARSING.
    */

    // Find out what sign we've got.
    if (*curr == '+' || *curr == '-') {
        sign = *curr;
        curr++;
        if ((curr != s_end) && (*curr == '.')) {
            // accept. Somethig like `.7e+2`, `-.5234`
            leading_decimal_dots = true;
        }
    } else if (IS_DIGIT(*curr)) { /* Pass through. */
    } else if (*curr == '.') {
        // accept. Somethig like `.7e+2`, `-.5234`
        leading_decimal_dots = true;
    } else {
        goto fail;
    }

    // Read the integer part.
    end_not_reached = (curr != s_end);
    if (!leading_decimal_dots) {
        while (end_not_reached && IS_DIGIT(*curr)) {
            mantissa *= 10;
            mantissa += static_cast<int>(*curr - 0x30);
            curr++;
            read++;
            end_not_reached = (curr != s_end);
        }

        // We must make sure we actually got something.
        if (read == 0) goto fail;
    }

    // We allow numbers of form "#", "###" etc.
    if (!end_not_reached) goto assemble;

    // Read the decimal part.
    if (*curr == '.') {
        curr++;
        read = 1;
        end_not_reached = (curr != s_end);
        while (end_not_reached && IS_DIGIT(*curr)) {
            static const double pow_lut[] = {
                    1.0, 0.1, 0.01, 0.001, 0.0001, 0.00001, 0.000001, 0.0000001,
            };
            const int lut_entries = sizeof pow_lut / sizeof pow_lut[0];

            // NOTE: Don't use powf here, it will absolutely murder precision.
            mantissa += static_cast<int>(*curr - 0x30) *
                        (read < lut_entries ? pow_lut[read] : std::pow(10.0, -read));
            read++;
            curr++;
            end_not_reached = (curr != s_end);
        }
    } else if (*curr == 'e' || *curr == 'E') {
    } else {
        goto assemble;
    }

    if (!end_not_reached) goto assemble;

    // Read the exponent part.
    if (*curr == 'e' || *curr == 'E') {
        curr++;
        // Figure out if a sign is present and if it is.
        end_not_reached = (curr != s_end);
        if (end_not_reached && (*curr == '+' || *curr == '-')) {
            exp_sign = *curr;
            curr++;
        } else if (IS_DIGIT(*curr)) { /* Pass through. */
        } else {
            // Empty E is not allowed.
            goto fail;
        }

        read = 0;
        end_not_reached = (curr != s_end);
        while (end_not_reached && IS_DIGIT(*curr)) {
            // To avoid annoying MSVC's min/max macro definiton,
            // Use hardcoded int max value
            if (exponent > (2147483647 / 10)) { // 2147483647 = std::numeric_limits<int>::max()
                // Integer overflow
                goto fail;
            }
            exponent *= 10;
            exponent += static_cast<int>(*curr - 0x30);
            curr++;
            read++;
            end_not_reached = (curr != s_end);
        }
        exponent *= (exp_sign == '+' ? 1 : -1);
        if (read == 0) goto fail;
    }

    assemble:
    *result = (sign == '+' ? 1 : -1) *
              (exponent ? std::ldexp(mantissa * std::pow(5.0, exponent), exponent)
                        : mantissa);
    return true;
    fail:
    return false;
}

static inline real_t parseReal(const char **token, double default_value = 0.0) {
    (*token) += strspn((*token), " \t");
    const char *end = (*token) + strcspn((*token), " \t\r");
    double val = default_value;
    tryParseDouble((*token), end, &val);
    real_t f = static_cast<real_t>(val);
    (*token) = end;
    return f;
}

static inline void parseReal2(real_t *x, real_t *y, const char **token,
                              const double default_x = 0.0,
                              const double default_y = 0.0) {
    (*x) = parseReal(token, default_x);
    (*y) = parseReal(token, default_y);
}

static inline void parseReal3(real_t *x, real_t *y, real_t *z,
                              const char **token, const double default_x = 0.0,
                              const double default_y = 0.0,
                              const double default_z = 0.0) {
    (*x) = parseReal(token, default_x);
    (*y) = parseReal(token, default_y);
    (*z) = parseReal(token, default_z);
}

// Parse triples with index offsets: i, i/j/k, i//k, i/j
static bool parseTriple(const char **token, int vsize, int vnsize, int vtsize,
                        vertex_index_t *ret) {
    if (!ret) {
        return false;
    }

    vertex_index_t vi(-1);

    if (!fixIndex(atoi((*token)), vsize, &vi.v_idx, false)) {
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
        if (!fixIndex(atoi((*token)), vnsize, &vi.vn_idx, true)) {
            return false;
        }
        (*token) += strcspn((*token), "/ \t\r");
        (*ret) = vi;
        return true;
    }

    // i/j/k or i/j
    if (!fixIndex(atoi((*token)), vtsize, &vi.vt_idx, true)) {
        return false;
    }

    (*token) += strcspn((*token), "/ \t\r");
    if ((*token)[0] != '/') {
        (*ret) = vi;
        return true;
    }

    // i/j/k
    (*token)++;  // skip '/'
    if (!fixIndex(atoi((*token)), vnsize, &vi.vn_idx, true)) {
        return false;
    }
    (*token) += strcspn((*token), "/ \t\r");

    (*ret) = vi;

    return true;
}

template<typename T>
static int pnpoly(int nvert, T *vertx, T *verty, T testx, T testy) {
    int i, j, c = 0;
    for (i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((verty[i] > testy) != (verty[j] > testy)) &&
            (testx <
             (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) +
             vertx[i]))
            c = !c;
    }
    return c;
}

struct TinyObjPoint {
    real_t x, y, z;

    TinyObjPoint() : x(0), y(0), z(0) {}

};

inline real_t dot(const TinyObjPoint &v1, const TinyObjPoint &v2) {
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

static bool exportGroupsToShape(shape_t *shape, const PrimGroup &prim_group, const std::string &name,
                                bool triangulate, const std::vector<real_t> &v) {
    if (prim_group.IsEmpty()) {
        return false;
    }

    shape->name = name;

    // polygon
    if (!prim_group.faceGroup.empty()) {
        // Flatten _vertices and _indices
        for (size_t i = 0; i < prim_group.faceGroup.size(); i++) {
            const face_t &face = prim_group.faceGroup[i];

            size_t npolys = face.vertex_indices.size();

            if (npolys < 3) {
                // Face must have 3+ _vertices.
                continue;
            }

            if (triangulate && npolys != 3) {
                if (npolys == 4) {
                    vertex_index_t i0 = face.vertex_indices[0];
                    vertex_index_t i1 = face.vertex_indices[1];
                    vertex_index_t i2 = face.vertex_indices[2];
                    vertex_index_t i3 = face.vertex_indices[3];

                    size_t vi0 = size_t(i0.v_idx);
                    size_t vi1 = size_t(i1.v_idx);
                    size_t vi2 = size_t(i2.v_idx);
                    size_t vi3 = size_t(i3.v_idx);

                    if (((3 * vi0 + 2) >= v.size()) || ((3 * vi1 + 2) >= v.size()) ||
                        ((3 * vi2 + 2) >= v.size()) || ((3 * vi3 + 2) >= v.size())) {
                        // Invalid triangle.
                        // FIXME(syoyo): Is it ok to simply skip this invalid triangle?
                        continue;
                    }

                    real_t v0x = v[vi0 * 3 + 0];
                    real_t v0y = v[vi0 * 3 + 1];
                    real_t v0z = v[vi0 * 3 + 2];
                    real_t v1x = v[vi1 * 3 + 0];
                    real_t v1y = v[vi1 * 3 + 1];
                    real_t v1z = v[vi1 * 3 + 2];
                    real_t v2x = v[vi2 * 3 + 0];
                    real_t v2y = v[vi2 * 3 + 1];
                    real_t v2z = v[vi2 * 3 + 2];
                    real_t v3x = v[vi3 * 3 + 0];
                    real_t v3y = v[vi3 * 3 + 1];
                    real_t v3z = v[vi3 * 3 + 2];

                    real_t e02x = v2x - v0x;
                    real_t e02y = v2y - v0y;
                    real_t e02z = v2z - v0z;
                    real_t e13x = v3x - v1x;
                    real_t e13y = v3y - v1y;
                    real_t e13z = v3z - v1z;

                    real_t sqr02 = e02x * e02x + e02y * e02y + e02z * e02z;
                    real_t sqr13 = e13x * e13x + e13y * e13y + e13z * e13z;

                    index_t idx0, idx1, idx2, idx3;

                    idx0.vertex_index = i0.v_idx;
                    idx0.normal_index = i0.vn_idx;
                    idx0.texcoord_index = i0.vt_idx;
                    idx1.vertex_index = i1.v_idx;
                    idx1.normal_index = i1.vn_idx;
                    idx1.texcoord_index = i1.vt_idx;
                    idx2.vertex_index = i2.v_idx;
                    idx2.normal_index = i2.vn_idx;
                    idx2.texcoord_index = i2.vt_idx;
                    idx3.vertex_index = i3.v_idx;
                    idx3.normal_index = i3.vn_idx;
                    idx3.texcoord_index = i3.vt_idx;

                    if (sqr02 < sqr13) {
                        // [0, 1, 2], [0, 2, 3]
                        shape->mesh.indices.push_back(idx0);
                        shape->mesh.indices.push_back(idx1);
                        shape->mesh.indices.push_back(idx2);

                        shape->mesh.indices.push_back(idx0);
                        shape->mesh.indices.push_back(idx2);
                        shape->mesh.indices.push_back(idx3);
                    } else {
                        // [0, 1, 3], [1, 2, 3]
                        shape->mesh.indices.push_back(idx0);
                        shape->mesh.indices.push_back(idx1);
                        shape->mesh.indices.push_back(idx3);

                        shape->mesh.indices.push_back(idx1);
                        shape->mesh.indices.push_back(idx2);
                        shape->mesh.indices.push_back(idx3);
                    }

                    // Two triangle faces
                    shape->mesh.num_face_vertices.push_back(3);
                    shape->mesh.num_face_vertices.push_back(3);

//                        shape->mesh.smoothing_group_ids.push_back(face.smoothing_group_id);
//                        shape->mesh.smoothing_group_ids.push_back(face.smoothing_group_id);

                } else {
                    vertex_index_t i0 = face.vertex_indices[0];
                    vertex_index_t i1(-1);
                    vertex_index_t i2 = face.vertex_indices[1];

                    // find the two axes to work in
                    size_t axes[2] = {1, 2};
                    for (size_t k = 0; k < npolys; ++k) {
                        i0 = face.vertex_indices[(k + 0) % npolys];
                        i1 = face.vertex_indices[(k + 1) % npolys];
                        i2 = face.vertex_indices[(k + 2) % npolys];
                        size_t vi0 = size_t(i0.v_idx);
                        size_t vi1 = size_t(i1.v_idx);
                        size_t vi2 = size_t(i2.v_idx);

                        if (((3 * vi0 + 2) >= v.size()) || ((3 * vi1 + 2) >= v.size()) ||
                            ((3 * vi2 + 2) >= v.size())) {
                            // Invalid triangle.
                            // FIXME(syoyo): Is it ok to simply skip this invalid triangle?
                            continue;
                        }
                        real_t v0x = v[vi0 * 3 + 0];
                        real_t v0y = v[vi0 * 3 + 1];
                        real_t v0z = v[vi0 * 3 + 2];
                        real_t v1x = v[vi1 * 3 + 0];
                        real_t v1y = v[vi1 * 3 + 1];
                        real_t v1z = v[vi1 * 3 + 2];
                        real_t v2x = v[vi2 * 3 + 0];
                        real_t v2y = v[vi2 * 3 + 1];
                        real_t v2z = v[vi2 * 3 + 2];
                        real_t e0x = v1x - v0x;
                        real_t e0y = v1y - v0y;
                        real_t e0z = v1z - v0z;
                        real_t e1x = v2x - v1x;
                        real_t e1y = v2y - v1y;
                        real_t e1z = v2z - v1z;
                        real_t cx = std::fabs(e0y * e1z - e0z * e1y);
                        real_t cy = std::fabs(e0z * e1x - e0x * e1z);
                        real_t cz = std::fabs(e0x * e1y - e0y * e1x);
                        const real_t epsilon = std::numeric_limits<real_t>::epsilon();
                        if (cx > epsilon || cy > epsilon || cz > epsilon) {
                            // found a corner
                            if (cx > cy && cx > cz) {
                            } else {
                                axes[0] = 0;
                                if (cz > cx && cz > cy) {
                                    axes[1] = 1;
                                }
                            }
                            break;
                        }
                    }

                    face_t remainingFace = face;  // copy
                    size_t guess_vert = 0;
                    vertex_index_t ind[3];
                    real_t vx[3];
                    real_t vy[3];

                    // How many iterations can we do without decreasing the remaining
                    // _vertices.
                    size_t remainingIterations = face.vertex_indices.size();
                    size_t previousRemainingVertices =
                            remainingFace.vertex_indices.size();

                    while (remainingFace.vertex_indices.size() > 3 &&
                           remainingIterations > 0) {

                        npolys = remainingFace.vertex_indices.size();
                        if (guess_vert >= npolys) {
                            guess_vert -= npolys;
                        }

                        if (previousRemainingVertices != npolys) {
                            // The number of remaining _vertices decreased. Reset counters.
                            previousRemainingVertices = npolys;
                            remainingIterations = npolys;
                        } else {
                            // We didn't consume a vertex on previous iteration, reduce the
                            // available iterations.
                            remainingIterations--;
                        }

                        for (size_t k = 0; k < 3; k++) {
                            ind[k] = remainingFace.vertex_indices[(guess_vert + k) % npolys];
                            size_t vi = size_t(ind[k].v_idx);
                            if (((vi * 3 + axes[0]) >= v.size()) ||
                                ((vi * 3 + axes[1]) >= v.size())) {
                                // ???
                                vx[k] = static_cast<real_t>(0.0);
                                vy[k] = static_cast<real_t>(0.0);
                            } else {
                                vx[k] = v[vi * 3 + axes[0]];
                                vy[k] = v[vi * 3 + axes[1]];
                            }
                        }

                        //
                        // area is calculated per face
                        //
                        real_t e0x = vx[1] - vx[0];
                        real_t e0y = vy[1] - vy[0];
                        real_t e1x = vx[2] - vx[1];
                        real_t e1y = vy[2] - vy[1];
                        real_t cross = e0x * e1y - e0y * e1x;

                        real_t area = (vx[0] * vy[1] - vy[0] * vx[1]) * static_cast<real_t>(0.5);
                        // if an internal angle
                        if (cross * area < static_cast<real_t>(0.0)) {
                            guess_vert += 1;
                            continue;
                        }

                        // check all other verts in case they are inside this triangle
                        bool overlap = false;
                        for (size_t otherVert = 3; otherVert < npolys; ++otherVert) {
                            size_t idx = (guess_vert + otherVert) % npolys;

                            if (idx >= remainingFace.vertex_indices.size()) {
                                continue;
                            }

                            size_t ovi = size_t(remainingFace.vertex_indices[idx].v_idx);

                            if (((ovi * 3 + axes[0]) >= v.size()) ||
                                ((ovi * 3 + axes[1]) >= v.size())) {
                                continue;
                            }
                            real_t tx = v[ovi * 3 + axes[0]];
                            real_t ty = v[ovi * 3 + axes[1]];
                            if (pnpoly(3, vx, vy, tx, ty)) {
                                overlap = true;
                                break;
                            }
                        }

                        if (overlap) {
                            // std::cout << "overlap2\n";
                            guess_vert += 1;
                            continue;
                        }

                        // this triangle is an ear
                        {
                            index_t idx0, idx1, idx2;
                            idx0.vertex_index = ind[0].v_idx;
                            idx0.normal_index = ind[0].vn_idx;
                            idx0.texcoord_index = ind[0].vt_idx;
                            idx1.vertex_index = ind[1].v_idx;
                            idx1.normal_index = ind[1].vn_idx;
                            idx1.texcoord_index = ind[1].vt_idx;
                            idx2.vertex_index = ind[2].v_idx;
                            idx2.normal_index = ind[2].vn_idx;
                            idx2.texcoord_index = ind[2].vt_idx;

                            shape->mesh.indices.push_back(idx0);
                            shape->mesh.indices.push_back(idx1);
                            shape->mesh.indices.push_back(idx2);

                            shape->mesh.num_face_vertices.push_back(3);
//                                shape->mesh.smoothing_group_ids.push_back(
//                                        face.smoothing_group_id);
                        }

                        // remove v1 from the list
                        size_t removed_vert_index = (guess_vert + 1) % npolys;
                        while (removed_vert_index + 1 < npolys) {
                            remainingFace.vertex_indices[removed_vert_index] =
                                    remainingFace.vertex_indices[removed_vert_index + 1];
                            removed_vert_index += 1;
                        }
                        remainingFace.vertex_indices.pop_back();
                    }
                    if (remainingFace.vertex_indices.size() == 3) {
                        i0 = remainingFace.vertex_indices[0];
                        i1 = remainingFace.vertex_indices[1];
                        i2 = remainingFace.vertex_indices[2];
                        {
                            index_t idx0, idx1, idx2;
                            idx0.vertex_index = i0.v_idx;
                            idx0.normal_index = i0.vn_idx;
                            idx0.texcoord_index = i0.vt_idx;
                            idx1.vertex_index = i1.v_idx;
                            idx1.normal_index = i1.vn_idx;
                            idx1.texcoord_index = i1.vt_idx;
                            idx2.vertex_index = i2.v_idx;
                            idx2.normal_index = i2.vn_idx;
                            idx2.texcoord_index = i2.vt_idx;

                            shape->mesh.indices.push_back(idx0);
                            shape->mesh.indices.push_back(idx1);
                            shape->mesh.indices.push_back(idx2);

                            shape->mesh.num_face_vertices.push_back(3);
//                                shape->mesh.smoothing_group_ids.push_back(
//                                        face.smoothing_group_id);
                        }
                    }
                }  // npolys
            } else {
                for (size_t k = 0; k < npolys; k++) {
                    index_t idx;
                    idx.vertex_index = face.vertex_indices[k].v_idx;
                    idx.normal_index = face.vertex_indices[k].vn_idx;
                    idx.texcoord_index = face.vertex_indices[k].vt_idx;
                    shape->mesh.indices.push_back(idx);
                }

                shape->mesh.num_face_vertices.push_back(
                        static_cast<unsigned char>(npolys));
            }
        }
    }

    return true;
}


bool LoadObj(attrib_t *attrib, std::vector<shape_t> *shapes, const char *filename, bool triangulate) {
    attrib->vertices.clear();
    attrib->normals.clear();
    attrib->texcoords.clear();
    shapes->clear();

    std::stringstream errss;

    std::ifstream ifs(filename);
    if (!ifs) {
        errss << "Cannot open file [" << filename << "]\n";
        return false;
    }

    return LoadObj(attrib, shapes, &ifs, triangulate);
}

bool LoadObj(attrib_t *attrib, std::vector<shape_t> *shapes, std::istream *inStream, bool triangulate) {

    std::vector<real_t> v;
    std::vector<real_t> vn;
    std::vector<real_t> vt;
    PrimGroup prim_group;
    std::string name;

    shape_t shape;

    std::string linebuf;
    while (inStream->peek() != -1) {
        safeGetline(*inStream, linebuf);

        // Trim newline '\r\n' or '\n'
        if (linebuf.size() > 0) {
            if (linebuf[linebuf.size() - 1] == '\n')
                linebuf.erase(linebuf.size() - 1);
        }
        if (linebuf.size() > 0) {
            if (linebuf[linebuf.size() - 1] == '\r')
                linebuf.erase(linebuf.size() - 1);
        }

        // Skip if empty line.
        if (linebuf.empty()) {
            continue;
        }

        // Skip leading space.
        const char *token = linebuf.c_str();
        token += strspn(token, " \t");

        assert(token);
        if (token[0] == '\0') continue;  // empty line

        if (token[0] == '#') continue;  // comment line

        // vertex
        if (token[0] == 'v' && IS_SPACE((token[1]))) {
            token += 2;
            real_t x, y, z;
            parseReal3(&x, &y, &z, &token);
            v.push_back(x);
            v.push_back(y);
            v.push_back(z);
            continue;
        }
        // normal
        if (token[0] == 'v' && token[1] == 'n' && IS_SPACE((token[2]))) {
            token += 3;
            real_t x, y, z;
            parseReal3(&x, &y, &z, &token);
            vn.push_back(x);
            vn.push_back(y);
            vn.push_back(z);
            continue;
        }
        // texcoord
        if (token[0] == 'v' && token[1] == 't' && IS_SPACE((token[2]))) {
            token += 3;
            real_t x, y;
            parseReal2(&x, &y, &token);
            vt.push_back(x);
            vt.push_back(y);
            continue;
        }
        // face
        if (token[0] == 'f' && IS_SPACE((token[1]))) {
            token += 2;
            token += strspn(token, " \t");

            face_t face;
            face.vertex_indices.reserve(3);

            while (!IS_NEW_LINE(token[0])) {
                vertex_index_t vi;
                parseTriple(&token, static_cast<int>(v.size() / 3),
                            static_cast<int>(vn.size() / 3),
                            static_cast<int>(vt.size() / 2), &vi);

                face.vertex_indices.push_back(vi);
                size_t n = strspn(token, " \t\r");
                token += n;
            }
            prim_group.faceGroup.push_back(face);
            continue;
        }

    }

    bool ret = exportGroupsToShape(&shape, prim_group, name, triangulate, v);

    if (ret || shape.mesh.indices.size()) {
        shapes->push_back(shape);
    }
    prim_group.clear();  // for safety

    attrib->vertices.swap(v);
    attrib->normals.swap(vn);
    attrib->texcoords.swap(vt);

    return true;
}
