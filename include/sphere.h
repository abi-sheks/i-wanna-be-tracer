#pragma once

#include <memory>
#include "hittable.h"
#include "interval.h"
#include "material.h"

class Sphere : public Hittable
{
public:
    Sphere(Point3 _center, double _radius, std::shared_ptr<Material> _mat) : center(_center), radius(_radius), mat(_mat) {}

    bool hit(const Ray &ray, Interval t_limit, hit_info &info) const override
    {
        // placing P = A + tB in (P-C).(P-C) = radius^2 and solving for parameter t
        vec3 oc = ray.origin() - center; // A-C
        // a,b,c in quadratic equation sense (optimized by putting b = 2b)
        auto a = ray.direction().sqrLength();          // B.B
        auto b_half = (oc.dot(ray.direction()));       // 2B.(A-C)
        auto c = (oc.sqrLength()) - (radius * radius); //(A-c).(A-C) - radius^2
        auto discriminant = (b_half * b_half) - (a * c);
        if (discriminant < 0)
        {
            return false;
        }

        double sqrtd = sqrt(discriminant);

        double root = (-b_half - sqrtd) / a;

        //checking if smaller root is within acceptable range... 
        if(!t_limit.contains(root))
        {
            //if not then larger root
            root = (-b_half + sqrtd) / a;
            if(!t_limit.contains(root))
            {
                return false;
            }
        }

        info.t = root;
        info.p = ray.at(info.t);
        info.mat = mat;
        //need unit vector, length of normal vector to sphere is radius
        vec3 outward_normal = (info.p - center) / radius;
        info.set_normal_face(ray, outward_normal);     
        return true;
    }

private:
    Point3 center;
    double radius;
    std::shared_ptr<Material> mat;
};