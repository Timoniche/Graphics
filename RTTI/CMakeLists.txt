cmake_minimum_required(VERSION 3.13)
project(Graphics)

set(CMAKE_CXX_STANDARD 17)

include_directories(${Graphics_SOURCE_DIR})

add_executable(Graphics main.cpp DynamicCastObject.h)
add_executable(DinamicCastTest DynamicCastTest.cpp DynamicCastObject.h
        gtest/gtest.h
        gtest/gtest-all.cc
        gtest/gtest_main.cc)
add_executable(DCOBits DCOBitsTest.cpp DCOBits.h
        gtest/gtest.h
        gtest/gtest-all.cc
        gtest/gtest_main.cc)