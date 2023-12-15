A simple raytracer built without any graphics APIs, as detailed in [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html) and [_Ray Tracing: The Next Week_](https://raytracing.github.io/books/RayTracingTheNextWeek.html)
. It did, in fact, take nearly a weekend (+ another 5-6 days).  
The project is purely educational, I was interested in how raytracing worked in old Wolfenstein-style games, and wanted to write my own. Though this doesnt resemble a project of that sort, im happy with the result and learning.

## Setup
Make sure you have CMake installed, as that is the build system used here. (You can build the project hassle free with just ```g++``` or a Makefile too, there are zero external dependencies, and most of the code is in header only files) The program works by writing to a .ppm file, so make sure you have an image loader for that format.  
- Create a build directory for your executable using ```mkdir build```
- ```cd``` into the ```/build``` directory
- Run ```cmake ../``` and ```cmake --build ..``` to build your executable.
- Run the program and write the contents (after specifying which scene you would want to render in ```main.cpp```) into a ppm file. On bash, this is ```./Raycaster > render.ppm```
- You can view some of my renders from the book by running ```feh``` on the renders inside the ```\renders``` directory.
- View the render with an image viewer of your choice, ex :- ```feh render.ppm```

## Future plans
- Satisfied with how much I've learnt, need to dig further into the math behind Perlin noise, maybe will add other geometric primitives, rotations when free, but not very likely.
