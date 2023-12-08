#pragma once

#include "vec3.h"
#include "interval.h"
#include "utilities.h"
#include <iostream>


using Color = vec3;

void writeColor(std::ostream &out, Color pixel_color, int samples_per_pixel)
{
    // pixel_color comes from getRay
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // taking average of all sample values
    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    //convert with gamma 2
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    // to clamp values bw 0.0 to 1.0
    static Interval intensity(0.000, 0.999);
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}