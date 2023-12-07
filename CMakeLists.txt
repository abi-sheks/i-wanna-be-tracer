cmake_minimum_required(VERSION 3.10)
project(Raycaster)
add_executable(Raycaster main.cpp)

add_subdirectory(include)
target_link_libraries(Raycaster PUBLIC include)
target_include_directories(Raycaster PUBLIC
                           "./build"
                           "./include")
   
