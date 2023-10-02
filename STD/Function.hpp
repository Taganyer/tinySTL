//
// Created by 86152 on 2023/9/21.
//

#ifndef TINYSTL_FUNCTION_HPP
#define TINYSTL_FUNCTION_HPP

#include "Detail/Hashcode.hpp"

namespace STD {

    template<typename Arg>
    struct Less {
        inline bool operator()(const Arg &left, const Arg &right) const {
            return left < right;
        }
    };

    template<typename Arg>
    struct Greater {
        inline bool operator()(const Arg &left, const Arg &right) const {
            return left > right;
        }
    };

    template<typename Arg>
    struct Equal {
        inline bool operator()(const Arg &left, const Arg &right) const {
            return left == right;
        }
    };

    template<typename Arg>
    struct Not_Less_Than {
        inline bool operator()(const Arg &left, const Arg &right) const {
            return left >= right;
        }
    };

    template<typename Arg>
    struct Not_Greater_Than {
        inline bool operator()(const Arg &left, const Arg &right) const {
            return left <= right;
        }
    };
}

#endif //TINYSTL_FUNCTION_HPP
