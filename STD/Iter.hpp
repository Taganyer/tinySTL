//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_ITER_HPP
#define TINYSTL_ITER_HPP

namespace STD {
    template<typename Type>
    class Iter {
    protected:
        Type *target = nullptr;
    public:
        explicit Iter(Type *ptr) : target(ptr) {};

        virtual Type &operator*() const { return *target; };

        Iter<Type> &operator=(const Iter<Type> &other) {
            this->target = other.target;
            return *this;
        };

        virtual Iter<Type> &operator=(Type *ptr) {
            target = ptr;
            return *this;
        };

        virtual Iter<Type> &operator++() & {
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
    class cIter {
    protected:
        Type *target = nullptr;
    public:
        explicit cIter(Type *ptr) : target(ptr) {};

        inline const decltype(*target) operator*() {
            return *target;
        };

        inline cIter<Type> &operator=(const cIter<Type> &other) {
            this->target = other.target;
            return *this;
        };

        inline cIter<Type> &operator=(const Type *&ptr) {
            target = ptr;
            return *this;
        }

        virtual inline cIter<Type> &operator++() &{
            ++target;
            return *this;
        };

        virtual inline const cIter<Type> operator++(int) &{
            auto temp = *this;
            ++target;
            return temp;
        };

        inline friend bool operator==(const cIter<Type> &left, const cIter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const cIter<Type> &left, const cIter<Type> &right) {
            return left.target != right.target;
        }
    };


    template<typename Type>
    class rIter {
    protected:
        Type *target = nullptr;
    public:
        explicit rIter(Type *ptr) : target(ptr) {};

        inline decltype(*target) operator*() {
            return *target;
        };

        inline rIter<Type> &operator=(const rIter<Type> &other) {
            if (this->target != other.target) this->target = other.target;
            return *this;
        };

        inline rIter<Type> &operator=(const Type *&ptr) {
            target = ptr;
            return *this;
        }

        virtual inline rIter<Type> &operator++() &{
            --target;
            return *this;
        };

        virtual inline const rIter<Type> operator++(int) &{
            auto temp = *this;
            --target;
            return temp;
        };

        virtual inline rIter<Type> &operator--() &{
            ++target;
            return *this;
        };

        virtual inline const rIter<Type> operator--(int) &{
            auto temp = *this;
            ++target;
            return temp;
        };

        inline friend bool operator==(const rIter<Type> &left, const rIter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const rIter<Type> &left, const rIter<Type> &right) {
            return left.target != right.target;
        }
    };

    template<typename Type>
    class crIter {
    protected:
        Type *target = nullptr;
    public:
        explicit crIter(Type *ptr) : target(ptr) {};

        inline const decltype(*target) operator*() {
            return *target;
        };

        inline crIter<Type> &operator=(const crIter<Type> &other) {
            if (this->target != other.target) this->target = other.target;
            return *this;
        };

        inline crIter<Type> &operator=(const Type *&ptr) {
            target = ptr;
            return *this;
        }

        virtual inline crIter<Type> &operator++() &{
            --target;
            return *this;
        };

        virtual inline const crIter<Type> operator++(int) &{
            auto temp = *this;
            --target;
            return temp;
        };

        virtual inline crIter<Type> &operator--() &{
            ++target;
            return *this;
        };

        virtual inline const crIter<Type> operator--(int) &{
            auto temp = *this;
            ++target;
            return temp;
        }

        inline friend bool operator==(const crIter<Type> &left, const crIter<Type> &right) {
            return left.target == right.target;
        }

        inline friend bool operator!=(const crIter<Type> &left, const crIter<Type> &right) {
            return left.target != right.target;
        }
    };
}

#endif //TINYSTL_ITER_HPP
