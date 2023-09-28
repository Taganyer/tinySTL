//
// Created by 86152 on 2023/8/23.
//

#ifndef TINYSTL_MOVE_HPP
#define TINYSTL_MOVE_HPP

#include "Config.hpp"
#include "Type_traits.hpp"

namespace STD {

    template<typename Arg>
    constexpr typename Remove_reference<Arg>::Type &&move(Arg &&arg) noexcept {
        return static_cast<typename Remove_reference<Arg>::Type &&>(arg);
    }

    template<typename Arg>
    constexpr Arg &&forward(typename Remove_reference<Arg>::Type &arg) noexcept {
        return static_cast<Arg &&>(arg);
    }

    template<typename Arg>
    void swap(Arg &left, Arg &right) noexcept {
        Arg temp = STD::move(left);
        left = STD::move(right);
        right = STD::move(temp);
    }

    template<typename Arg, Size size_>
    void swap(Arg (&left)[size_], Arg (&right)[size_]) noexcept {
        for (Size i = 0; i < size_; ++i) {
            STD::swap(left[i], right[i]);
        }
    }
}

#endif //TINYSTL_MOVE_HPP
