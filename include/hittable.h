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
    // texels
    double u;
    double v;

    void setNormalFace(const Ray &r, const vec3 &outward_normal)
    {
        // checks if ray is coming from outside the sphere, or from inside the sphere
        front_face = r.direction().dot(outward_normal) < 0;
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

class Translate : public Hittable
{
public:
    Translate(std::shared_ptr<Hittable> obj, const vec3 &displacement) : object(obj), offset(displacement)
    {
        bbox = object->boundingBox() + displacement;
    }
    bool hit(const Ray &ray, Interval t_limits, hit_info &info) const override
    {
        // first, check intersection with offsetted ray
        Ray offset_r(ray.origin() - offset, ray.direction(), ray.time());

        if (!object->hit(offset_r, t_limits, info))
        {
            return false;
        }

        // offset object poi accordingly
        info.p += offset;
        return true;
    }
    AABB boundingBox() const override { return bbox; }

private:
    std::shared_ptr<Hittable> object;
    vec3 offset;
    AABB bbox;
};

class RotateY : public Hittable
{
public:
    RotateY(double angle, std::shared_ptr<Hittable> _object) : object(_object)
    {
        auto theta = degreeToRadians(angle);
        sin_theta = sin(theta);
        cos_theta = cos(theta);
        bbox = object->boundingBox();
        Point3 min(infinity, infinity, infinity);
        Point3 max(-infinity, -infinity, -infinity);
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                    auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                    auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                    // following basic coordinate change formula
                    auto newx = cos_theta * x + sin_theta * z;
                    auto newz = -sin_theta * x + cos_theta * z;

                    vec3 test(newx, y, newz);
                    for (int c = 0; c < 3; c++)
                    {
                        min[c] = fmin(min[c], test[c]);
                        max[c] = fmax(max[c], test[c]);
                    }
                }
            }
        }
        bbox = AABB(min, max);
    }
    bool hit(const Ray &ray, Interval t_limits, hit_info &info) const override
    {
        auto origin = ray.origin();
        auto dir = ray.direction();

        // same strategy as translation, just need to handle normal as well now.

        // rotate the ray backwards
        origin[0] = cos_theta * ray.origin()[0] - sin_theta * ray.origin()[2];
        origin[2] = sin_theta * ray.origin()[0] + cos_theta * ray.origin()[2];
        dir[0] = cos_theta * ray.direction()[0] - sin_theta * ray.direction()[2];
        dir[2] = sin_theta * ray.direction()[0] + cos_theta * ray.direction()[2];

        Ray rotated_ray(origin, dir, ray.time());

        // check for hit with rotated ray
        if (!object->hit(rotated_ray, t_limits, info))
        {
            return false;
        }

        // rotate the POI and normal forward
        // i.e change the normal from object space to world space
        auto p = info.p;
        auto normal = info.normal;
        p[0] = cos_theta * info.p[0] + sin_theta * info.p[2];
        p[2] = -sin_theta * info.p[0] + cos_theta * info.p[2];
        normal[0] = cos_theta * info.normal[0] + sin_theta * info.normal[2];
        normal[2] = -sin_theta * info.normal[0] + cos_theta * info.normal[2];

        info.p = p;
        info.normal = normal;
        return true;
    }
    AABB boundingBox() const override {return bbox;}

private:
    std::shared_ptr<Hittable> object;
    double sin_theta;
    double cos_theta;
    AABB bbox;
};
