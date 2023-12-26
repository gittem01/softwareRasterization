cmake_minimum_required(VERSION 3.10)

project(FastRasterizer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(${CMAKE_SOURCE_DIR}/external/SDL)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cglm)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/external/SDL/include)
include_directories(${CMAKE_SOURCE_DIR}/external/cglm/include)
include_directories(${CMAKE_SOURCE_DIR}/external/stb_image)

add_executable(FastRasterizer main.c "external/stb_image/stb_image.h")

target_link_libraries(FastRasterizer SDL2-static)
