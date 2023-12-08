#pragma once
#include "vec3.h"

class Ray
{

public:
    Ray() {}

    Ray(const Point3 &_orig, const vec3 &_dir) : orig(_orig), dir(_dir) {}

    Point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    Point3 at(double param) const
    {
        return orig + (param * dir);
    }

private:
    Point3 orig;
    vec3 dir;
};