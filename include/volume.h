#pragma once
#include "hittable.h"
#include "material.h"
#include "aabb.h"
#include "texture.h"

class ConstantMedium : public Hittable
{
public:
    ConstantMedium(std::shared_ptr<Hittable> bound, double den, std::shared_ptr<Texture> tex) : boundary(bound), neg_inv_density(-1 / den), phase_function(std::make_shared<Isotropic>(tex)) {}
    ConstantMedium(std::shared_ptr<Hittable> bound, double den, Color c) : boundary(bound), neg_inv_density(-1 / den), phase_function(std::make_shared<Isotropic>(c)) {}

    bool hit(const Ray &ray, Interval t_limits, hit_info &info) const override
    {
        // a lot of gymnastics here just to account for hit from within volume
        hit_info hit1, hit2;

        // general hit checks
        if (!boundary->hit(ray, universe, hit1))
        {
            return false;
        }
        if (!boundary->hit(ray, Interval(hit1.t + 0.0001, infinity), hit2))
        {
            return false;
        }

        // these checks make more sense when u think from POV of ray from inside object
        if (hit1.t < t_limits.min)
            hit1.t = t_limits.min;
        if (hit2.t > t_limits.max)
            hit2.t = t_limits.max;

        if (hit1.t >= hit2.t)
        {
            return false;
        }

        if (hit1.t < 0)
        {
            hit1.t = 0;
        }
        auto ray_length = ray.direction().length();
        auto distance_inside_boundary = (hit2.t - hit1.t) * ray_length;
        auto hit_distance = neg_inv_density * log(randomDouble());

        if(hit_distance > distance_inside_boundary)
        {
            return false;
        }
        info.t = hit1.t + hit_distance / ray_length;
        info.p = ray.at(hit1.t);
        info.normal = vec3(1, 1 ,1); //arbitrary
        info.front_face = true; //arbitrary
        info.mat = phase_function;
        return true;
    }
    AABB boundingBox() const override {return boundary->boundingBox();}

private:
    std::shared_ptr<Hittable> boundary;
    double neg_inv_density;
    std::shared_ptr<Material> phase_function;
};