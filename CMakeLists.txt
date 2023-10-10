cmake_minimum_required(VERSION 3.26)
project(tinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(tinySTL main.cpp
        STD/Detail/String.cpp
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
        STD/Unordered_Map.hpp
        STD/Detail/Red_Black_Tree.hpp
        STD/Detail/AVL_Tree.hpp
        STD/Map.hpp
        STD/Set.hpp
        STD/Detail/Hashtable.hpp
        STD/Unordered_Set.hpp
        STD/Detail/MultiRed_Balck_Tree.hpp
        STD/Detail/MultiHashtable.hpp
        STD/Memory.hpp
)
