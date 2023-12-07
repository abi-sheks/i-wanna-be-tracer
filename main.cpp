#include <iostream>
#include "vec3.h"
#include "color.h"

int main()
{
    int img_width = 256;
    int img_height = 256;

    std::cout << "P3\n"
              << img_width << " " << img_height << "\n255\n";

    for (int j = 0; j < img_height; ++j)
    {
        std::clog<< "\rLines remaining" << (img_height - j) << ' ' << std::flush; 
        for (int i = 0; i < img_width; ++i)
        {
            //for normalization to 0.0.to 1.0 then map to 0 to 255 inside writeColor
            auto pixel_color = color(double(i) / (img_width - 1), double(j) / (img_height - 1), 0);
            writeColor(std::cout, pixel_color);
        }
    }

    std::clog<<"\rDone.           \n";
}