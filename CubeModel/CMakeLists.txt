cmake_minimum_required(VERSION 3.9)
project("Graphics Demo")

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp Graphics.cpp)
set(CMAKE_PREFIX_PATH "C:/freeglut")

set(FREEGLUT_INCLUDE_DIRS "C:/freeglut/include")
set(FREEGLUT_LIBRARY_DIRS "C:/freeglut/lib/")

set(FREEGLUT_LIBRARIES freeglut)
set(OpenGL_LIBRARIES glu32 opengl32)

find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

link_directories(${OPENGL_gl_LIBRARY})
include_directories(${FREEGLUT_INCLUDE_DIRS})
link_directories(${FREEGLUT_LIBRARY_DIRS})

add_executable(cube RotatingCube.cpp Graphics.h Graphics.cpp)
add_executable(demo ${SOURCE_FILES})
target_link_libraries(demo libfreeglut.a libfreeglut_static.a ${FREEGLUT_LIBRARIES} ${OpenGL_LIBRARIES})
target_link_libraries(cube libfreeglut.a libfreeglut_static.a ${FREEGLUT_LIBRARIES} ${OpenGL_LIBRARIES})