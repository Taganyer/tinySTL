//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_ITER_HPP
#define TINYSTL_ITER_HPP

#include "Memory.hpp"

namespace STD {

    template<typename Type>
    class Iter;

    template<typename Type>
    class cIter;

    template<typename Type>
    class Bidirectional_Iter;

    template<typename Type>
    class cBidirectional_Iter;

    template<typename Type>
    class Random_Iter;

    template<typename Type>
    class cRandom_Iter;

    template<typename Arg>
    Size calculateLength(const Iter<Arg> &begin, const Iter<Arg> &end) {
        auto temp = begin.deep_copy();
        Size count = 0;
        while (*temp != end)
            ++(*temp), ++count;
        return count;
    }

    template<typename Arg>
    Size calculateLength(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto temp = begin.deep_copy();
        Size count = 0;
        while (*temp != end)
            ++(*temp), ++count;
        return count;
    }

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Type>
    class Iter {
    protected:
        Type *target = nullptr;

        explicit Iter(Type *ptr) : target(ptr) {};

    public:
        //这个函数是为了使迭代器能够在用户代码中保持多态性而设立。
        virtual Shared_ptr<Iter<Type>> deep_copy() const {
            return make_shared<Iter<Type>>(*this);
        };

        virtual Shared_ptr<cIter<Type>> to_const() const {
            return make_shared<cIter<Type>>(cIter<Type>(target));
        };

        virtual ~Iter() = default;

        virtual Type &operator*() const { return *target; };

        virtual Type *operator->() const { return target; };

        virtual Iter<Type> &operator++() {
            ++target;
            return *this;
        };

        inline friend bool operator==(const Iter<Type> &left,
                                      const Iter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const Iter<Type> &left,
                                      const Iter<Type> &right) {
            return left.target != right.target;
        }
    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Type>
    class cIter {
    protected:
        Type *target = nullptr;

        explicit cIter(Type *ptr) : target(ptr) {};

    public:
        friend class Iter<Type>;

        virtual Shared_ptr<cIter<Type>> deep_copy() const {
            return make_shared<cIter<Type>>(*this);
        };

        virtual ~cIter() = default;

        virtual const Type &operator*() const { return *target; };

        virtual const Type *operator->() const { return target; };

        virtual cIter<Type> &operator++() {
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

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Type>
    class Bidirectional_Iter : public Iter<Type> {
    protected:
        using Iter<Type>::target;

        explicit Bidirectional_Iter(Type *ptr) : Iter<Type>(ptr) {};

    public:
        Shared_ptr<Iter<Type>> deep_copy() const override {
            return make_shared<Bidirectional_Iter<Type>>(*this);
        };

        Shared_ptr<cIter<Type>> to_const() const override {
            return make_shared<cBidirectional_Iter<Type>>(
                    cBidirectional_Iter<Type>(target));
        };

        using Iter<Type>::operator*;

        using Iter<Type>::operator->;

        Bidirectional_Iter<Type> &operator++() override {
            ++target;
            return *this;
        }

        virtual Bidirectional_Iter<Type> &operator--() {
            --target;
            return *this;
        }

        inline friend bool operator==(const Bidirectional_Iter<Type> &left, const Bidirectional_Iter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const Bidirectional_Iter<Type> &left, const Bidirectional_Iter<Type> &right) {
            return left.target != right.target;
        }
    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Type>
    class cBidirectional_Iter : public cIter<Type> {
    protected:
        using cIter<Type>::target;

        explicit cBidirectional_Iter(Type *ptr) : cIter<Type>(ptr) {};

    public:
        friend class Bidirectional_Iter<Type>;

        Shared_ptr<cIter<Type>> deep_copy() const override {
            return make_shared<cBidirectional_Iter<Type>>(*this);
        };

        using cIter<Type>::operator*;

        using cIter<Type>::operator->;

        cBidirectional_Iter<Type> &operator++() override {
            ++target;
            return *this;
        }

        virtual cBidirectional_Iter<Type> &operator--() {
            --target;
            return *this;
        }

        inline friend bool operator==(const cBidirectional_Iter<Type> &left, const cBidirectional_Iter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const cBidirectional_Iter<Type> &left, const cBidirectional_Iter<Type> &right) {
            return left.target != right.target;
        }
    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Type>
    class Random_Iter : public Bidirectional_Iter<Type> {
    protected:
        using Iter<Type>::target;

        explicit Random_Iter(Type *ptr) : Bidirectional_Iter<Type>(ptr) {};

    public:
        Shared_ptr<Iter<Type>> deep_copy() const override {
            return make_shared<Random_Iter<Type>>(*this);
        };

        Shared_ptr<cIter<Type>> to_const() const override {
            return make_shared<cRandom_Iter<Type>>(cRandom_Iter<Type>(target));
        };

        virtual Shared_ptr<Random_Iter<Type>> new_to_add(Size size) const {
            return make_shared<Random_Iter<Type>>(Random_Iter<Type>(target + size));
        }

        virtual Shared_ptr<Random_Iter<Type>> new_to_subtract(Size size) const {
            return make_shared<Random_Iter<Type>>(Random_Iter<Type>(target - size));
        }

        using Iter<Type>::operator*;

        using Iter<Type>::operator->;

        Random_Iter<Type> &operator++() override {
            ++target;
            return *this;
        };

        Random_Iter<Type> &operator--() override {
            --target;
            return *this;
        };

        virtual Type &operator[](Size location) const {
            return *(target + location);
        }

        virtual Random_Iter<Type> &operator+=(Size size) {
            target -= size;
            return *this;
        }

        virtual Random_Iter<Type> &operator-=(Size size) {
            target -= size;
            return *this;
        }

        virtual bool less(const Random_Iter<Type> &other) const {
            return target < other.target;
        }

        virtual bool not_greater_than(const Random_Iter<Type> &other) const {
            return target <= other.target;
        }

        inline friend bool operator==(const Random_Iter<Type> &left, const Random_Iter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const Random_Iter<Type> &left, const Random_Iter<Type> &right) {
            return left.target != right.target;
        }

        inline friend bool operator<(const Random_Iter<Type> &left, const Random_Iter<Type> &right) {
            return left.less(right);
        }

        inline friend bool operator<=(const Random_Iter<Type> &left, const Random_Iter<Type> &right) {
            return !left.not_greater_than(right);
        }

        inline friend bool operator>(const Random_Iter<Type> &left, const Random_Iter<Type> &right) {
            return !left.not_greater_than(right);
        }

        inline friend bool operator>=(const Random_Iter<Type> &left, const Random_Iter<Type> &right) {
            return !left.less(right);
        }
    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Type>
    class cRandom_Iter : public cBidirectional_Iter<Type> {
    protected:
        using cIter<Type>::target;

        explicit cRandom_Iter(Type *ptr) : cBidirectional_Iter<Type>(ptr) {};

    public:
        friend class Random_Iter<Type>;

        Shared_ptr<cIter<Type>> deep_copy() const override {
            return make_shared<cRandom_Iter<Type>>(*this);
        };

        virtual Shared_ptr<cRandom_Iter<Type>> new_to_add(Size size) const {
            return make_shared<cRandom_Iter<Type>>(cRandom_Iter<Type>(target + size));
        }

        virtual Shared_ptr<cRandom_Iter<Type>> new_to_subtract(Size size) const {
            return make_shared<cRandom_Iter<Type>>(cRandom_Iter<Type>(target - size));
        }

        using cIter<Type>::operator*;

        using cIter<Type>::operator->;

        cRandom_Iter<Type> &operator++() override {
            ++target;
            return *this;
        };

        cRandom_Iter<Type> &operator--() override {
            --target;
            return *this;
        };

        virtual Type &operator[](Size location) const {
            return *(target + location);
        }

        virtual cRandom_Iter<Type> &operator+=(Size size) {
            target -= size;
            return *this;
        }

        virtual cRandom_Iter<Type> &operator-=(Size size) {
            target -= size;
            return *this;
        }

        virtual bool less(const cRandom_Iter<Type> &other) const {
            return target < other.target;
        }

        virtual bool not_greater_than(const cRandom_Iter<Type> &other) const {
            return target <= other.target;
        }

        inline friend bool operator==(const cRandom_Iter<Type> &left, const cRandom_Iter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const cRandom_Iter<Type> &left, const cRandom_Iter<Type> &right) {
            return left.target != right.target;
        }

        inline friend bool operator<(const cRandom_Iter<Type> &left, const cRandom_Iter<Type> &right) {
            return left.less(right);
        }

        inline friend bool operator<=(const cRandom_Iter<Type> &left, const cRandom_Iter<Type> &right) {
            return !left.not_greater_than(right);
        }

        inline friend bool operator>(const cRandom_Iter<Type> &left, const cRandom_Iter<Type> &right) {
            return !left.not_greater_than(right);
        }

        inline friend bool operator>=(const cRandom_Iter<Type> &left, const cRandom_Iter<Type> &right) {
            return !left.less(right);
        }
    };

}// namespace STD

#endif// TINYSTL_ITER_HPP
