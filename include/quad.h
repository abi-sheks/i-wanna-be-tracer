#pragma once
#include <memory>
#include "material.h"
#include "aabb.h"
#include "hittable.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"

class Quad : public Hittable
{
public:
    Quad(const Point3 &_O, const vec3 &_u, const vec3 &_v, std::shared_ptr<Material> _mat)
        : O(_O), u(_u), v(_v), mat(_mat)
    {
        // basic plane eqn principles
        auto temp = _u;
        auto normal = temp.cross(_v);
        n = normalize(normal);
        D = n.dot(_O);
        setBoundingBox();
        w = normal / normal.dot(normal);
    }
    void setBoundingBox()
    {
        bbox = AABB(O, O + u + v).pad();
    }
    AABB boundingBox() const override { return bbox; }
    bool hit(const Ray &ray, Interval t_limits, hit_info &info) const override
    {
        // put ray = a + tb in n.p=D, solve for t
        auto denom = ray.direction().dot(n); // b.n
        // checking parallel
        if (fabs(denom) < 1e-8)
        {
            return false;
        }
        auto t = (D - ray.origin().dot(n)) / denom;
        if (!t_limits.contains(t))
        {
            return false;
        }
        // checking whether poi is in plane using alpha beta test
        auto poi = ray.at(t);
        vec3 hit_vec = poi - O;
        auto alpha = (hit_vec.cross(v)).dot(w);
        auto beta = (-hit_vec.cross(u)).dot(w);

        if (!isInterior(alpha, beta, info))
        {
            return false;
        }

        info.t = t;
        info.p = poi;
        info.mat = mat;
        info.setNormalFace(ray, n);

        return true;
    }

    // making virtual to extend to other quadrilateral primitives, same simple principle applies everywhere
    virtual bool isInterior(double a, double b, hit_info &info) const
    {
        if (a > 1 || a < 0 || b < 0 || b > 1)
        {
            return false;
        }
        info.u = a;
        info.v = b;
        return true;
    }

private:
    Point3 O;
    vec3 u, v;
    std::shared_ptr<Material> mat;
    AABB bbox;
    // using basic plane formula n.p = d
    double D;
    vec3 n;
    // for checking if poi is in plane, there is a clear derivation using u, v as basis vectors for quad region.
    vec3 w;
};