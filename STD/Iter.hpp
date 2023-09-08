//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_ITER_HPP
#define TINYSTL_ITER_HPP

#include "Memory.hpp"

namespace STD {

    template<typename Type>
    class Iter {
    protected:
        Type *target = nullptr;

    public:
        //这个函数是为了使迭代器能够在用户代码中保持多态性而设立。
        virtual Shared_ptr<Iter<Type>> deep_copy() const;

        explicit Iter(Type *ptr) : target(ptr) {};

        virtual ~Iter() = default;

        virtual Type &operator*() const { return *target; };

        virtual Iter<Type> &operator++() &{
            ++target;
            return *this;
        };

        inline friend bool operator==(const Iter<Type> &left, const Iter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const Iter<Type> &left, const Iter<Type> &right) {
            return left.target != right.target;
        }
    };

    template<typename Type>
    Shared_ptr<Iter<Type>> Iter<Type>::deep_copy() const {
        return make_shared<Iter<Type>>(*this);
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Type>
    class cIter {
    protected:
        Type *target = nullptr;

    public:
        virtual Shared_ptr<cIter<Type>> deep_copy() const;

        explicit cIter(Type *ptr) : target(ptr) {};

        virtual ~cIter() = default;

        virtual const Type &operator*() const {
            return *target;
        };

        virtual cIter<Type> &operator++() &{
            ++target;
            return *this;
        };

        friend bool operator==(const cIter<Type> &left, const cIter<Type> &right) {
            return left.target == right.target;
        }

        friend bool operator!=(const cIter<Type> &left, const cIter<Type> &right) {
            return left.target != right.target;
        }
    };

    template<typename Type>
    Shared_ptr<cIter<Type>> cIter<Type>::deep_copy() const {
        return Shared_ptr<cIter<Type>>(*this);
    }

}

#endif //TINYSTL_ITER_HPP
