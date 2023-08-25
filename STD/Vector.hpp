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

        void pop_back();

        template<typename ...args>
        Iterator emplace(Size pos, args &&...vals);

        Iterator insert(Size pos, const Arg &val);

        Iterator erase(Size pos);

        Arg &operator[](Size pos);

        Vector<Arg> &operator=(const Vector<Arg> &other);

        template<typename Type>
        friend bool operator==(const Vector<Type> &left, const Vector<Type> &right);

        template<typename Type>
        friend bool operator!=(const Vector<Type> &left, const Vector<Type> &right);

        template<typename Type>
        friend bool operator<(const Vector<Type> &left, const Vector<Type> &right);

        template<typename Type>
        friend bool operator<=(const Vector<Type> &left, const Vector<Type> &right);

        template<typename Type>
        friend bool operator>(const Vector<Type> &left, const Vector<Type> &right);

        template<typename Type>
        friend bool operator>=(const Vector<Type> &left, const Vector<Type> &right);

        Arg &at(Size pos);

        Arg &front();

        Arg &back();

        Iterator begin();

        Iterator end();

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Vector<Arg>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        Shared_ptr<Iter<Arg>> deep_copy() override;

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

        template<typename Type>
        friend bool
        operator>(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right);

        template<typename Type>
        friend bool
        operator<(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right);;

        template<typename Type>
        friend bool
        operator>=(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right);;

        template<typename Type>
        friend bool
        operator<=(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right);;

        template<typename Type>
        friend long long
        operator-(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right);;

        ~Iterator() = default;
    };

//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    Shared_ptr<Iter<Arg>> Vector<Arg>::Iterator::deep_copy() {
        return Shared_ptr<Vector<Arg>::Iterator>(*this);
    }

    template<typename Type>
    inline long long
    operator-(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right) {
        return left.target - right.target;
    }

    template<typename Type>
    inline bool operator<=(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right) {
        return left.target <= right.target;
    }

    template<typename Type>
    inline bool operator>=(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right) {
        return left.target >= right.target;
    }

    template<typename Type>
    inline bool operator<(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right) {
        return left.target < right.target;
    }

    template<typename Type>
    inline bool operator>(const typename Vector<Type>::Iterator &left, const typename Vector<Type>::Iterator &right) {
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
        auto l = val_begin, r = other.val_begin;
        for (int i = 0; i < size_; ++i) {
            *l = *r;
            ++l, ++r;
        }
    }

    template<typename Arg>
    Vector<Arg>::~Vector<Arg>() {
        Deallocate_n(val_begin);
    }

//----------------------------------------------------------------------------------------------------------------------


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
    typename Vector<Arg>::Iterator Vector<Arg>::erase(Size pos) {
        if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'erase' function");
        auto now = val_begin + pos, next = now + 1;
        now->~Arg();
        while (next != val_end) {
            *now = move(*next);
            ++now, ++next;
        }
        --val_end;
        --size_;
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
    void Vector<Arg>::pop_back() {
        if (size_) {
            (--val_end)->~Arg();
            --size_;
        }
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
    Vector<Arg> &Vector<Arg>::operator=(const Vector<Arg> &other) {
        Deallocate_n(val_begin);
        size_ = other.size_;
        val_begin = Allocate_n<Arg>(other.capacity());
        val_end = val_begin + size_;
        store_end = val_begin + other.capacity();
        auto l = val_begin, r = other.val_begin;
        for (int i = 0; i < size_; ++i) {
            *l = *r;
            ++l, ++r;
        }
    }

    template<typename Type>
    bool operator==(const Vector<Type> &left, const Vector<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l != left.val_end && r != right.val_end) {
            if (*l != *r) return false;
            ++l, ++r;
        }
        return l == left.val_end && r == right.val_end;
    }

    template<typename Type>
    bool operator!=(const Vector<Type> &left, const Vector<Type> &right) {
        return !(left == right);
    }

    template<typename Type>
    bool operator<(const Vector<Type> &left, const Vector<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l != left.val_end && r != right.val_end) {
            if (*l != *r) return *l < *r;
            ++l, ++r;
        }
        return l == left.val_end && r != right.val_end;
    }

    template<typename Type>
    bool operator<=(const Vector<Type> &left, const Vector<Type> &right) {
        return left == right || left < right;
    }

    template<typename Type>
    bool operator>(const Vector<Type> &left, const Vector<Type> &right) {
        return left != right && !(left < right);
    }

    template<typename Type>
    bool operator>=(const Vector<Type> &left, const Vector<Type> &right) {
        return !(left < right);
    }

    template<typename Arg>
    Arg &Vector<Arg>::front() {
        if (!size_) throw outOfRange("You're accessing a non-existent element in the 'front()' function");
        return *val_begin;
    }

    template<typename Arg>
    Arg &Vector<Arg>::back() {
        if (!size_) throw outOfRange("You're accessing a non-existent element in the 'back()' function");
        return *(val_end - 1);
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
