//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_ALLOCATER_HPP
#define TINYSTL_ALLOCATER_HPP

#include "warning.hpp"

namespace STD {
    using Size = unsigned long long;

    template<typename Arg>
    Arg* Allocate(Arg arg) {
        auto* ptr = new Arg(arg);
        if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
        return ptr;
    }

    template<typename Arg>
    void Deallocate(Arg* ptr) {
        delete ptr;
    }

    template<typename Arg>
    Arg* Allocate_n(Size size) {
        if (!size) return nullptr;
        else {
            auto* ptr = new Arg[size];
            if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
            return ptr;
        }
    }

    template<typename Arg>
    void Deallocate_n(Arg* ptr) {
        delete[] ptr;
    }
}

#endif //TINYSTL_ALLOCATER_HPP
