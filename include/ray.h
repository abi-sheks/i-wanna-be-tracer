#pragma once
#include "vec3.h"

class Ray
{

public:
    Ray() {}

    Ray(const Point3 &_orig, const vec3 &_dir, double time = 0.0) : orig(_orig), dir(_dir), tm(time) {}

    Point3 origin() const { return orig; }
    vec3 direction() const { return dir; }
    double time() const {return tm;}

    Point3 at(double param) const
    {
        return orig + (param * dir);
    }

private:
    Point3 orig;
    vec3 dir;
    double tm;
};