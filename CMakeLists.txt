cmake_minimum_required(VERSION 3.26)
project(tinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(tinySTL main.cpp
        STD/Allocater.hpp
        STD/Warning.hpp
        STD/Iter.hpp
        STD/Vector.hpp
        STD/Move.hpp
        STD/Memory.hpp
        STD/String.hpp
        STD/source/String.cpp
        STD/Algorithms.hpp
        STD/source/Algorithms.cpp
        STD/List.hpp
        STD/List.hpp
        STD/Initializer_list.hpp
        STD/Forward_list.hpp
        STD/Forward_list.hpp
)
