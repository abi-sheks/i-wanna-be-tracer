#include <iostream>
#include <memory>
#include "camera.h"
#include "sphere.h"
#include "hittable_array.h"

int main()
{
    HittableArray world;

    // materials and geometries are decoupled, have fun here
    // this sets up the final render in the book, change according to your scene
    auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            auto choose_mat = randomDouble();
            Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8)
                {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    double aspect_ratio;   // imagewidth / imageheight
    int img_width;         // Rendered image width in pixel count
    int samples_per_pixel; // Count of random samples for each pixel, keep low for lower quality render
    int max_depth;         // Maximum number of ray bounces into scene, keep low for low performance hardware and faster render time, can segfault
    double vfov;           // Vertical view angle (field of view)
    Point3 lookFrom;       // Point camera is looking from
    Point3 lookAt;         // Point camera is looking at
    vec3 vup;              // Camera-relative "up" direction
    double defocus_angle;  // Variation angle of rays through each pixel
    double focus_distance; // Distance from camera lookfrom point to plane of perfect focus

    // Use this constructor after filling above values accordingly for different renders and comment out default
    // Camera camera = Camera(aspect_ratio, img_width, samples_per_pixel, max_depth, vfov, lookFrom, lookAt, vup, defocus_angle, focus_distance)

    // or use the default render
    // Camera camera = Camera();

    //for final render
    Camera camera = Camera(16.0 / 9.0, 400, 100, 10, 20.0, Point3(13, 2, 3), Point3(0, 0, 0), vec3(0, 1, 0), 0.6, 10.0);
    camera.render(world);
}