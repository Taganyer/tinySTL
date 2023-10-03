cmake_minimum_required(VERSION 3.26)
project(tinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(tinySTL main.cpp
        STD/source/String.cpp
        STD/Array.hpp
        STD/Vector.hpp
        STD/Algorithms/source/Container_algorithms.hpp
        STD/Algorithms/source/Algorithms1.cpp
        test.hpp
        STD/Detail/cIter.hpp
        STD/Detail/crIter.hpp
        STD/Iterator.hpp
        STD/List.hpp
        STD/Forward_List.hpp
        STD/Deque.hpp
        STD/Stack.hpp
        STD/Queue.hpp
        STD/Algorithms/Algorithms2.hpp
        STD/Detail/Hashcode.hpp
        STD/Uordered_Map.hpp
        STD/Detail/Red_Black_Tree.hpp
)
