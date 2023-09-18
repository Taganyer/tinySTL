//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_VECTOR_HPP
#define TINYSTL_VECTOR_HPP

#include "Allocater.hpp"
#include "Iter.hpp"
#include "Warning.hpp"
#include "Move.hpp"

namespace STD {

    template<typename Arg>
    class Vector {
    private:

        Size size_ = 0;

        Arg *val_begin = nullptr, *val_end = nullptr, *store_end = nullptr;

        void reallocate(Size size);

        Arg *backward(Size pos_from, Size pos_to);

        static void fill_with(Arg *pos, Arg target, Size size);

        static void fill_with(Arg *pos, const std::initializer_list<Arg> &list);

        static void fill_with(Arg *pos, const Arg *target, Size size);

        static void fill_with(Arg *pos, const cIter<Arg> &begin, Size size);

        static void rfill_with(Arg *pos, Arg target, Size size);

        static void rfill_with(Arg *pos, const std::initializer_list<Arg> &list);

        static void rfill_with(Arg *pos, const Arg *target, Size size);

        static void rfill_with(Arg *pos, const cIter<Arg> &begin, Size size);

    public:

        class Iterator;

        class cIterator;

        class rIterator;

        class crIterator;

        Vector() = default;

        Vector(Size size, Arg target = Arg());

        //编译器给这个类开后门了，不用它没办法实现相同的效果。
        Vector(const std::initializer_list<Arg> &list);

        Vector(const Iter<Arg> &begin, const Iter<Arg> &end);

        Vector(const cIter<Arg> &begin, const cIter<Arg> &end);

        Vector(const Vector<Arg> &other);

        Vector(Vector<Arg> &&other) noexcept;

        ~Vector<Arg>() { Deallocate_n(val_begin); };

        Size size() const { return size_; };

        Size capacity() const {
            if (!store_end) return 0;
            return store_end - val_begin;
        };

        void clear(bool whether_to_release = false);

        void shirk_to_fit();

        Vector<Arg> &assign(Size size, const Arg &val);

        Vector<Arg> &assign(const Iter<Arg> &begin, const Iter<Arg> &end);

        Vector<Arg> &assign(const cIter<Arg> &begin, const cIter<Arg> &end);

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(const Arg &val);

        void push_back(Size size, const Arg &val);;

        void push_back(const Iter<Arg> &begin, const Iter<Arg> &end);

        void push_back(const cIter<Arg> &begin, const cIter<Arg> &end);

        void pop_back();

        template<typename ...args>
        Iterator emplace(Size pos, args &&...vals);

        template<typename ...args>
        Iterator emplace(const Iterator &pos, args &&...vals);

        template<typename ...args>
        cIterator emplace(const cIterator &pos, args &&...vals);

        template<typename ...args>
        rIterator emplace(const rIterator &pos, args &&...vals);

        template<typename ...args>
        crIterator emplace(const crIterator &pos, args &&...vals);

        Iterator insert(Size pos, const Arg &val, Size size = 1);

        Iterator insert(Size pos, const std::initializer_list<Arg> &list);

        Iterator insert(Size pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert(Size pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator insert(const Iterator &pos, Size size, const Arg &val);

        Iterator insert(const Iterator &pos, const std::initializer_list<Arg> &list);

        Iterator insert(const Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert(const Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        cIterator insert(const cIterator &pos, Size size, const Arg &val);

        cIterator insert(const cIterator &pos, const std::initializer_list<Arg> &list);

        cIterator insert(const cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        cIterator insert(const cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        rIterator insert(const rIterator &pos, Size size, const Arg &val);

        rIterator insert(const rIterator &pos, const std::initializer_list<Arg> &list);

        rIterator insert(const rIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        rIterator insert(const rIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        crIterator insert(const crIterator &pos, Size size, const Arg &val);

        crIterator insert(const crIterator &pos, const std::initializer_list<Arg> &list);

        crIterator insert(const crIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        crIterator insert(const crIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator erase(Size pos, Size size);

        Iterator erase(Size pos);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        rIterator erase(const rIterator &iter);

        crIterator erase(const crIterator &iter);

        rIterator erase(const rIterator &begin, const rIterator &end);

        crIterator erase(const crIterator &begin, const crIterator &end);

        Arg &operator[](Size pos) const { return *(val_begin + pos); };

        Arg &at(Size pos) const {
            if (pos >= size_) throw outOfRange("You provided an out-of-range subscript int the 'Vector::at' function");
            return *(val_begin + pos);
        };

        Arg &front() const {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'Vector::front' function");
            return *val_begin;
        };

        Arg &back() const {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'Vector::back' function");
            return *(val_end - 1);
        };

        bool empty() const { return !size_; };

        Vector<Arg> &operator=(const Vector<Arg> &other);

        Vector<Arg> &operator=(Vector<Arg> &&other) noexcept;

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

        Iterator begin() const { return Vector<Arg>::Iterator(val_begin); };

        Iterator end() const { return Vector<Arg>::Iterator(val_end); };

        cIterator cbegin() const { return Vector<Arg>::cIterator(val_begin); };

        cIterator cend() const { return Vector<Arg>::cIterator(val_end); };

        rIterator rbegin() const { return Vector<Arg>::rIterator(val_end - 1); };

        rIterator rend() const { return Vector<Arg>::rIterator(val_begin - 1); };

        crIterator crbegin() const { return Vector<Arg>::crIterator(val_end - 1); };

        crIterator crend() const { return Vector<Arg>::crIterator(val_begin - 1); };

    };


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Vector<Arg>::Vector(Size size, Arg target) : val_begin(Allocate_n<Arg>(size + size / 5)), size_(size) {
        val_end = val_begin + size;
        store_end = val_begin + size + size / 5;
        fill_with(val_begin, target, size);
    }

    template<typename Arg>
    Vector<Arg>::Vector(const std::initializer_list<Arg> &list) : size_(list.size()), val_begin(Allocate_n<Arg>(size_)),
                                                                  val_end(val_begin + list.size()) {
        fill_with(val_begin, list);
        store_end = val_end;
    }

    template<typename Arg>
    Vector<Arg>::Vector(const Iter<Arg> &begin, const Iter<Arg> &end) : Vector(*begin.to_const(), *end.to_const()) {}

    template<typename Arg>
    Vector<Arg>::Vector(const cIter<Arg> &begin, const cIter<Arg> &end) : size_(calculateLength(begin, end)) {
        val_begin = Allocate_n<Arg>(size_);
        store_end = val_end = val_begin + size_;
        fill_with(val_begin, begin, size_);
    }

    template<typename Arg>
    Vector<Arg>::Vector(const Vector<Arg> &other) : size_(other.size_), val_begin(Allocate_n<Arg>(other.capacity())) {
        val_end = val_begin + size_;
        store_end = val_begin + other.capacity();
        fill_with(val_begin, other.val_begin, other.size_);
    }

    template<typename Arg>
    Vector<Arg>::Vector(Vector<Arg> &&other) noexcept : size_(other.size_), val_begin(other.val_begin),
                                                        val_end(other.val_end), store_end(other.store_end) {
        other.size_ = 0;
        other.val_begin = other.val_end = other.store_end = nullptr;
    }

    template<typename Arg>
    void Vector<Arg>::reallocate(Size size) {
        auto the_new = Allocate_n<Arg>(size), the_old = val_begin;
        store_end = the_new + size;
        for (int i = 0; i < size_; ++i) *the_new++ = move(*the_old++);
        val_begin = store_end - size;
        val_end = val_begin + size_;
        Deallocate_n(the_old - size_);
    }

    template<typename Arg>
    Arg *Vector<Arg>::backward(Size pos_from, Size pos_to) {
        Size new_size = size_ + pos_to - pos_from;
        if (new_size > capacity()) {
            auto the_new = Allocate_n<Arg>(new_size), temp = the_new;
            for (int i = 0; i < pos_from; ++i) {
                *temp = move(*val_begin);
                ++temp, ++val_begin;
            }
            temp = the_new + pos_to;
            while (val_begin < val_end) {
                *temp = *val_begin;
                ++temp, ++val_begin;
            }
            val_begin = the_new;
            store_end = val_end = temp;
        } else {
            auto temp1 = val_end - 1, temp2 = val_end - 1 + pos_to - pos_from, target_end = val_begin + pos_from;
            val_end = temp2 + 1;
            while (temp1 >= target_end) {
                *temp2 = move(*temp1);
                --temp1, --temp2;
            }
        }
        size_ += pos_to - pos_from;
        return val_begin + pos_from;
    }

    template<typename Arg>
    void Vector<Arg>::fill_with(Arg *pos, Arg target, Size size) {
        for (int i = 0; i < size; ++i) {
            *pos = target;
            ++pos;
        }
    }

    template<typename Arg>
    void Vector<Arg>::fill_with(Arg *pos, const Arg *target, Size size) {
        for (int i = 0; i < size; ++i) {
            *pos = *target;
            ++pos, ++target;
        }
    }

    template<typename Arg>
    void Vector<Arg>::fill_with(Arg *pos, const std::initializer_list<Arg> &list) {
        for (auto &t: list) {
            *pos = t;
            ++pos;
        }
    }

    template<typename Arg>
    void Vector<Arg>::fill_with(Arg *pos, const cIter<Arg> &begin, Size size) {
        auto temp = begin.deep_copy();
        for (int i = 0; i < size; ++i) {
            *pos = **temp;
            ++pos, ++(*temp);
        }
    }

    template<typename Arg>
    void Vector<Arg>::rfill_with(Arg *pos, const Arg *target, Size size) {
        pos += size - 1;
        for (int i = 0; i < size; ++i) {
            *pos = *target;
            --pos, ++target;
        }
    }

    template<typename Arg>
    void Vector<Arg>::rfill_with(Arg *pos, const std::initializer_list<Arg> &list) {
        pos += list.size() - 1;
        for (auto &t: list) {
            *pos = t;
            --pos;
        }
    }

    template<typename Arg>
    void Vector<Arg>::rfill_with(Arg *pos, const cIter<Arg> &begin, Size size) {
        pos += size - 1;
        auto temp = begin.deep_copy();
        for (int i = 0; i < size; ++i) {
            *pos = **temp;
            --pos, ++(*temp);
        }
    }

    template<typename Arg>
    void Vector<Arg>::clear(bool whether_to_release) {
        if (whether_to_release) {
            Deallocate_n(val_begin);
            val_begin = val_end = store_end = nullptr;
        } else {
            Arg *temp = val_begin;
            while (temp != val_end) *(temp++).~Arg();
            val_end = val_begin;
        }
        size_ = 0;
    }

    template<typename Arg>
    void Vector<Arg>::shirk_to_fit() {
        if (capacity() == size_) return;
        else if (!size_) {
            Deallocate_n(val_begin);
            val_begin = store_end = val_end = nullptr;
            return;
        }
        auto temp = Allocate_n<Arg>(size_);
        fill_with(temp, val_begin, size_);
        Deallocate_n(val_begin);
        val_begin = temp;
        store_end = val_end = temp + size_;
    }

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::assign(Size size, const Arg &val) {
        Deallocate_n(val_begin);
        val_begin = Allocate_n<Arg>(size);
        fill_with(val_begin, val, size);
        store_end = val_end = val_begin + size;
        size_ = size;
        return *this;
    }

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::assign(const Iter<Arg> &begin, const Iter<Arg> &end) {
        return assign(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::assign(const cIter<Arg> &begin, const cIter<Arg> &end) {
        Deallocate_n(val_begin);
        size_ = calculateLength(begin, end);
        val_begin = Allocate_n<Arg>(size_);
        fill_with(val_begin, begin, size_);
        store_end = val_end = val_begin + size_;
        return *this;
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
    void Vector<Arg>::push_back(const Arg &val) {
        if (capacity() <= size_)
            reallocate(size_ + 1 > size_ + size_ / 5 ? size_ + 1 : size_ + size_ / 5);
        *(val_begin + size_) = val;
        ++size_;
        ++val_end;
    }

    template<typename Arg>
    void Vector<Arg>::push_back(Size size, const Arg &val) {
        if (capacity() - size_ < size) reallocate(capacity() + size);
        size_ += size;
        fill_with(val_end, val, size);
        size_ += size;
        val_end += size;
    }

    template<typename Arg>
    void Vector<Arg>::push_back(const Iter<Arg> &begin, const Iter<Arg> &end) {
        push_back(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    void Vector<Arg>::push_back(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto temp(begin.deep_copy());
        Size count = calculateLength(begin, end);
        if (capacity() - size_ < count) reallocate(capacity() + count);
        fill_with(val_end, begin, count);
        size_ += count;
        val_end += count;
    }

    template<typename Arg>
    void Vector<Arg>::pop_back() {
        if (size_) {
            (--val_end)->~Arg();
            --size_;
        }
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::Iterator Vector<Arg>::emplace(Size pos, args &&... vals) {
        if (pos > size_) throw outOfRange("Your function 'Vector::emplace' was passed an out-of-range position\n");
        *backward(pos, pos + 1) = Arg(forward<args>(vals)...);
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::Iterator Vector<Arg>::emplace(const Vector<Arg>::Iterator &pos, args &&... vals) {
        if (pos.target < val_begin || pos.target > val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::emplace' function");
        return emplace(pos.target - val_begin, forward<args>(vals)...);
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::cIterator Vector<Arg>::emplace(const Vector<Arg>::cIterator &pos, args &&... vals) {
        if (pos.target < val_begin || pos.target > val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::emplace' function");
        return Vector<Arg>::cIterator(emplace(pos.target - val_begin, forward<args>(vals)...).target);
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::rIterator Vector<Arg>::emplace(const Vector<Arg>::rIterator &pos, args &&... vals) {
        if (pos.target < val_begin - 1 || pos.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::emplace' function");
        return Vector<Arg>::rIterator(emplace(pos.target - val_begin + 1, forward<args>(vals)...).target - 1);
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::crIterator Vector<Arg>::emplace(const Vector<Arg>::crIterator &pos, args &&... vals) {
        if (pos.target < val_begin - 1 || pos.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::emplace' function");
        return Vector<Arg>::crIterator(emplace(pos.target - val_begin + 1, forward<args>(vals)...).target - 1);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::insert(Size pos, const Arg &val, Size size) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Vector::insert' function");
        if (!size) return Iterator(val_begin + pos);
        fill_with(backward(pos, pos + size), val, size);
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::insert(Size pos, const std::initializer_list<Arg> &list) {
        if (!list.size()) return Vector<Arg>::Iterator(val_begin + pos);
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
        fill_with(backward(pos, pos + list.size()), list);
        return Vector<Arg>::Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::insert(Size pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return Vector<Arg>::Iterator(insert(pos, *begin.to_const(), *end.to_const()).target);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(Size pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Vector::insert' function");
        Size size = calculateLength(begin, end);
        fill_with(backward(pos, pos + size), begin, size);
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::insert(const Iterator &pos, Size size, const Arg &val) {
        if (pos.target < val_begin || pos.target > val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return insert(pos.target - val_begin, size, val);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::insert(const Iterator &pos, const std::initializer_list<Arg> &list) {
        return insert(pos.target - val_begin, list);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(const Vector<Arg>::Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        if (pos.target < val_begin || pos.target > val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return insert(pos.target - val_begin, begin, end);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(const Vector<Arg>::Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (pos.target < val_begin || pos.target > val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return insert(pos.target - val_begin, begin, end);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator Vector<Arg>::insert(const Vector<Arg>::cIterator &pos, Size size, const Arg &val) {
        if (pos.target < val_begin || pos.target > val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return Vector<Arg>::cIterator(insert(pos.target - val_begin, size, val).target);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator Vector<Arg>::insert(const cIterator &pos, const std::initializer_list<Arg> &list) {
        return cIterator(insert(pos.target - val_begin, list).target);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator
    Vector<Arg>::insert(const Vector<Arg>::cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        if (pos.target < val_begin || pos.target > val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return Vector<Arg>::cIterator(insert(pos.target - val_begin, begin, end).target);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator
    Vector<Arg>::insert(const Vector<Arg>::cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (pos.target < val_begin || pos.target > val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return Vector<Arg>::cIterator(insert(pos.target - val_begin, begin, end).target);
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator Vector<Arg>::insert(const Vector<Arg>::rIterator &pos, Size size, const Arg &val) {
        if (pos.target < val_begin - 1 || pos.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return Vector<Arg>::rIterator(insert(pos.target - val_begin + 1, size, val).target - 1);
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator Vector<Arg>::insert(const rIterator &pos, const std::initializer_list<Arg> &list) {
        if (!list.size()) return pos;
        if (pos.target < val_begin - 1 || pos.target >= val_end)
            throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
        Size temp = pos.target - val_begin + 1;
        rfill_with(backward(temp, temp + list.size()), list);
        return Vector<Arg>::rIterator(val_begin + temp + list.size() - 1);
    }


    template<typename Arg>
    typename Vector<Arg>::rIterator
    Vector<Arg>::insert(const Vector<Arg>::rIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return insert(pos, *begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator
    Vector<Arg>::insert(const Vector<Arg>::rIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (pos.target < val_begin - 1 || pos.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        Size target_len = STD::calculateLength(begin, end);
        auto temp = backward(pos.target - val_begin + 1, pos.target - val_begin + 1 + target_len);
        rfill_with(temp, begin, target_len);
        return Vector<Arg>::rIterator(temp + target_len - 1);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator
    Vector<Arg>::insert(const Vector<Arg>::crIterator &pos, Size size, const Arg &val) {
        if (pos.target < val_begin - 1 || pos.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return Vector<Arg>::crIterator(insert(pos.target - val_begin + 1, size, val).target - 1);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator
    Vector<Arg>::insert(const Vector<Arg>::crIterator &pos, const std::initializer_list<Arg> &list) {
        return Vector<Arg>::crIterator(insert(rIterator(pos.target), list).target);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator
    Vector<Arg>::insert(const Vector<Arg>::crIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return Vector<Arg>::crIterator(
                insert(Vector<Arg>::rIterator(pos.target), *begin.to_const(), *end.to_const()).target);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator
    Vector<Arg>::insert(const Vector<Arg>::crIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (pos.target < val_begin - 1 || pos.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return Vector<Arg>::crIterator(insert(Vector<Arg>::rIterator(pos.target), begin, end).target);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::erase(Size pos, Size size) {
        if (!size) return Vector::Iterator(val_begin + pos);
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Vector::insert' function");
        auto temp1 = val_begin + pos, temp2 = val_begin + pos;
        if (pos + size > size_) size = size_ - pos;
        size_ -= size;
        for (int i = 0; i < size; ++i) {
            temp1->~Arg();
            ++temp1;
        }
        while (temp1 != val_end) {
            *temp2 = *temp1;
            ++temp1, ++temp2;
        }
        val_end = temp2;
        return Vector::Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::erase(Size pos) {
        return erase(pos, 1);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::erase(const Vector::Iterator &iter) {
        if (iter.target < val_begin || iter.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::erase' function");
        return erase(iter.target - val_begin, 1);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator Vector<Arg>::erase(const Vector<Arg>::cIterator &iter) {
        if (iter.target < val_begin)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::erase' function");
        return erase(iter.target - val_begin, 1);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::erase(const Vector::Iterator &begin, const Vector<Arg>::Iterator &end) {
        return erase(begin.target - val_begin, end - begin);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator
    Vector<Arg>::erase(const Vector::cIterator &begin, const Vector<Arg>::cIterator &end) {
        if (end < begin || begin.target < val_begin || end.target > val_end)
            throw outOfRange("You passed in a pair of out-of-range iterators in the 'Vector::erase' function");
        return Vector<Arg>::cIterator(erase(begin.target - val_begin, end - begin).target);
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator Vector<Arg>::erase(const Vector::rIterator &iter) {
        if (iter.target < val_begin || iter.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::erase' function");
        return Vector<Arg>::rIterator(erase(iter.target - val_begin, 1).target - 1);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator Vector<Arg>::erase(const Vector<Arg>::crIterator &iter) {
        if (iter.target < val_begin || iter.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::erase' function");
        return Vector<Arg>::crIterator(erase(iter.target - val_begin, 1).target - 1);
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator
    Vector<Arg>::erase(const Vector::rIterator &begin, const Vector<Arg>::rIterator &end) {
        if (end > begin || begin.target >= val_end || end.target < val_begin - 1)
            throw outOfRange("You passed in a pair of out-of-range iterators in the 'Vector::erase' function");
        return Vector<Arg>::rIterator(erase(end.target - val_begin + 1, end - begin).target - 1);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator
    Vector<Arg>::erase(const Vector::crIterator &begin, const Vector<Arg>::crIterator &end) {
        if (end > begin || begin.target >= val_end || end.target < val_begin - 1)
            throw outOfRange("You passed in a pair of out-of-range iterators in the 'Vector::erase' function");
        return Vector<Arg>::crIterator(erase(end.target - val_begin + 1, end - begin).target - 1);
    }

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::operator=(const Vector<Arg> &other) {
        if (this == &other) return *this;
        Deallocate_n(val_begin);
        size_ = other.size_;
        val_begin = Allocate_n<Arg>(other.capacity());
        val_end = val_begin + size_;
        store_end = val_begin + other.capacity();
        fill_with(val_begin, other.val_begin, size_);
        return *this;
    }

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::operator=(Vector<Arg> &&other) noexcept {
        if (this == &other) return *this;
        size_ = other.size_;
        val_begin = other.val_begin;
        val_end = other.val_end;
        store_end = other.store_end;
        other.size_ = 0;
        other.val_begin = other.val_end = other.store_end = nullptr;
        return *this;
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
        return !(left > right);
    }

    template<typename Type>
    bool operator>(const Vector<Type> &left, const Vector<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l != left.val_end && r != right.val_end) {
            if (*l != *r) return *l > *r;
            ++l, ++r;
        }
        return r == right.val_end && l != left.val_end;
    }

    template<typename Type>
    bool operator>=(const Vector<Type> &left, const Vector<Type> &right) {
        return !(left < right);
    }


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Vector<Arg>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        Iterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit Iterator(Arg *ptr) : Iter<Arg>(ptr) {};

    public:
        friend class Vector<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Vector<Arg>::Iterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Vector<Arg>::cIterator>(Vector<Arg>::cIterator(target));
        };

        Iterator(const Iterator &other) : Iter<Arg>(other.target) {};

        ~Iterator() = default;

        Iterator &operator=(const Iterator &other) {
            target = other.target;
            return *this;
        };


        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        Iterator &operator++() override {
            ++target;
            return *this;
        };

        Iterator operator++(int) { return Vector<Arg>::Iterator(target++); };

        Iterator &operator--() {
            --target;
            return *this;
        };

        Iterator operator--(int) { return Vector<Arg>::Iterator(target--); };

        Iterator operator+(Size size) const { return Vector<Arg>::Iterator(target + size); };

        Iterator operator-(Size size) const { return Vector<Arg>::Iterator(target - size); };

        Iterator &operator+=(Size size) {
            target += size;
            return *this;
        };

        Iterator &operator-=(Size size) {
            target -= size;
            return *this;
        };

        friend bool
        operator==(const Iterator &left, const Iterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const Iterator &left, const Iterator &right) { return left.target != right.target; };

        friend bool
        operator>(const Iterator &left, const Iterator &right) { return left.target > right.target; };

        friend bool
        operator<(const Iterator &left, const Iterator &right) { return left.target < right.target; };

        friend bool
        operator>=(const Iterator &left, const Iterator &right) { return left.target >= right.target; };

        friend bool
        operator<=(const Iterator &left, const Iterator &right) { return left.target <= right.target; };

        friend long long
        operator-(const Iterator &left, const Iterator &right) { return left.target - right.target; };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Vector<Arg>::cIterator : public cIter<Arg> {
    protected:
        using cIter<Arg>::target;

        cIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit cIterator(Arg *ptr) : cIter<Arg>(ptr) {};

    public:
        friend class Vector<Arg>;

        friend class Vector<Arg>::Iterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Vector<Arg>::cIterator>(*this); };

        cIterator(const cIterator &other) : cIter<Arg>(other.target) {};

        ~cIterator() override = default;

        cIterator &operator=(const cIterator &other) {
            target = other.target;
            return *this;
        };

        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        cIterator &operator++() override {
            ++target;
            return *this;
        };

        cIterator operator++(int) { return Vector<Arg>::cIterator(target++); };

        cIterator operator+(Size size) const { return Vector<Arg>::cIterator(target + size); };

        cIterator operator-(Size size) const { return Vector<Arg>::cIterator(target - size); };

        cIterator &operator+=(Size size) {
            target += size;
            return *this;
        };

        cIterator &operator-=(Size size) {
            target -= size;
            return *this;
        };

        cIterator &operator--() {
            --target;
            return *this;
        };

        cIterator operator--(int) { return Vector<Arg>::cIterator(target--); };

        friend bool
        operator==(const cIterator &left, const cIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const cIterator &left, const cIterator &right) { return left.target != right.target; };

        friend bool
        operator>(const cIterator &left, const cIterator &right) { return left.target > right.target; };

        friend bool
        operator<(const cIterator &left, const cIterator &right) { return left.target < right.target; };

        friend bool
        operator>=(const cIterator &left, const cIterator &right) { return left.target >= right.target; };

        friend bool
        operator<=(const cIterator &left, const cIterator &right) { return left.target <= right.target; };

        friend long long
        operator-(const cIterator &left, const cIterator &right) { return left.target - right.target; };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Vector<Arg>::rIterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        rIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        }

        explicit rIterator(Arg *ptr) : Iter<Arg>(ptr) {};

    public:
        friend class Vector<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Vector<Arg>::rIterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Vector<Arg>::crIterator>(Vector<Arg>::crIterator(target));
        };

        rIterator(const rIterator &other) : Iter<Arg>(other.target) {};

        ~rIterator() = default;

        rIterator &operator=(const rIterator &other) {
            target = other.target;
            return *this;
        };

        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        rIterator &operator++() override {
            --target;
            return *this;
        };

        rIterator operator++(int) { return Vector<Arg>::rIterator(target--); };

        rIterator &operator--() {
            ++target;
            return *this;
        };

        rIterator operator--(int) { return Vector<Arg>::rIterator(target++); };

        rIterator operator+(Size size) const { return Vector<Arg>::rIterator(target - size); };

        rIterator operator-(Size size) const { return Vector<Arg>::rIterator(target + size); };

        rIterator &operator+=(Size size) {
            target -= size;
            return *this;
        };

        rIterator &operator-=(Size size) {
            target += size;
            return *this;
        };

        friend bool
        operator==(const rIterator &left, const rIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const rIterator &left, const rIterator &right) { return left.target != right.target; };

        friend bool
        operator>(const rIterator &left, const rIterator &right) { return left.target < right.target; };

        friend bool
        operator<(const rIterator &left, const rIterator &right) { return left.target > right.target; };

        friend bool
        operator>=(const rIterator &left, const rIterator &right) { return left.target <= right.target; };

        friend bool
        operator<=(const rIterator &left, const rIterator &right) { return left.target >= right.target; };

        friend long long
        operator-(const rIterator &left, const rIterator &right) { return right.target - left.target; };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Vector<Arg>::crIterator : public cIter<Arg> {
    protected:
        using cIter<Arg>::target;

        crIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit crIterator(Arg *ptr) : cIter<Arg>(ptr) {};

    public:
        friend class Vector<Arg>;

        friend class Vector<Arg>::rIterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Vector<Arg>::crIterator>(*this); };

        crIterator(const crIterator &other) : cIter<Arg>(other.target) {};

        ~crIterator() = default;

        crIterator &operator=(const crIterator &other) {
            target = other.target;
            return *this;
        };

        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        crIterator &operator++() override {
            --target;
            return *this;
        };

        crIterator operator++(int) { return Vector<Arg>::crIterator(target--); };

        crIterator &operator--() {
            ++target;
            return *this;
        };

        crIterator operator--(int) { return Vector<Arg>::crIterator(target++); };

        crIterator operator+(Size size) const { return Vector<Arg>::crIterator(target - size); };

        crIterator operator-(Size size) const { return Vector<Arg>::crIterator(target + size); };

        crIterator &operator+=(Size size) {
            target -= size;
            return *this;
        };

        crIterator &operator-=(Size size) {
            target += size;
            return *this;
        };

        friend bool
        operator==(const crIterator &left, const crIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const crIterator &left, const crIterator &right) { return left.target != right.target; };

        friend bool
        operator>(const crIterator &left, const crIterator &right) { return left.target < right.target; };

        friend bool
        operator<(const crIterator &left, const crIterator &right) { return left.target > right.target; };

        friend bool
        operator>=(const crIterator &left, const crIterator &right) { return left.target <= right.target; };

        friend bool
        operator<=(const crIterator &left, const crIterator &right) { return left.target >= right.target; };

        friend long long
        operator-(const crIterator &left, const crIterator &right) { return right.target - left.target; };
    };

//----------------------------------------------------------------------------------------------------------------------

}

#endif //TINYSTL_VECTOR_HPP
