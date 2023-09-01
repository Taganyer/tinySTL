//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_ITER_HPP
#define TINYSTL_ITER_HPP

#include "smart_ptr.hpp"

namespace STD {

    template<typename Type>
    class Iter {
    protected:
        Type *target = nullptr;

    public:
        virtual Shared_ptr<Iter<Type>> deep_copy() const;

        explicit Iter(Type *ptr) : target(ptr) {};

        virtual ~Iter() = default;

        virtual Type &operator*() const { return *target; };

        Iter<Type> &operator=(const Iter<Type> &other) {
            this->target = other.target;
            return *this;
        };

        virtual Iter<Type> &operator=(Type *ptr) {
            target = ptr;
            return *this;
        };

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
        return Shared_ptr<Iter<Type>>(*this);
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Type>
    class cIter {
    protected:
        Type *target = nullptr;

        virtual cIter<Type> &operator=(Type *ptr) {
            target = ptr;
            return *this;
        }

    public:
        virtual Shared_ptr<cIter<Type>> deep_copy() const;

        explicit cIter(Type *ptr) : target(ptr) {};

        virtual ~cIter() = default;

        virtual const Type &operator*() const {
            return *target;
        };

        cIter<Type> &operator=(const cIter<Type> &other) {
            this->target = other.target;
            return *this;
        };

        virtual cIter<Type> &operator++() &{
            ++target;
            return *this;
        };

        virtual const cIter<Type> operator++(int) {
            return cIter<Type>(target++);
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
