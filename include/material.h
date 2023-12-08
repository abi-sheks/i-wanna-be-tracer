#pragma once

#include "utilities.h"
#include "hittable.h"
#include "ray.h"
#include "color.h"

class hit_info;

class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray &ray_in, hit_info &info, Color &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(const Color &col) : albedo(col) {}
    bool scatter(const Ray &ray_in, hit_info &info, Color &attenuation, Ray &scattered) const override
    {
        // always scatters
        // the math for this Lambertian diffusion is beautiful, read up again.
        auto scatter_direction = info.normal + randomUnitVec();

        // to check for case when randomUnitVec() is exactly opposite info.normal
        if (scatter_direction.near_zero())
        {
            scatter_direction = info.normal;
        }
        scattered = Ray(info.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    // albedo = proportion of incident light reflected.
    Color albedo;
};
class Metal : public Material
{
public:
    Metal(const Color &col, double f) : albedo(col), fuzz(f < 1 ? f : 1) {}
    bool scatter(const Ray &ray_in, hit_info &info, Color &attenuation, Ray &scattered) const override
    {
        vec3 dir = normalize(ray_in.direction());
        vec3 reflected_direction = reflect(dir, info.normal);
        scattered = Ray(info.p, reflected_direction + fuzz*randomUnitVec());
        attenuation = albedo;
        //to count out reflections below the surface
        return info.normal.dot(scattered.direction());
    }

private:
    // albedo = proportion of incident light reflected.
    Color albedo;
    double fuzz;
};