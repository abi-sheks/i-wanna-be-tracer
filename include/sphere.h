#pragma once

#include <memory>
#include <cmath>
#include "aabb.h"
#include "hittable.h"
#include "interval.h"
#include "material.h"
#include "utilities.h"

class Sphere : public Hittable
{
public:
    // for stationary sphere
    Sphere(Point3 _center_start, double _radius, std::shared_ptr<Material> _mat) : center_start(_center_start), radius(_radius), mat(_mat), is_moving(false)
    {
        // bounding box calc
        // constructs a cube of sorts surrounding the sphere
        auto radius_vector = vec3(radius, radius, radius);
        bbox = AABB(center_start - radius_vector, center_start + radius_vector);
    }

    // for moving sphere (final center_point expected) (simulates movement by lerping center between a start and end point over time period)
    Sphere(Point3 _center_start, Point3 center_end, double _radius, std::shared_ptr<Material> _mat) : center_start(_center_start), radius(_radius), mat(_mat), is_moving(true)
    {
        auto radius_vector = vec3(radius, radius, radius);
        AABB b1(_center_start - radius_vector, _center_start + radius_vector);
        AABB b2(center_end - radius_vector, center_end + radius_vector);
        bbox = AABB(b1, b2);
        center_delta = center_end - _center_start;
    }

    AABB boundingBox() const override { return bbox; }
    bool hit(const Ray &ray, Interval t_limit, hit_info &info) const override
    {
        // placing P = A + tB in (P-C).(P-C) = radius^2 and solving for parameter t

        // check for movemement
        double ray_time = ray.time();
        Point3 center = is_moving ? getCenter(ray_time) : center_start;
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

        // checking if smaller root is within acceptable range...
        if (!t_limit.contains(root))
        {
            // if not then larger root
            root = (-b_half + sqrtd) / a;
            if (!t_limit.contains(root))
            {
                return false;
            }
        }

        info.t = root;
        info.p = ray.at(info.t);
        info.mat = mat;
        // need unit vector, length of normal vector to sphere is radius
        vec3 outward_normal = (info.p - center) / radius;
        info.setNormalFace(ray, outward_normal);
        getSphereUV(outward_normal, info.u, info.v);
        return true;
    }

private:
    Point3 center_start;
    vec3 center_delta;
    AABB bbox;
    bool is_moving;
    double radius;
    std::shared_ptr<Material> mat;

    Point3 getCenter(double time) const
    {
        // returns a lerp of center_start and center_end
        // at time = 0, center is at start, time = 1 center is at end
        return center_start + (time * center_delta);
    }

    static void getSphereUV(const Point3& p, double& u, double& v) {
    //P : a point on unit sphere centered at origin

    //derivation from spherical coordinates, mapping x y z to u v,
    // we use u = phi /2pi, v = theta/pi
    auto theta = acos(-p.y());
    auto phi = atan2(-p.z(), p.x()) + pi;
    u = phi / (2*pi);
    v = theta / pi;
    }
};