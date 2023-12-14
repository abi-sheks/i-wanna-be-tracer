#pragma once
#include <memory>
#include "vec3.h"
#include "image.h"
#include "color.h"
#include "perlin.h"

class Texture
{
public:
    virtual ~Texture() = default;

    virtual Color value(double u, double v, const Point3 &point) const = 0;
};

class SolidColor : public Texture
{
public:
    SolidColor(Color _color) : color(_color) {}
    SolidColor(double r, double g, double b) : color(Color(r, g, b)) {}
    Color value(double u, double v, const Point3 &point) const override
    {
        // just returns a constant colour
        return color;
    }

private:
    Color color;
};

class CheckerTexture : public Texture
{
public:
    CheckerTexture(double scale, std::shared_ptr<Texture> _even, std::shared_ptr<Texture> _odd) : odd(_odd), even(_even), inverse_scale(1.0 / scale) {}
    CheckerTexture(double scale, Color c1, Color c2) : inverse_scale(1.0 / scale), even(std::make_shared<SolidColor>(c1)), odd(std::make_shared<SolidColor>(c2)) {}

    // spatial texture, does not depend on (u, v)
    Color value(double u, double v, const Point3 &point) const override
    {
        auto xInt = static_cast<int>(std::floor(inverse_scale * point.x()));
        auto yInt = static_cast<int>(std::floor(inverse_scale * point.y()));
        auto zInt = static_cast<int>(std::floor(inverse_scale * point.z()));

        bool isEven = (xInt + yInt + zInt) % 2;

        return isEven ? even->value(u, v, point) : odd->value(u, v, point);
    }

private:
    double inverse_scale;
    std::shared_ptr<Texture> even;
    std::shared_ptr<Texture> odd;
};

class ImageTexture : public Texture
{
public:
    ImageTexture(const char *filename) : image(filename) {}
    Color value(double u, double v, const Point3 &point) const override
    {
        // if no image loaded, default is cyan
        if (image.height() <= 0)
            return Color(0, 1, 1);
        // clamping input texels to [0, 1] x [1, 0]
        u = Interval(0, 1).clamp(u);
        v = 1.0 - Interval(0, 1).clamp(v);

        // converting to image coordinates
        auto i = static_cast<int>(u * image.width());
        auto j = static_cast<int>(v * image.height());
        auto pixel = image.pixelData(i, j);
        auto color_scale = 1.0 / 255.0;
        // to scale down rgb values bw 0 and 1.
        return Color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }

private:
    Image image;
};

class NoiseTexture : public Texture
{
public:
    NoiseTexture() : frequency(1) {}
    NoiseTexture(double _frequency) : frequency(_frequency) {}
    Color value(double u, double v, const Point3 &point) const override
    {
        //0.5*(1.0 + )to handle cases where perlin interpretation gives negative value, (sqrt of that will give NaN)
        //marble texture achieved by making color proportional to some sine function, and shifting its phase by turbulence
        auto scaled = frequency*point;
        return Color(1.0, 1.0, 1.0)*0.5*(1.0 + sin(scaled.z() + 10*noise.turbulence(scaled)));
    }

private:
    Perlin noise;
    double frequency;
};