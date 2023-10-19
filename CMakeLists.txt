cmake_minimum_required(VERSION 3.26)
project(tinySTL)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        tinySTL main.cpp
        STD/Detail/String.cpp
        STD/Algorithms/Detail/Algorithms.cpp
)
#STD/Algorithms.hpp
#STD/Memory.hpp
#STD/Detail/Iter.hpp
#STD/Detail/cIter.hpp
#STD/Detail/crIter.hpp
#STD/Iterator.hpp
#STD/Detail/Hashcode.hpp
#STD/Detail/Red_Black_Tree.hpp
#STD/Detail/AVL_Tree.hpp
#STD/Detail/Hashtable.hpp
#STD/Detail/MultiRed_Black_Tree.hpp
#STD/Detail/MultiHashtable.hpp
#STD/Array.hpp
#STD/Vector.hpp
#STD/Deque.hpp
#STD/List.hpp
#STD/Forward_List.hpp
#STD/Stack.hpp
#STD/Queue.hpp
#STD/Map.hpp
#STD/Set.hpp
#STD/Unordered_Map.hpp
#STD/Unordered_Set.hpp
#STD/Tuple.hpp
#STD/BitSet.hpp
#test.hpp
