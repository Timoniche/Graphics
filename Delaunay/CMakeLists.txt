cmake_minimum_required(VERSION 3.13)
project(sfml)

set(CMAKE_CXX_STANDARD 17)

# SFML
## If you want to link SFML statically
set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
add_executable(sfml main.cpp
        delaunay.h
        delaunay.cpp
        edge.h
        edge.cpp
        numeric.h
        numeric.cpp
        triangle.h
        triangle.cpp
        vector2.h
        vector2.cpp circumcenter.h
        #graham.cpp
        )
target_link_libraries(sfml sfml-system sfml-window sfml-graphics sfml-network sfml-audio)