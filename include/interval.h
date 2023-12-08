#pragma once
#include "utilities.h"

class Interval
{
public:
    double min, max;
    Interval() : min(-infinity), max(+infinity) {}
    Interval(double _min, double _max) : min(_min), max(_max) {}

    bool contains(const double x)
    {
        return x >= min && x <= max;
    }
    bool surrounds(const double x)
    {
        return x > min && x < max;
    }
    double clamp(double x) {
        if(x < min) return min;
        if(x > max) return max;
        return x;
    }
    static const Interval empty, universe;
};

const static Interval empty(+infinity, -infinity);
const static Interval universe(-infinity, +infinity);