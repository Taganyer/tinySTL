cmake_minimum_required(VERSION 3.26)
project(tinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(tinySTL main.cpp
        STD/source/String.cpp
        STD/source/Algorithms.cpp
        STD/List.hpp
        STD/Array.hpp
        STD/Deque.hpp
)
