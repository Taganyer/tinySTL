//
// Created by 86152 on 2023/8/23.
//

#ifndef TINYSTL_MOVE_HPP
#define TINYSTL_MOVE_HPP

#include <type_traits>

namespace STD {
    template<typename Arg>
    struct remove_reference {
        using type = Arg;
    };

    template<typename Arg>
    struct remove_reference<Arg &> {
        using type = Arg;
    };

    template<typename Arg>
    struct remove_reference<Arg &&> {
        using type = Arg;
    };

    template<typename Arg>
    constexpr typename remove_reference<Arg>::type &&move(Arg &&arg) noexcept {
        return static_cast<typename remove_reference<Arg>::type &&>(arg);
    }

    template<typename Arg>
    constexpr Arg &&forward(typename remove_reference<Arg>::type &arg) noexcept {
        return static_cast<Arg &&>(arg);
    }

    template<typename Arg>
    void swap(Arg &left, Arg &right) noexcept {
        Arg temp = move(left);
        left = move(right);
        right = move(temp);
    }
}

#endif //TINYSTL_MOVE_HPP
