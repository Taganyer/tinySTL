cmake_minimum_required(VERSION 3.26)
project(tinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        tinySTL main.cpp
        STD/Detail/String.cpp
        STD/Algorithms/Detail/String_Algorithms1.cpp
)
