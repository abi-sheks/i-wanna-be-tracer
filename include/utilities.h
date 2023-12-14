#pragma once
#include <limits>
#include <cmath>
#include <cstdlib>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.14159;

inline double degreeToRadians(double degrees)
{
    return (pi * degrees) / 180.0;
}

inline double randomDouble() {
    return rand() / (RAND_MAX + 1.0);
}
inline double randomDouble(double min, double max) {
    return min + (max-min)*randomDouble();
}
inline int randomInt(int min, int max)
{
    //in [min, max]
    return static_cast<int>(randomDouble(min, max+1));
}
inline double linearToGamma(double linear)
{
    return sqrt(linear);
}
inline void swapInt(int*a, int*b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}