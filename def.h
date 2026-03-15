#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG // _DEBUG ('new' redefinition conflicts with assimp)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> // subst X: D:/root/diskX
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace glm;

typedef double dbl;
typedef float flt;
typedef unsigned long       DWORD;

class color
{
public:
    dbl R, G, B, A;

    color()
    {
        R = 0;
        G = 0;
        B = 0;
        A = 255;
    }

    ~color()
    {
    }

    color& clamp(void)
    {
        R = std::max(std::min(R, 255.0), 0.0);
        G = std::max(std::min(G, 255.0), 0.0);
        B = std::max(std::min(B, 255.0), 0.0);
        A = std::max(std::min(A, 255.0), 0.0);
        return *this;
    }

    color(const dbl Red, const dbl Green, const dbl Blue, const dbl Alpha)
    {
        R = Red;
        G = Green;
        B = Blue;
        A = Alpha;
    }

    color(const dbl Red, const dbl Green, const dbl Blue)
    {
        R = Red;
        G = Green;
        B = Blue;
        A = 255;
    }

    color(const dbl Component) :
        R(Component), G(Component), B(Component), A(255)
    {
    }

    color(const vec3 Vec)
    {
        R = Vec[0];
        G = Vec[1];
        B = Vec[2];
        A = 255;
    }

    color& operator-(void)
    {
        R = 255 - R;
        G = 255 - G;
        B = 255 - B;
        return *this;
    }

    color operator-(const color& c) const
    {
        dbl NewR = R - c.R,
            NewG = G - c.G,
            NewB = B - c.B;

        return color(NewR, NewG, NewB);
    }

    color operator+(const color& c) const
    {
        dbl NewR = R + c.R,
            NewG = G + c.G,
            NewB = B + c.B;

        return color(NewR, NewG, NewB);
    }

    color operator*(const dbl& x) const
    {
        dbl NewR = R * x,
            NewG = G * x,
            NewB = B * x;

        return color(NewR, NewG, NewB);
    }

    color operator/(const dbl& x) const
    {
        dbl NewR = R / x,
            NewG = G / x,
            NewB = B / x;

        return color(NewR, NewG, NewB);
    }

    DWORD DwordColor(void)
    {
        DWORD DColor = ((DWORD)R << 8 * 2) + ((DWORD)G << 8) + (DWORD)B;
        return DColor;
    }
};
