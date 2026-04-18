#pragma once

#include "../def.h"


class camera
{
public:
    vec3 Loc, Up, Right, Dir;
    float speed;
public:
    camera(vec3 cLoc = vec3(0), vec3 cUp = vec3(0, 1, 0), vec3 cRight = vec3(1, 0, 0), vec3 cDir = vec3(0, 0, -1)) :
        Loc(cLoc), Up(cUp), Right(cRight), Dir(cDir) { speed = 4.0; }

    size_t MakeRay(int sx, int sy, dbl jitterX = 0.5, dbl jitterY = 0.5) {
        return 0;
    }
};
