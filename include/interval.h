#pragma once
#include "utilities.h"

class Interval
{
public:
    double min, max;
    Interval() : min(-infinity), max(+infinity) {}
    Interval(double _min, double _max) : min(_min), max(_max) {}
    // exclusively for bbox, basically does expand/larger interval op for these
    Interval(const Interval &i1, const Interval &i2) : min(fmin(i1.min, i2.min)), max(fmax(i1.max, i2.max)) {}

    bool contains(const double x)
    {
        return x >= min && x <= max;
    }
    bool surrounds(const double x)
    {
        return x > min && x < max;
    }
    double size()
    {
        return max - min;
    }
    Interval expand(double padding) const
    {
        auto padding_by_two = padding / 2;
        return Interval(min - padding_by_two, max + padding_by_two);
    }
    double clamp(double x)
    {
        if (x < min)
            return min;
        if (x > max)
            return max;
        return x;
    }
    static const Interval empty, universe;
};

const static Interval empty(+infinity, -infinity);
const static Interval universe(-infinity, +infinity);

Interval operator+(const Interval &i, double offset)
{
    return Interval(i.min + offset, i.max + offset);
}