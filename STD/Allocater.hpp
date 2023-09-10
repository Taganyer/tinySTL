//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_ALLOCATER_HPP
#define TINYSTL_ALLOCATER_HPP

#include "Warning.hpp"
#include "Move.hpp"

namespace STD {
    using Size = unsigned long long;

    const int MAX_INT = 2147483647;

    const int MIN_INT = -2147483648;

    const unsigned int MAX_UINT_INT = 4294967295;

    const long long MAX_LLONG = 9223372036854775807;

    const long long MIN_LLONG = -9223372036854775808;

    const unsigned long long MAX_UINT_LLONG = 18446744073709551615;

    template<typename Arg>
    Arg* Allocate(const Arg& arg) {
        auto* ptr = new Arg(arg);
        if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
        return ptr;
    }

    template<typename Arg>
    Arg* Allocate(Arg&& arg) {
        auto* ptr = new Arg(move(arg));
        if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
        return ptr;
    }

    template<typename Arg, typename ...Args>
    Arg* Allocate(Args&& ...args) {
        auto* ptr = new Arg(move(args)...);
        if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
        return ptr;
    }

    template<typename Arg>
    void Deallocate(Arg* ptr) {
        delete ptr;
    }

    template<typename Arg>
    void constDeallocate(const Arg* ptr) {
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

    template<typename Arg>
    struct Default_delete {
        constexpr Default_delete() noexcept = default;
        void operator()(Arg *ptr) const {
            Deallocate(ptr);
        }
    };

    template<typename Arg>
    void Memset(Arg *target, const Arg &val, Size size) {
        for (int i = 0; i < size; ++i) {
            *target = val;
            ++target;
        }
    }
}

#endif //TINYSTL_ALLOCATER_HPP
