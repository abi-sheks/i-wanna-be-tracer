#pragma once
#include "interval.h"
#include "vec3.h"
#include "ray.h"
// Axis aligned bounding rectangular parallelopiped
class AABB
{
public:
    Interval x, y, z;
    AABB() {}
    AABB(const Interval &_x, const Interval &_y, const Interval &_z) : x(_x), y(_y), z(_z) {}
    AABB(const Point3 &a, const Point3 &b)
    {
        // uses a and b as extrema for bb
        x = Interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
        y = Interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
        z = Interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
    }
    //for two boxes
    AABB(const AABB& box0, const AABB& box1)
    {
        x = Interval(box0.x, box1.x);
        y = Interval(box0.y, box1.y);
        z = Interval(box0.z, box1.z);
    }
    //for 2D quadrilaterals, which will have zero dimension along some axis
    AABB pad()
    {
        double delta = 0.0001;
        Interval _x = (x.size() >= delta) ? x : x.expand(delta);
        Interval _y = (y.size() >= delta) ? y : y.expand(delta);
        Interval _z = (z.size() >= delta) ? z : z.expand(delta);
        return AABB(_x, _y, _z);
    }
    const Interval &getAxis(int n) const
    {
        // relevant to hit function
        if (n == 1)
            return y;
        if (n == 2)
            return z;
        return x;
    }

    bool hit(const Ray &r, Interval r_t) const
    {
        for (int i = 0; i < 3; i++)
        {
            auto b_i = 1 / r.direction()[i];
            auto A_i = r.origin()[i];

            // using formula t_x = (x - A_x/b_x), generalise with i
            auto t0 = (getAxis(i).min - A_i) * b_i;
            auto t1 = (getAxis(i).max - A_i) * b_i;

            // handling ray travelling along negative axis
            if (b_i < 0)
            {
                std::swap(t0, t1);
            }

            // standard checks
            if (t0 > r_t.min)
                r_t.min = t0;
            if (t1 < r_t.max)
                r_t.max = t1;

            // for non hit condition
            if (r_t.max <= r_t.min)
            {
                return false;
            }
        }
        return true;
    }
};

AABB operator+(AABB bbox, const vec3& offset)
{
    return AABB(bbox.x + offset.x(),bbox.y + offset.y(), bbox.z + offset.z());
}

AABB operator+(const vec3& offset, AABB bbox)
{
    return bbox + offset;
}