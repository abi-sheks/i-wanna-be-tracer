#include <iostream>
#include <memory>
#include "volume.h"
#include "camera.h"
#include "texture.h"
#include "sphere.h"
#include "bvh_node.h"
#include "quad.h"
#include "hittable_array.h"

void finalBookOneScene()
{
    HittableArray world;

    // materials and geometries are decoupled, have fun here
    // this sets up the final render in book1, change according to your scene
    auto checker = std::make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(checker)));

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
                    Point3 center_end = center + vec3(0, randomDouble(0, 0.5), 0);
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

    world = HittableArray(std::make_shared<BVHNode>(world));

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

    // for final render
    Camera camera = Camera(16.0 / 9.0, 400, 400, 100, 20.0, Point3(13, 2, 3), Point3(0, 0, 0), vec3(0, 1, 0), 0.6, 10.0, Color(0.7, 0.8, 1.0));
    camera.render(world);
}

void twoSpheres()
{
    HittableArray world;

    auto checker = std::make_shared<CheckerTexture>(0.8, Color(.2, .3, .1), Color(.9, .9, .9));

    world.add(std::make_shared<Sphere>(Point3(0, -10, 0), 10, std::make_shared<Lambertian>(checker)));
    world.add(std::make_shared<Sphere>(Point3(0, 10, 0), 10, std::make_shared<Lambertian>(checker)));

    Camera camera(16.0 / 9.0, 400, 100, 50, 20, Point3(13, 2, 3), Point3(0, 0, 0), vec3(0, 1, 0), 0, 10, Color(0.7, 0.8, 1.0));
    camera.render(world);
}
void earth()
{
    auto earth_texture = std::make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = std::make_shared<Lambertian>(earth_texture);
    auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);
    auto world = HittableArray(globe);

    Camera camera(16.0 / 9.0, 400, 100, 50, 20, Point3(0, 0, 12), Point3(0, 0, 0), vec3(0, 1, 0), 0, 10, Color(0.7, 0.8, 1.0));
    camera.render(world);
}

void perlinSphere()
{
    HittableArray world;

    auto pertext = std::make_shared<NoiseTexture>(4);
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    world.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    Camera camera(16.0 / 9.0, 400, 100, 50, 20, Point3(13, 2, 3), Point3(0, 0, 0), vec3(0, 1, 0), 0, 10, Color(0.7, 0.8, 1.0));
    camera.render(world);
}
void quads()
{
    HittableArray world;

    // Materials
    auto left_red = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto back_green = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto right_blue = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lower_teal = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quads
    world.add(std::make_shared<Quad>(Point3(-3, -2, 5), vec3(0, 0, -4), vec3(0, 4, 0), left_red));
    world.add(std::make_shared<Quad>(Point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(std::make_shared<Quad>(Point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(std::make_shared<Quad>(Point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(std::make_shared<Quad>(Point3(-2, -3, 5), vec3(4, 0, 0), vec3(0, 0, -4), lower_teal));

    Camera camera(1.0, 400, 100, 50, 80, Point3(0, 0, 9), Point3(0, 0, 0), vec3(0, 1, 0), 0, 10, Color(0.7, 0.8, 1.0));
    camera.render(world);
}
void simpleLight()
{
    HittableArray world;

    auto pertext = std::make_shared<NoiseTexture>(4);
    world.add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(pertext)));
    world.add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(pertext)));

    auto difflight = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(Color(4, 4, 4)));
    world.add(std::make_shared<Sphere>(Point3(0, 7, 0), 2, difflight));
    world.add(std::make_shared<Quad>(Point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), difflight));

    Camera camera(16.0 / 9.0, 400, 100, 50, 20, Point3(26, 3, 6), Point3(0, 2, 0), vec3(0, 1, 0), 0, 10, Color(0, 0, 0));
    camera.render(world);
}
void cornellBox()
{
    HittableArray world;

    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(Color(15, 15, 15)));

    world.add(std::make_shared<Quad>(Point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(std::make_shared<Quad>(Point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(555, 555, 555), vec3(-555, 0, 0), vec3(0, 0, -555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    std::shared_ptr<Hittable> box1 = box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(15, box1);
    box1 = std::make_shared<Translate>(box1, vec3(265, 0, 295));
    world.add(box1);

    std::shared_ptr<Hittable> box2 = box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(-18, box2);
    box2 = std::make_shared<Translate>(box2, vec3(130, 0, 65));
    world.add(box2);

    Camera camera(1.0, 600, 100, 50, 40, Point3(278, 278, -800), Point3(278, 278, 0), vec3(0, 1, 0), 0, 10, Color(0, 0, 0));
    camera.render(world);
}
void cornellSmoke()
{
    HittableArray world;

    auto red = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(Color(7, 7, 7)));

    world.add(std::make_shared<Quad>(Point3(555, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), green));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), vec3(0, 555, 0), vec3(0, 0, 555), red));
    world.add(std::make_shared<Quad>(Point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light));
    world.add(std::make_shared<Quad>(Point3(0, 555, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 0), vec3(555, 0, 0), vec3(0, 0, 555), white));
    world.add(std::make_shared<Quad>(Point3(0, 0, 555), vec3(555, 0, 0), vec3(0, 555, 0), white));

    std::shared_ptr<Hittable> box1 = box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(15, box1);
    box1 = std::make_shared<Translate>(box1, vec3(265, 0, 295));

    std::shared_ptr<Hittable> box2 = box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(-18, box2);
    box2 = std::make_shared<Translate>(box2, vec3(130, 0, 65));

    world.add(std::make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
    world.add(std::make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    Camera camera(1.0, 600, 200, 50, 40, Point3(28, 278, -800), Point3(278, 278, 0), vec3(0, 1, 0), 0, 10, Color(0, 0, 0));
    camera.render(world);
}
void finalBookTwoScene()
{
    HittableArray boxes1;
    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++)
    {
        for (int j = 0; j < boxes_per_side; j++)
        {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = randomDouble(1, 101);
            auto z1 = z0 + w;

            boxes1.add(box(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    HittableArray world;

    world.add(std::make_shared<BVHNode>(boxes1));

    auto light = std::make_shared<DiffuseLight>(std::make_shared<SolidColor>(Color(7, 7, 7)));
    world.add(std::make_shared<Quad>(Point3(123, 554, 147), vec3(300, 0, 0), vec3(0, 0, 265), light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    auto sphere_material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.add(std::make_shared<Sphere>(center1, center2, 50, sphere_material));

    world.add(std::make_shared<Sphere>(Point3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    world.add(std::make_shared<Sphere>(
        Point3(0, 150, 145), 50, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)));

    auto boundary = std::make_shared<Sphere>(Point3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5));
    world.add(boundary);
    world.add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere>(Point3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
    world.add(std::make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    auto emat = std::make_shared<Lambertian>(std::make_shared<ImageTexture>("earthmap.jpg"));
    world.add(std::make_shared<Sphere>(Point3(400, 200, 400), 100, emat));
    auto pertext = std::make_shared<NoiseTexture>(0.1);
    world.add(std::make_shared<Sphere>(Point3(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

    HittableArray boxes2;
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 100;
    for (int j = 0; j < ns; j++)
    {
        boxes2.add(std::make_shared<Sphere>(Point3::random(0, 165), 10, white));
    }

    world.add(std::make_shared<Translate>(
        std::make_shared<RotateY>(15,
                                  std::make_shared<BVHNode>(boxes2)),
        vec3(-100, 270, 395)));

    Camera camera(1.0, 400, 100, 4, 40, Point3(478, 278, -600), Point3(278, 278, 0), vec3(0, 1, 0), 0, 10, Color(0, 0, 0));
    camera.render(world);
}
int main()
{
    switch (9)
    {
    case 1:
        finalBookOneScene();
        break;
    case 2:
        twoSpheres();
        break;
    case 3:
        earth();
        break;
    case 4:
        perlinSphere();
        break;
    case 5:
        quads();
        break;
    case 6:
        simpleLight();
        break;
    case 7:
        cornellBox();
        break;
    case 8:
        cornellSmoke();
        break;
    case 9:
        finalBookTwoScene();
        break;
    }
}