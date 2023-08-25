//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_VECTOR_HPP
#define TINYSTL_VECTOR_HPP

#include "allocater.hpp"
#include "Iter.hpp"
#include "warning.hpp"
#include "Move.hpp"

namespace STD {

    template<typename Arg>
    class Vector {
    private:
        Size size_ = 0;

        Arg *val_begin = nullptr, *val_end = nullptr, *store_end = nullptr;

        void init(Size size, Arg arg);

        void reallocate(Size size);

    public:

        class Iterator;

        Vector();

        Vector(Size size, Arg target = Arg());

        Vector(const Iter<Arg> &begin, const Iter<Arg> &end);

        Vector(const Vector<Arg> &other);

        ~Vector<Arg>();

        Size size() const {
            return size_;
        }

        Size capacity() const {
            if (!store_end) return 0;
            return store_end - val_begin;
        }

        void clear(bool whether_to_release = false);

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(Arg &val);

        template<typename ...args>
        Iterator emplace(Size pos, args &&...vals);

        Iterator insert(Size pos, const Arg &val);

        Arg &operator[](Size pos);

        Arg &at(Size pos);

        Iterator begin();

        Iterator end();

    };

    template<typename Arg>
    class Vector<Arg>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;
    public:
        explicit Iterator(Arg *ptr) : Iter<Arg>(ptr) {};

        Iterator(const Iterator &other) : Iter<Arg>(other.target) {};

        Iterator &operator=(Arg *ptr) override;

        Iterator &operator=(const Iterator &other);

        Arg &operator*() const override;

        Iterator &operator++() & override;

        const Iterator operator++(int) &;

        Iterator operator+(Size size);

        Iterator operator-(Size size);

        Iterator &operator+=(Size size);

        Iterator &operator-=(Size size);

        Iterator &operator--() &;

        const Iterator operator--(int) &;

        friend bool operator>(const Vector<Arg>::Iterator &left, const Vector<Arg>::Iterator &right);

        friend bool operator<(const Vector<Arg>::Iterator &left, const Vector<Arg>::Iterator &right);

        friend bool operator>=(const Vector<Arg>::Iterator &left, const Vector<Arg>::Iterator &right);

        friend bool operator<=(const Vector<Arg>::Iterator &left, const Vector<Arg>::Iterator &right);

        friend long long operator-(const Vector<Arg>::Iterator &left, const Vector<Arg>::Iterator &right);

        ~Iterator() = default;
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    inline long long
    operator-(const typename Vector<Arg>::Iterator &left, const typename Vector<Arg>::Iterator &right) {
        return left.target - right.target;
    }

    template<typename Arg>
    inline bool operator<=(const typename Vector<Arg>::Iterator &left, const typename Vector<Arg>::Iterator &right) {
        return left.target <= right.target;
    }

    template<typename Arg>
    inline bool operator>=(const typename Vector<Arg>::Iterator &left, const typename Vector<Arg>::Iterator &right) {
        return left.target >= right.target;
    }

    template<typename Arg>
    inline bool operator<(const typename Vector<Arg>::Iterator &left, const typename Vector<Arg>::Iterator &right) {
        return left.target < right.target;
    }

    template<typename Arg>
    inline bool operator>(const typename Vector<Arg>::Iterator &left, const typename Vector<Arg>::Iterator &right) {
        return left.target > right.target;
    }

    template<typename Arg>
    inline const typename Vector<Arg>::Iterator Vector<Arg>::Iterator::operator--(int) &{
        return Vector<Arg>::Iterator(target--);
    }

    template<typename Arg>
    inline typename Vector<Arg>::Iterator &Vector<Arg>::Iterator::operator--() &{
        --target;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator &Vector<Arg>::Iterator::operator-=(Size size) {
        if (target - size < val_begin) throw outOfRange("You subtracted an out-of-range value to the function\n");
        target -= size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator &Vector<Arg>::Iterator::operator+=(Size size) {
        if (target + size >= val_end) throw outOfRange("You added an out-of-range value to the function\n");
        target += size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::Iterator::operator-(Size size) {
        if (target - size < val_begin) throw outOfRange("You subtracted an out-of-range value to the function\n");
        return Vector<Arg>::Iterator(target - size);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::Iterator::operator+(Size size) {
        if (target + size >= val_end) throw outOfRange("You added an out-of-range value to the function\n");
        return Vector<Arg>::Iterator(target + size);
    }

    template<typename Arg>
    inline const typename Vector<Arg>::Iterator Vector<Arg>::Iterator::operator++(int) &{
        Arg *temp = target++;
        return Vector<Arg>::Iterator(temp);
    }

    template<typename Arg>
    inline typename Vector<Arg>::Iterator &Vector<Arg>::Iterator::operator++() &{
        ++target;
        return *this;
    }

    template<typename Arg>
    inline Arg &Vector<Arg>::Iterator::operator*() const {
        return *target;
    }

    template<typename Arg>
    inline typename Vector<Arg>::Iterator &Vector<Arg>::Iterator::operator=(const Vector<Arg>::Iterator &other) {
        this->target = other.target;
        return *this;
    }

    template<typename Arg>
    inline typename Vector<Arg>::Iterator &Vector<Arg>::Iterator::operator=(Arg *ptr) {
        target = ptr;
        return *this;
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Vector<Arg>::Vector() = default;

    template<typename Arg>
    Vector<Arg>::Vector(Size size, Arg target) : val_begin(Allocate_n<Arg>(size + size / 5)) {
        val_end = val_begin + size;
        store_end = val_begin + size + size / 5;
        init(size, target);
    }

    template<typename Arg>
    Vector<Arg>::Vector(const Iter<Arg> &begin, const Iter<Arg> &end) {
        while (begin != end) {
            push_back(*begin);
            ++const_cast<Iter<Arg> &>(begin);
        }
    }

    template<typename Arg>
    Vector<Arg>::Vector(const Vector<Arg> &other) : size_(other.size_), val_begin(Allocate_n<Arg>(other.capacity())) {
        val_end = val_begin + size_;
        store_end = val_begin + other.capacity();
        for (int i = 0; i < size_; ++i) *(val_begin + i) = *(other.val_begin + i);
    }

    template<typename Arg>
    Vector<Arg>::~Vector<Arg>() {
        Deallocate_n(val_begin);
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    void Vector<Arg>::init(Size size, Arg arg) {
        Arg *temp = val_begin;
        for (Size i = 0; i < size; ++i) {
            *temp = arg;
            ++temp;
        }
    }

    template<typename Arg>
    void Vector<Arg>::reallocate(Size size) {
        auto the_new = Allocate_n<Arg>(size), the_old = val_begin;
        store_end = the_new + size;
        for (int i = 0; i < size_; ++i) *(the_new++) = *(the_old++);
        val_begin = store_end - size;
        val_end = val_begin + size_;
        Deallocate_n(the_old - size_);
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::Iterator Vector<Arg>::emplace(Size pos, args &&... vals) {
        if (pos > size_) throw outOfRange("Your function 'emplace' was passed an out-of-range position\n");
        Arg last = Arg(forward<args>(vals)...);
        if (capacity() <= size_) {
            Size size = size_ + 1 > size_ + size_ / 5 ? size_ + 1 : size_ + size_ / 5;
            auto the_new = Allocate_n<Arg>(size), the_old = val_begin;
            store_end = the_new + size;
            for (int i = 0; i <= size_; ++i) {
                if (i == pos) *(the_new++) = last;
                else *(the_new++) = *(the_old++);
            }
            ++size_;
            the_old = val_begin;
            val_begin = store_end - size;
            val_end = val_begin + size_;
            Deallocate_n(the_old);
        } else {
            auto temp = val_begin + pos;
            while (temp != val_end) swap(last, *(temp++));
            *temp = last;
            ++val_end;
            ++size_;
        }
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::insert(Size pos, const Arg &val) {
        if (pos > size_) throw outOfRange("Your function 'insert' was passed an out-of-range position\n");
        if (capacity() <= size_) {
            Size size = size_ + 1 > size_ + size_ / 5 ? size_ + 1 : size_ + size_ / 5;
            auto the_new = Allocate_n<Arg>(size), the_old = val_begin;
            store_end = the_new + size;
            for (int i = 0; i <= size_; ++i) {
                if (i == pos) *(the_new++) = val;
                else *(the_new++) = *(the_old++);
            }
            ++size_;
            the_old = val_begin;
            val_begin = store_end - size;
            val_end = val_begin + size_;
            Deallocate_n(the_old);
        } else {
            Arg last = val;
            auto temp = val_begin + pos;
            while (temp != val_end) swap(last, *(temp++));
            *temp = last;
            ++val_end;
            ++size_;
        }
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    template<typename... args>
    void Vector<Arg>::emplace_back(args &&... vals) {
        if (capacity() <= size_)
            reallocate(size_ + 1 > size_ + size_ / 5 ? size_ + 1 : size_ + size_ / 5);
        *(val_begin + size_) = Arg(forward<args>(vals)...);
        ++size_;
        ++val_end;
    }

    template<typename Arg>
    void Vector<Arg>::push_back(Arg &val) {
        if (capacity() <= size_)
            reallocate(size_ + 1 > size_ + size_ / 5 ? size_ + 1 : size_ + size_ / 5);
        *(val_begin + size_) = val;
        ++size_;
        ++val_end;
    }

    template<typename Arg>
    void Vector<Arg>::clear(bool whether_to_release) {
        if (whether_to_release) {
            Deallocate_n(val_begin);
            val_begin = val_end = store_end = nullptr;
        } else {
            Arg *temp = val_begin;
            while (temp != val_end)*(temp++).~Arg();
            val_end = val_begin;
        }
        size_ = 0;
    }

    template<typename Arg>
    Arg &Vector<Arg>::at(Size pos) {
        if (pos >= size_) throw outOfRange("You provided an out-of-range subscript");
        return *(val_begin + pos);
    }

    template<typename Arg>
    Arg &Vector<Arg>::operator[](Size pos) {
        return *(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::begin() {
        return Vector<Arg>::Iterator(val_begin);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::end() {
        return Vector<Arg>::Iterator(val_end);
    }

}

#endif //TINYSTL_VECTOR_HPP
