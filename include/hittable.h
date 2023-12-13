#pragma once

#include <memory>
#include "vec3.h"
#include "ray.h"
#include "aabb.h"
#include "interval.h"

class Material;
class hit_info
{
public:
    Point3 p;
    vec3 normal;
    double t;
    std::shared_ptr<Material> mat;
    // uses strategy of always setting normal opposite to incoming ray
    bool front_face;
    //texels
    double u;
    double v;

    void set_normal_face(const Ray &r, vec3 &outward_normal)
    {
        // checks if ray is coming from outside the sphere, or from inside the sphere
        front_face = outward_normal.dot(r.direction()) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable
{
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray &ray, Interval t_limits, hit_info &info) const = 0;

    virtual AABB boundingBox() const = 0;
};