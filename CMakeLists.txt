cmake_minimum_required(VERSION 3.26)
project(tinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(tinySTL main.cpp
        STD/source/String.cpp
        STD/Algorithms/source/Algorithms1.cpp
        STD/List.hpp
        STD/Array.hpp
        STD/Deque.hpp
        STD/Stack.hpp
        STD/Function.hpp
        STD/Queue.hpp
        STD/Initializer_list.hpp
)
