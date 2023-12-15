#pragma once
#include <memory>
#include "material.h"
#include "aabb.h"
#include "hittable.h"
#include "hittable_array.h"
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

inline std::shared_ptr<HittableArray> box(const Point3& p1, const Point3& p2, std::shared_ptr<Material> mat)
{
    //returns 3D box with p1 and p2 as opposite vertices
    std::shared_ptr<HittableArray> box = std::make_shared<HittableArray>();

    //extrema vertices
    auto min = Point3(fmin(p1.x(), p2.x()), fmin(p1.y(), p2.y()), fmin(p1.z(), p2.z()));
    auto max = Point3(fmax(p1.x(), p2.x()), fmax(p1.y(), p2.y()), fmax(p1.z(), p2.z()));
    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    //adding each side
    box->add(std::make_shared<Quad>(Point3(min.x(), min.y(), max.z()), dx, dy, mat)); //front
    box->add(std::make_shared<Quad>(Point3(min.x(), max.y(), max.z()), dx, -dz, mat)); //top
    box->add(std::make_shared<Quad>(Point3(max.x(), min.y(), max.z()), dy, -dz, mat)); //right
    box->add(std::make_shared<Quad>(Point3(min.x(), min.y(), min.z()), dy, dz, mat)); //left
    box->add(std::make_shared<Quad>(Point3(max.x(), min.y(), min.z()), dy, -dx, mat)); //back
    box->add(std::make_shared<Quad>(Point3(min.x(), min.y(), min.z()), dx, dz, mat)); //front

    return box;
}