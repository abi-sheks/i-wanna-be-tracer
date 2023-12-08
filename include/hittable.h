#pragma once

#include "vec3.h"
#include "ray.h"
#include "interval.h"

typedef struct {
        Point3 p;
        vec3 normal;
        double t;

        //uses strategy of always setting normal opposite to incoming ray
        bool front_face;

        void set_normal_face(const Ray& r, vec3& outward_normal)
        {
            //checks if ray is coming from outside the sphere, or from inside the sphere
            front_face = outward_normal.dot(r.direction()) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
} hit_info;


class Hittable {
    public:
        virtual ~Hittable() = default;

        virtual bool hit(const Ray& ray, Interval t_limits, hit_info& info) const = 0;

};