cmake_minimum_required (VERSION 3.1)
set (CMAKE_C_STANDARD 14)
set (CMAKE_CXX_STANDARD 14)
project (RayTracing)

include_directories("${PROJECT_SOURCE_DIR}/")
include_directories("${PROJECT_SOURCE_DIR}/Image")
include_directories("${PROJECT_SOURCE_DIR}/Math")

add_subdirectory(Image)

add_executable(RayTracing main.cpp)

target_link_libraries(RayTracing Image)