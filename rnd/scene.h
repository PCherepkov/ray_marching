#pragma once

#include <vector>

#include "../def.h"
#include "ray/shape.h"
#include "ray/ray.h"
#include "ray/mins.h"

class scene
{
    dbl tol = 1e-4;
    dbl inf = 1e6;
public:
    std::vector<shape*> objects;
    int nofs;


    scene()
    {
    }

    ~scene()
    {
        for (auto el : objects)
            delete el;
    }

    void AddShape(shape* Shp)
    {
        objects.push_back(Shp);
        nofs++;
    }

    dbl SDF(vec3 Point)
    {
        dbl mdist = inf;
        for (auto s : objects) {
            dbl prev = mdist, dist = s->sdf(Point);
            mdist = ExpSMin(mdist, dist, 0.1);
        }
        return mdist;
    }

    vec3 Normal(vec3 Point)
    {
        dbl
            x = SDF(Point + vec3(0.01, 0, 0)) - SDF(Point + vec3(-0.01, 0, 0)),
            y = SDF(Point + vec3(0, 0.01, 0)) - SDF(Point + vec3(0, -0.01, 0)),
            z = SDF(Point + vec3(0, 0, 0.01)) - SDF(Point + vec3(0, 0, -0.01));
        return normalize(vec3(x, y, z));
    }

    color Shade(ray Ray, dbl t, shape* obj)
    {
        vec3 LightPos = normalize(vec3(0, 4, -10));
        vec3 LightColor(1, 1, 1);
        vec3 ObjColor = vec3(obj->Color.R, obj->Color.G, obj->Color.B) / 255.f;

        vec3 N = Normal(Ray.O + Ray.D * (flt)t);
        flt diff = std::max(dot(N, LightPos), 0.f);
        vec3 diffuse = LightColor * diff;

        flt amb = 0.1;
        vec3 ambient = amb * LightColor;

        vec3 res = (diffuse + ambient) * ObjColor;
        // res = LightColor * std::max(dot(N, LightPos), 0.f);

        return color(res.x * 255, res.y * 255, res.z * 255);
    }

    color March(ray Ray)
    {
        color ResColor(93, 46, 140);
        shape* obj = nullptr;
        dbl step = (inf + tol) / 2, t = 0;
        vec3 pos = Ray.O;
        dbl mdist = inf;

        while (step > tol and step < inf) {
            mdist = inf;

            for (auto s : objects) {
                dbl prev = mdist, dist = s->sdf(pos);
                mdist = ExpSMin(mdist, dist, 0.16);
                if (abs(mdist - prev) > abs(mdist - dist))
                    obj = s;
            }
            step = mdist;
            t += step;
            pos += Ray.D * (flt)step;
        }

        if (step <= tol)
        {
            // ResColor = obj->Color;
            ResColor = Shade(Ray, t, obj);
            return ResColor;
            dbl sdf0 = objects[0]->sdf(pos), sdf1 = objects[1]->sdf(pos);
            dbl coef0 = mdist < sdf0 ? sdf0 - mdist : mdist - sdf0;
            dbl coef1 = mdist < sdf1 ? sdf1 - mdist : mdist - sdf1;
            ResColor = Shade(Ray, t, objects[0]) * coef0 + Shade(Ray, t, objects[1]) * coef1;
            ResColor = ResColor / (coef0 + coef1);
        }
        
        return ResColor;
    }
};

