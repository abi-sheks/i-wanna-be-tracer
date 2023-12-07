#pragma once

#include <iostream>
#include <cmath>

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
};

using point3 = vec3; // alias for readability

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

//need to define here as it depends on above overload
inline vec3 normalize(vec3 v) {
    return v / v.length();
}
