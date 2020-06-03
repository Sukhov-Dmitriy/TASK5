cmake_minimum_required(VERSION 3.5)

project(task5 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task5 box.h box.cpp CImg.h Figure.h FigureFactory.h functions.h functions.cpp functions1.h ptr.h ptr.cpp settings.h settings.cpp sphere.h sphere.cpp tetra.h tetra.cpp)
