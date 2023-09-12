//
// Created by 86152 on 2023/9/12.
//

#ifndef TINYSTL_INITIALIZER_LIST_HPP
#define TINYSTL_INITIALIZER_LIST_HPP

#include "Allocater.hpp"

namespace STD {
    template<typename Type>
    class Initializer_list {
    private:

        Size size_ = 0;

        Type *array = nullptr;

        template<typename Arg>
        void init(Type *&ptr, const Arg &arg) const {
            *ptr = Arg(arg);
        }

        template<typename ...Args, typename Arg>
        void init(Type *&ptr, const Arg &arg, const Args &...args) const {
            *ptr = Arg(arg);
            init(++ptr, args...);
        }

    public:

        constexpr Initializer_list() noexcept = default;

        template<typename ...Args>
        explicit Initializer_list(const Args &...args) : size_(sizeof...(args)), array(Allocate_n<Type>(size_)) {
            auto temp = array;
            init(temp, args...);
        };

        Pair<Type *, Size> Move() {
            Type *temp = array;
            array = nullptr;
            return {temp, size_};
        };

        constexpr Size Size() const noexcept { return size_; };

        constexpr const Type *begin() const noexcept { return array; };

        constexpr const Type *end() const noexcept { return array + size_; };
    };

    template<typename Type>
    constexpr const Type *begin(Initializer_list<Type> list) noexcept { return list.begin(); }

    template<typename Type>
    constexpr const Type *end(Initializer_list<Type> list) noexcept { return list.end(); }
}

#endif //TINYSTL_INITIALIZER_LIST_HPP
