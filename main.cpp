#include <iostream>
#include <memory>
#include "camera.h"
#include "sphere.h"
#include "hittable_array.h"

int main()
{
    HittableArray world;
    world.add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera camera(16.0 / 9.0, 400, 100);
    camera.render(world);

}