cmake_minimum_required(VERSION 3.8)

project(FileCopies)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -gdwarf-3")
#set(CMAKE_PREFIX_PATH "/Users/ddulaev/Qt/5.11.2/clang_64/lib/cmake")
set(CMAKE_PREFIX_PATH "/Users/dulaev/Qt/5.12.0/clang_64/lib/cmake")
#set(CMAKE_PREFIX_PATH "C:/Qt/5.12.0/winrt_x86_msvc2017/bin")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Concurrent REQUIRED)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Concurrent_INCLUDE_DIRS})


add_executable(FileCopies
        dglgeometry.cpp
        dglgeometry.h
        dglwidget.cpp
        dglwidget.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        matrix.h
        )

target_link_libraries(FileCopies Qt5::Core)
target_link_libraries(FileCopies Qt5::Widgets)
target_link_libraries(FileCopies Qt5::Concurrent)
target_link_libraries(FileCopies stdc++fs)

qt5_use_modules(FileCopies Widgets)