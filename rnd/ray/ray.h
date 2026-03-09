#pragma once

#include "../../def.h"

class ray
{
public:
    vec3 O;
    vec3 D;

    ray()
    {
        O = vec3(0);
        D = vec3(0);
    }

    ray(const vec3& Ov, const vec3& Dv)
    {
        O = Ov;
        D = Dv;
    }

    ~ray()
    {
    }
};
