#pragma once

#include "../def.h"
#include "ray/ray.h"


class camera
{
public:
    mat4 MView;
    mat4 MProj;

    vec3 Loc, At, Up, Right, Dir;
private:
    const dbl
        Near,          // Near clip plane.
        Far,           // Far clip plane.
        ProjSize;      // Projection basic size.
    dbl
        Wp, Hp,        // Projection scales.
        Dist;          // Camera distance.
    int Width, Height; // Frame sizes.
public:
    camera(dbl Near = 0.1, dbl Far = 1000, dbl ProjSize = 1.0) :
        Near(Near), Far(Far), ProjSize(ProjSize), Dist(1.0) {}

    camera& SetProj(const int W, const int H)
    {
        dbl ratio_x = ProjSize / 2, ratio_y = ratio_x;
        Width = W;
        Height = H;

        if (W > H)
            ratio_x *= (dbl)W / H;
        else
            ratio_y *= (dbl)H / W;
        Wp = ratio_x * 2;
        Hp = ratio_y * 2;
        MProj = frustum(-ratio_x, ratio_x, -ratio_y, ratio_y, Near, Far);
        return *this;
    }

    camera& SetView(const vec3& Location, const vec3& AtVec, const vec3& UpVec)
    {
        MView = lookAt(Location, AtVec, UpVec);
        Dir = AtVec - Location;
        Dir = normalize(Dir);

        Right = cross(Dir, UpVec);
        Right = normalize(Right);

        Up = UpVec;
        Up = normalize(Up);

        //Dist = !Dir;

        Loc = Location;
        At = AtVec;
        return *this;
    }

    ray MakeRay(int sx, int sy, dbl jitterX = 0.5, dbl jitterY = 0.5)
    {
        vec3
            A = Dir * (flt)Dist,
            B = Right * (flt)(((dbl)sx + jitterX - (dbl)Width / 2.0) * Wp / (dbl)Width),
            C = Up * (flt)(-((dbl)sy + jitterY - (dbl)Height / 2.0) * Hp / (dbl)Height),
            X = A + B + C,
            O = Loc + X,
            D = normalize(X);

        return ray(O, D);
    }
};
