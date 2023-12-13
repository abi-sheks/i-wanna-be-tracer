#pragma once
#include <algorithm>
#include <memory>
#include <vector>
#include "aabb.h"
#include "utilities.h"
#include "hittable.h"
#include "hittable_array.h"

// forming a tree of sorts where each node has two child nodes/leaves
class BVHNode : public Hittable
{
public:
    BVHNode(const HittableArray &hittables) : BVHNode(hittables.objects, 0, hittables.objects.size()) {}
    BVHNode(const std::vector<std::shared_ptr<Hittable>> &scene_objects, int start, int end)
    {
        // editable array
        auto objects = scene_objects;

        // pick random axis to check
        int axis = randomInt(0, 2);
        auto comparator = (axis == 0) ? compareBoxX : ((axis == 1) ? compareBoxY : compareBoxZ);
        int size = end - start;
        if (size == 1)
        {
            // only one element left, break recursion
            left = objects[start];
            right = objects[start];
        }
        else if (size == 2)
        {
            // again, only 2 left, break recursion
            left = objects[start + 1];
            right = objects[start];
            if (comparator(objects[start], objects[start + 1]))
            {
                left = objects[start];
                right = objects[start + 1];
            }
        }
        else
        {
            // continue recursing after splitting list in half after sorting
            std::sort(objects.begin() + start, objects.begin() + end, comparator);
            auto mid = start + size / 2;
            left = std::make_shared<BVHNode>(objects, start, mid);
            right = std::make_shared<BVHNode>(objects, mid, end);
        }
        // done building
        bbox = AABB(left->boundingBox(), right->boundingBox());
    }

    bool hit(const Ray &ray, Interval t_limits, hit_info &info) const override
    {
        if (!bbox.hit(ray, t_limits))
        {
            return false;
        }

        bool hit_left = left->hit(ray, t_limits, info);
        // this is an optimization, cuz left is also physically to the left of right
        bool hit_right = right->hit(ray, Interval(t_limits.min, hit_left ? info.t : t_limits.max), info);

        return hit_left || hit_right;
    }

    AABB boundingBox() const override { return bbox; }

private:
    // these will also be bvh_nodes
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    AABB bbox;

    static bool compareBox(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis_index)
    {
        // returns a < b to order left and right accordingly
        return a->boundingBox().getAxis(axis_index).min < b->boundingBox().getAxis(axis_index).min;
    }
    static bool compareBoxX(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return compareBox(a, b, 0);
    }
    static bool compareBoxY(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return compareBox(a, b, 1);
    }
    static bool compareBoxZ(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return compareBox(a, b, 2);
    }
};