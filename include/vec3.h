#pragma once

#include <iostream>
#include <cmath>
#include "utilities.h"

using std::sqrt;

class vec3
{
public:
    double comp[3];

    vec3() : comp{0, 0, 0} {}
    vec3(double c1, double c2, double c3) : comp{c1, c2, c3} {}

    double x() const { return comp[0]; }
    double y() const { return comp[1]; }
    double z() const { return comp[2]; }

    // keeping these inline for performance, very simple methods, no need to define in source
    vec3 operator-() const
    {
        return vec3(-comp[0], -comp[1], -comp[2]);
    }
    double operator[](int i) const { return comp[i]; }
    double &operator[](int i) { return comp[i]; }

    vec3 &operator+=(const vec3 &v)
    {
        comp[0] += v.comp[0];
        comp[1] += v.comp[1];
        comp[2] += v.comp[2];
        return *this;
    }

    vec3 &operator*=(double val)
    {
        comp[0] *= val;
        comp[1] *= val;
        comp[2] *= val;
        return *this;
    }

    vec3 &operator/=(double val)
    {
        return *this *= 1 / val;
    }
    double sqrLength()
    {
        return (comp[0] * comp[0]) + (comp[1] * comp[1]) + (comp[2] * comp[2]);
    }

    double length()
    {
        return sqrt(sqrLength());
    }

    double dot(const vec3 &v)
    {
        return (comp[0] * v[0]) + (comp[1] * v[1]) + (comp[2] * v[2]);
    }
    vec3 cross(const vec3 &v)
    {
        return vec3(comp[1] * v.comp[2] - comp[2] * v.comp[1],
                    comp[2] * v.comp[0] - comp[0] * v.comp[2],
                    comp[0] * v.comp[1] - comp[1] * v.comp[0]);
    }
    static vec3 random()
    {
        return vec3(randomDouble(), randomDouble(), randomDouble());
    }
    static vec3 random(double min, double max)
    {
        return vec3(randomDouble(min, max), randomDouble(min, max), randomDouble(min, max));
    }
    bool near_zero() const
    {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (fabs(comp[0]) < s) && (fabs(comp[1]) < s) && (fabs(comp[2]) < s);
    }
};

using Point3 = vec3; // alias for readability

inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
{
    return out << v.comp[0] << ' ' << v.comp[1] << ' ' << v.comp[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.comp[0] + v.comp[0], u.comp[1] + v.comp[1], u.comp[2] + v.comp[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.comp[0] - v.comp[0], u.comp[1] - v.comp[1], u.comp[2] - v.comp[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.comp[0] * v.comp[0], u.comp[1] * v.comp[1], u.comp[2] * v.comp[2]);
}

// not idempotent in type unfortunately :(
inline vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.comp[0], t * v.comp[1], t * v.comp[2]);
}

inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
}

inline vec3 operator/(vec3 v, double t)
{
    return (1 / t) * v;
}

// need to define here as it depends on above overload
inline vec3 normalize(vec3 v)
{
    return v / v.length();
}

inline vec3 randomInUnitSphere()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);
        if (p.sqrLength() < 1)
        {
            return p;
        }
    }
}
inline vec3 randomUnitVec()
{
    return normalize(randomInUnitSphere());
}
inline vec3 randomOnHemisphere(const vec3 &normal)
{
    vec3 on_unit_sphere = randomUnitVec();
    if (on_unit_sphere.dot(normal) > 0.0)
    {
        return on_unit_sphere;
    }
    else
    {
        return -on_unit_sphere;
    }
}

vec3 reflect(vec3 &v, const vec3 &normal)
{
    return v - 2 * v.dot(normal) * normal;
}