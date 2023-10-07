//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_ALLOCATER_HPP
#define TINYSTL_ALLOCATER_HPP

#include "Move.hpp"
#include "Warning.hpp"

namespace STD {

    template<typename Arg>
    Arg *Allocate(const Arg &arg) {
        auto *ptr = new Arg(arg);
        if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
        return ptr;
    }

    template<typename Arg>
    Arg *Allocate(Arg &&arg) {
        auto *ptr = new Arg(move(arg));
        if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
        return ptr;
    }

    template<typename Arg, typename ...Args>
    Arg *Allocate(Args &&...args) {
        auto *ptr = new Arg(move(args)...);
        if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
        return ptr;
    }

    template<typename Arg>
    void Deallocate(Arg *ptr) {
        delete ptr;
    }

    template<typename Arg>
    void constDeallocate(const Arg *ptr) {
        delete ptr;
    }

    template<typename Arg>
    Arg *Allocate_n(Size size) {
        if (!size) return nullptr;
        else {
            auto *ptr = new Arg[size];
            if (!ptr) throw badAlloc("badAlloc: Memory allocation failed\n");
            return ptr;
        }
    }

    template<typename Arg>
    void Deallocate_n(Arg *ptr) {
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
        while (size) {
            *target = val;
            ++target, --size;
        }
    }

    template<typename First, typename Second>
    struct Pair {
        First first;

        Second second;

        Pair(const First &first = First(), const Second &second = Second()) : first(first), second(second) {};

        Pair(First &&first, const Second &second) : first(move(first)), second(second) {};

        Pair(const First &first, Second &&second) : first(first), second(move(second)) {};

        Pair(First &&first, Second &&second) : first(move(first)), second(move(second)) {};

        friend bool operator==(const Pair<First, Second> &left, const Pair<First, Second> &right) {
            return left.first == right.first && left.second == right.second;
        }

        friend bool operator!=(const Pair<First, Second> &left, const Pair<First, Second> &right) {
            return left.first != right.first || left.second != right.second;
        }

        friend bool operator<(const Pair<First, Second> &left, const Pair<First, Second> &right) {
            if (left.first < right.first || left.first == right.first && left.second < right.second)
                return true;
            return false;
        }

        friend bool operator<=(const Pair<First, Second> &left, const Pair<First, Second> &right) {
            return !(left > right);
        }

        friend bool operator>(const Pair<First, Second> &left, const Pair<First, Second> &right) {
            if (left.first > right.first || left.first == right.first && left.second > right.second)
                return true;
            return false;
        }

        friend bool operator>=(const Pair<First, Second> &left, const Pair<First, Second> &right) {
            return !(left < right);
        }
    };
}

#endif //TINYSTL_ALLOCATER_HPP
