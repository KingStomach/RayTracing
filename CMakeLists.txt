cmake_minimum_required (VERSION 3.1)
set (CMAKE_C_STANDARD 14)
set (CMAKE_CXX_STANDARD 14)
project (RayTracing)
add_executable(RayTracing main.cpp HitObject.cpp Render.cpp Material.cpp)
