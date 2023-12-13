#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb_image.h"
#include <cstdlib>
#include <iostream>

class Image
{
public:
    Image() : data(nullptr) {}
    Image(const char *file_name)
    {
        auto filename = std::string(file_name);
        //taking advantage of short circuiting here
        if (load("../images/" + filename) || load("./images/" + filename) || load("../../images/" + filename))
        {
            return;
        }
        std::cerr << "Failed to load image" << filename << '\n';
    }
    ~Image() { STBI_FREE(data); }
    bool load(const std::string file_name)
    {
        auto n = bpp; // required by stbi_load, useless otherwise
        data = stbi_load(file_name.c_str(), &img_width, &img_height, &n, bpp);
        bpscanline = img_width * bpp; // required for pixel data offset
        return data != nullptr;
    }
    const unsigned char *pixelData(int x, int y) const
    {
        // for no data case, magenta
        static unsigned char magenta[] = {255, 0, 255};
        if (data == nullptr)
            return magenta;
        x = clamp(x, 0, img_width);
        y = clamp(y, 0, img_height);
        return data + (bpscanline * y) + (bpp * x);
    }
    int width() const { return (data == nullptr) ? 0 : img_width; }
    int height() const { return (data == nullptr) ? 0 : img_height; }

private:
    const int bpp = 3;
    unsigned char *data;
    int img_width;
    int img_height;
    int bpscanline;

    static int clamp(int x, int low, int high)
    {
        if (x >= high)
            x = high - 1;
        if (x < low)
            x = low;
        return x;
    }
};