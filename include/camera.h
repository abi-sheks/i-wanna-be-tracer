#pragma once
#include "sphere.h"
#include "hittable.h"
#include "hittable_array.h"
#include "utilities.h"
#include "interval.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "material.h"

class Camera
{
public:
    double aspect_ratio = 16.0 / 9.0;
    int img_width = 800;
    int samples_per_pixel = 100;
    int max_depth = 20;
    double vfov = 20.0;
    Point3 lookFrom = Point3(-2, 2, 1); // Point camera is looking from
    Point3 lookAt = Point3(0, 0, -1);    // Point camera is looking at
    vec3 vup = vec3(0, 1, 0);
    double defocus_angle = 0;
    double focus_distance = 10;

    Camera() {}
    Camera(double _ar, int _img_width, int _samples_per_pixel, int _max_depth, double _vfov, Point3 _lookFrom, Point3 _lookAt, vec3 _vup, double _da, double _fd) :                                                                         aspect_ratio(_ar), 
                                                                                               img_width(_img_width),
                                                                                               samples_per_pixel(_samples_per_pixel),
                                                                                               max_depth(_max_depth),
                                                                                               vfov(_vfov),
                                                                                               lookFrom(_lookFrom),
                                                                                               lookAt(_lookAt),
                                                                                               vup(_vup),
                                                                                               defocus_angle(_da),
                                                                                               focus_distance(_fd) {}

    void render(Hittable &world)
    {
        initialize();

        std::cout << "P3\n"
                  << img_width << " " << img_height << "\n255\n";

        for (int j = 0; j < img_height; ++j)
        {
            std::clog << "\rLines remaining" << (img_height - j) << ' ' << std::flush;
            for (int i = 0; i < img_width; ++i)
            {
                // for normalization to 0.0.to 1.0 then map to 0 to 255 inside writeColor
                Color pixel_color(0, 0, 0);
                for (int s = 0; s < samples_per_pixel; s++)
                {
                    // returns and adds random sample from 0.5 square with pixel at centre
                    Ray r = getRay(i, j);
                    pixel_color += rayColor(r, max_depth, world);
                }
                writeColor(std::cout, pixel_color, samples_per_pixel);
            }
        }
        std::clog << "\rDone.           \n";
    }

private:
    int img_height;        // Rendered image height
    Point3 camera_center;  // Camera center
    Point3 pixel_top_left; // Location of pixel 0, 0
    vec3 pixel_distance_u; // Offset to pixel to the right
    vec3 pixel_distance_v; // Offset to pixel below

    //defocus disk
    vec3 defocus_disk_u;
    vec3 defocus_disk_v;

    // to achieve old orthographic view, make u,v,w unit axis vectors by adjusting lookFrom = (0, 0, -1), lookAt = (0, 0, 0) and cameraUp = (0, 1, 0)
    vec3 u, v, w; // camera basis vectors, v - cameraUp projected orthonormal to view dir, w - along view dir, u - cameraRight
    void initialize()
    {
        img_height = static_cast<int>(img_width / aspect_ratio);
        img_height = img_height < 1 ? 1 : img_height;

        // real value viewport dimensions are ok, img dimensions are not (they represent number of pixels)
        camera_center = lookFrom;
        // fov calcs
        double theta = degreeToRadians(vfov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h * focus_distance;
        auto viewport_width = viewport_height * (static_cast<double>(img_width) / img_height);

        // basis vectors
        w = normalize(lookFrom - lookAt); // negative of usual to account for right handedness
        u = normalize(vup.cross(w));
        v = w.cross(u);

        // following calculations assume pixel grid is inset from viewport edges by half pixel to pixel distance ()
        // horizontal and vertical vectors
        auto viewport_u = viewport_width * u;   // for our original view, u and v were (1, 0, 0) and (0, 1, 0) obviously.
        auto viewport_v = -viewport_height * v; // this is the generalization for any view

        // corresponding pixel delta vectors (pixel to pixel distance )
        pixel_distance_u = viewport_u / img_width;
        pixel_distance_v = viewport_v / img_height;

        // upper left pixel position, basic vector algebra
        // camera to frame is along negative z, right handed system
        auto viewport_upleft_edge = camera_center - (focus_distance * w) - (viewport_u / 2) - (viewport_v / 2); // in basic view w is (0, 0, 1)
        pixel_top_left = viewport_upleft_edge + (0.5 * pixel_distance_u) + (0.5 * pixel_distance_v);

        //defocus disk basis vectors
        auto defocus_radius = focus_distance * tan(degreeToRadians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }
    Color rayColor(const Ray &r, int depth, const Hittable &world)
    {
        hit_info info;

        // to prevent that nasty seg fault u were getting
        if (depth <= 0)
        {
            return Color(0, 0, 0);
        }
        // interval starts from small t to fix shadow acne
        // shadow acne was actually the issue causing major runtime lag and visual defect for me.
        // reintersection is a bitch.
        if (world.hit(r, Interval(0.001, infinity), info))
        {
            // understand the geometric meaning of this recursive call, basically how the rays travel on each rayColor
            // first call from render will give ray bw camera and sample point
            // this internal call simulates the ray bouncing off randomly. It'll most probably not collide elsewhere so it'll move on to the
            // below lerp and provide a color based on its random y value.
            Ray scattered;
            Color attenuation; // learn what these are in depth asap
            // so for lambertian, we took attenuation = 0.5, scattered ofc based on principle
            if (info.mat->scatter(r, info, attenuation, scattered))
            {
                return attenuation * rayColor(scattered, depth - 1, world);
            }
            return Color(0, 0, 0);
        }
        // lerps based on y value
        vec3 unit_direction = normalize(r.direction());
        // for parameter a
        auto a = 0.5 * (unit_direction.y() + 1.0);
        // start color (a = 0) - white, end color (a = 1) - blue
        return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
    }

    Ray getRay(int i, int j)
    {
        // returns ray per sample
        auto pixel_center = pixel_top_left + (i * pixel_distance_u) + (j * pixel_distance_v);
        auto pixel_sample = pixel_center + pixelSampleSquare();

        //gets a random point on defocus disk, or camera center if angle <= 0.
        auto origin = (defocus_angle <= 0) ? camera_center : defocusDiskSample();
        auto direction_from_cam = pixel_sample - camera_center;
        return Ray(origin, direction_from_cam);
    }
    vec3 pixelSampleSquare()
    {
        // returns random point in 0.5 square vicinity
        auto px = -0.5 + randomDouble();
        auto py = -0.5 + randomDouble();
        return (px * pixel_distance_u) + (py * pixel_distance_v);
    }
        Point3 defocusDiskSample() const {
        // Returns a random point in the camera defocus disk.
        auto p = randomInUnitDisk();
        return camera_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
};