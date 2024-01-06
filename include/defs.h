//
// Created by Brave Fang on 2023/11/19.
//

#ifndef BILLIARDS_DEFS_H
#define BILLIARDS_DEFS_H
#define RADIUS 0.05f
#define LATITUDE 300
#define LONGITUDE 300
#define ACCLAR 0.6
#define EPS 0.005
#define Y_UNIT_VECTOR glm::vec3(0.0f, 1.0f, 0.0f)

#define FoV 45.0f

#define IS_SPACE(x) (((x) == ' ') || ((x) == '\t'))
#define IS_NEW_LINE(x) (((x) == '\r') || ((x) == '\n') || ((x) == '\0'))

#endif //BILLIARDS_DEFS_H
