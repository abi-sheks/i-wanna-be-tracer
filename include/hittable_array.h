#pragma once

#include "hittable.h"
#include "interval.h"
#include "aabb.h"
#include <memory>
#include <vector>

class HittableArray : public Hittable
{
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableArray() {}
    HittableArray(std::shared_ptr<Hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(std::shared_ptr<Hittable> object)
    {
        objects.push_back(object);
        //incrementing the box volume incrementally for each object, taking advantage of property of AABB constructor and Interval constructor
        bbox = AABB(bbox, object->boundingBox());
    }

    AABB boundingBox() const override {return bbox;}

    // using a paradigm where hasA logic is shifted to isA logic
    // usually i would not inherit, and do the following function for each object in HittableArray elsewhere.
    // this approach just defines the above action but for the Array class itself.
    bool hit(const Ray &ray, Interval t_limit, hit_info &info) const
    {
        hit_info temp_rec;
        bool hit_anything = false;
        auto closest_so_far = t_limit.max;


        //checks for hit with farthest object first, then sets that t as t_max for next object, iterating over object hits from farthest to closest.
        //at the end, info contains hit info of closest object
        for (const auto &object : objects)
        {
            if (object->hit(ray, Interval(t_limit.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                info = temp_rec;
            }
        }

        return hit_anything;
    }
    private:
    AABB bbox;
};