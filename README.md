A simple raytracer built without any graphics APIs, as detailed in [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html). It did, in fact, take nearly a weekend.  
The project is purely educational, I was interested in how raytracing worked in old DOOM/Wolfenstein-style games, and wanted to write my own.

## Setup
Make sure you have CMake installed, as that is the build system used here. The program works by writing to a .ppm file, so make sure you have an image loader for that format.  
- Create a build directory for your executable using ```mkdir build```
- ```cd``` into the ```/build``` directory
- Run ```cmake ../``` and ```cmake --build ..``` to build your executable.
- Run the program and write the contents into a ppm file. On bash, this is ```./Raycaster > render.ppm```
- View the render with an image viewer of your choice, ex :- ```feh render.ppm```

## Future plans
- Add more features like lighting, textures, etc
