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

        void fill_with(Size size, Arg arg);

        void reallocate(Size size);

    public:

        class Iterator;

        class cIterator;

        class rIterator;

        class crIterator;

        Vector() = default;

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

        void shirk_to_fit();

        void assign(const Iter<Arg> &begin, const Iter<Arg> &end);

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(Arg &val);

        void push_back(const Iter<Arg> &begin, const Iter<Arg> &end);

        void pop_back();

        template<typename ...args>
        Iterator emplace(Size pos, args &&...vals);

        Iterator insert(Size pos, const Arg &val);

        Iterator insert(Size pos, Iter<Arg> &begin, Iter<Arg> &end);

        Iterator erase(Size pos, bool until_end = false);

        Iterator erase(const Iterator &begin);

        Iterator erase(const Iterator &begin, const Iterator &end);

        Arg &operator[](Size pos) const;

        Arg &at(Size pos) const;

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

        Arg &front();

        Arg &back();

        Iterator begin();

        Iterator end();

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Vector<Arg>::Vector(Size size, Arg target) : val_begin(Allocate_n<Arg>(size + size / 5)), size_(size) {
        val_end = val_begin + size;
        store_end = val_begin + size + size / 5;
        fill_with(size, target);
    }

    template<typename Arg>
    Vector<Arg>::Vector(const Iter<Arg> &begin, const Iter<Arg> &end) {
        auto iter = begin.deep_copy();
        Size count = 0;
        while (*iter != end) ++(*iter), ++count;
        iter = begin.deep_copy();
        size_ = count;
        val_begin = Allocate_n<Arg>(count);
        store_end = val_end = val_begin + count;
        while (*iter != end) {
            *val_begin = **iter;
            ++val_begin, ++(*iter);
        }
        val_begin = val_end - count;
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
    void Vector<Arg>::fill_with(Size size, Arg arg) {
        auto temp = val_begin;
        for (int i = 0; i < size; ++i) {
            *temp = arg;
            ++temp;
        }
    }

//----------------------------------------------------------------------------------------------------------------------


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
    typename Vector<Arg>::Iterator Vector<Arg>::insert(Size pos, Iter<Arg> &begin, Iter<Arg> &end) {
        auto temp(begin.deep_copy());
        Size count = 0;
        while (*temp != end) ++count, ++(*temp);
        if (!count) return Iterator(val_begin + pos);
        temp = begin.deep_copy();
        if (capacity() - size_ < count) {
            auto t_begin = Allocate_n<Arg>(capacity() + count), t = t_begin;
            store_end = t_begin + capacity() + count;
            for (int i = 0; i < pos; ++i) {
                *t_begin = move(*val_begin);
                ++t_begin, ++val_begin;
            }
            while (end != *temp) {
                *t_begin = **temp;
                ++t_begin, ++(*temp);
            }
            while (val_begin != val_end) {
                *t_begin = move(*val_begin);
                ++t_begin, ++val_begin;
            }
            Deallocate_n(val_begin - size_);
            val_begin = t;
            val_end = t_begin;
            size_ = val_end - val_begin;
        } else {
            auto temp1 = val_begin + pos, temp2 = val_end;
            while (temp1 != val_end) {
                *temp2 = move(*temp1);
                ++temp1, ++temp2;
            }
            val_end = temp2;
            temp1 = val_begin + pos;
            while (*temp != end) {
                *temp1 = **temp;
                ++temp1, ++(*temp);
            }
            size_ += count;
        }
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::erase(Size pos, bool until_end) {
        if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'erase' function");
        if (until_end) {
            auto now = val_begin + pos;
            while (now != val_end) {
                now->~Arg();
                ++now;
            }
            val_end = val_begin + pos;
            size_ = pos;
        } else {
            auto now = val_begin + pos, next = now + 1;
            now->~Arg();
            while (next != val_end) {
                *now = move(*next);
                ++now, ++next;
            }
            --val_end;
            --size_;
        }
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::erase(const Vector::Iterator &begin) {
        if (begin.target < val_begin)
            throw outOfRange("You passed in an out-of-range iterator in the 'erase' function");
        auto temp(begin.target);
        while (temp < val_end) {
            temp->~Arg();
            ++temp;
        }
        val_end = begin.target;
        size_ = val_end - val_begin;
        return begin;
    }


    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::erase(const Vector::Iterator &begin, const Vector::Iterator &end) {
        if (end < begin || begin.target < val_begin || end.target > val_end)
            throw outOfRange("You passed in a pair of out-of-range iterators in the 'erase' function");
        auto temp1(begin.target), temp2(end.target);
        while (temp1 != temp2) {
            temp1->~Arg();
            ++temp1;
        }
        temp1 = begin.target;
        while (temp2 != val_end) {
            *temp1 = *temp2;
            ++temp1, ++temp2;
        }
        val_end = begin.target;
        size_ = val_end - val_begin;
        return begin;
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
    void Vector<Arg>::push_back(const Iter<Arg> &begin, const Iter<Arg> &end) {
        auto temp(begin.deep_copy());
        Size count = 0;
        while (*temp != end) ++count, ++(*temp);
        temp = begin.deep_copy();
        if (capacity() - size_ < count) {
            auto t = Allocate_n<Arg>(capacity() + size_);
            store_end = capacity() + size_;
            while (val_begin != val_end) {
                *t = move(*val_begin);
                ++t, ++val_begin;
            }
            Deallocate_n(val_begin - size_);
            val_begin = t - size_;
            while (*temp != end) {
                *t = move(**temp);
                ++t, ++(*temp);
            }
            val_end = t;
            size_ = val_end - val_begin;
        } else {
            size_ += count;
            while (*temp != end) {
                *val_end = move(**temp);
                ++val_end, ++(*temp);
            }
        }
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
    void Vector<Arg>::shirk_to_fit() {
        if (capacity() == size_) return;
        else if (!size_) {
            Deallocate_n(val_begin);
            val_begin = store_end = val_end = nullptr;
            return;
        }
        auto temp = Allocate_n<Arg>(size_);
        for (int i = 0; i < size_; ++i) {
            *temp = *val_begin;
            ++temp, ++val_begin;
        }
        Deallocate_n(val_begin - size_);
        store_end = val_end = temp;
        val_begin = temp - size_;
    }

    template<typename Arg>
    void Vector<Arg>::assign(const Iter<Arg> &begin, const Iter<Arg> &end) {
        Deallocate_n(val_begin);
        auto temp = begin.deep_copy();
        Size count = 0;
        while (*temp != end) ++(*temp), ++count;
        temp = begin.deep_copy();
        auto t = Allocate_n<Arg>(count);
        while (*temp != end) {
            *t = **temp;
            ++t, ++(*temp);
        }
        store_end = val_end = t;
        val_begin = t - count;
        size_ = count;
    }

    template<typename Arg>
    Arg &Vector<Arg>::at(Size pos) const {
        if (pos >= size_) throw outOfRange("You provided an out-of-range subscript");
        return *(val_begin + pos);
    }

    template<typename Arg>
    Arg &Vector<Arg>::operator[](Size pos) const {
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


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Vector<Arg>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        Iterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

    public:
        friend class Vector<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override;

        explicit Iterator(Arg *ptr) : Iter<Arg>(ptr) {};

        Iterator(const Iterator &other) : Iter<Arg>(other.target) {};

        ~Iterator() = default;

        Iterator &operator=(const Iterator &other) {
            target = other.target;
            return *this;
        };

        Arg &operator*() const override { return *target; };

        Iterator &operator++() & override {
            ++target;
            return *this;
        };

        const Iterator operator++(int) &{ return Vector<Arg>::Iterator(target++); };

        virtual Iterator &operator--() &{
            --target;
            return *this;
        };

        virtual const Iterator operator--(int) &{ return Vector<Arg>::Iterator(target--); };

        virtual Iterator operator+(Size size);

        virtual Iterator operator-(Size size);

        virtual Iterator &operator+=(Size size);

        virtual Iterator &operator-=(Size size);

        template<typename Type>
        friend bool
        operator==(const Iterator &left, const Iterator &right) { return left.target == right.target; };

        template<typename Type>
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

    public:
        friend class Vector<Arg>;

        Shared_ptr<cIter<Arg>> deep_copy() const override;

        explicit cIterator(Arg *ptr) : cIter<Arg>(ptr) {};

        cIterator(const cIterator &other) : cIter<Arg>(other.target) {};

        ~cIterator() override = default;

        cIterator &operator=(const cIterator &other) {
            target = other.target;
            return *this;
        };

        const Arg &operator*() const override { return *target; };

        cIterator &operator++() &{
            ++target;
            return *this;
        };

        const cIterator operator++(int) &{ return Vector<Arg>::cIterator(target++); };

        virtual cIterator operator+(Size size);

        virtual cIterator operator-(Size size);

        virtual cIterator &operator+=(Size size);

        virtual cIterator &operator-=(Size size);

        virtual cIterator &operator--() &{
            --target;
            return *this;
        };

        virtual const cIterator operator--(int) &{ return Vector<Arg>::cIterator(target--); };

        template<typename Type>
        friend bool
        operator==(const cIterator &left, const cIterator &right) { return left.target == right.target; };

        template<typename Type>
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
    class Vector<Arg>::rIterator : public Vector<Arg>::Iterator {
    protected:
        using Iter<Arg>::target;

        rIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        }

    public:
        friend class Vector<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override;

        explicit rIterator(Arg *ptr) : Vector<Arg>::Iterator(ptr) {};

        rIterator(const rIterator &other) : Vector<Arg>::Iterator(other.target) {};

        ~rIterator() = default;

        rIterator &operator=(const rIterator &other) {
            target = other.target;
            return *this;
        };

        using Iterator::operator*;

        rIterator &operator++() & override {
            --target;
            return *this;
        };

        const rIterator operator++(int) &{ return Vector<Arg>::rIterator(target--); };

        rIterator &operator--() & override {
            ++target;
            return *this;
        };

        const rIterator operator--(int) & override { return Vector<Arg>::rIterator(target++); };

        rIterator operator+(Size size) override;

        rIterator operator-(Size size) override;

        rIterator &operator+=(Size size) override;

        rIterator &operator-=(Size size) override;

        template<typename Type>
        friend bool
        operator==(const rIterator &left, const rIterator &right) { return left.target == right.target; };

        template<typename Type>
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
    class Vector<Arg>::crIterator : public Vector<Arg>::cIterator {
    protected:
        using cIter<Arg>::target;

        crIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

    public:
        friend class Vector<Arg>;

        Shared_ptr<cIter<Arg>> deep_copy() const override;

        explicit crIterator(Arg *ptr) : Vector<Arg>::cIterator(ptr) {};

        crIterator(const crIterator &other) : Vector<Arg>::cIterator(other.target) {};

        ~crIterator() = default;

        crIterator &operator=(const crIterator &other) {
            target = other.target;
            return *this;
        };

        using cIterator::operator*;

        crIterator &operator++() & override {
            --target;
            return *this;
        };

        const crIterator operator++(int) &{ return Vector<Arg>::crIterator(target--); };

        crIterator &operator--() & override {
            ++target;
            return *this;
        };

        const crIterator operator--(int) & override { return Vector<Arg>::crIterator(target++); };

        crIterator operator+(Size size) override;

        crIterator operator-(Size size) override;

        crIterator &operator+=(Size size) override;

        crIterator &operator-=(Size size) override;

        template<typename Type>
        friend bool
        operator==(const crIterator &left, const crIterator &right) { return left.target == right.target; };

        template<typename Type>
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


    template<typename Arg>
    Shared_ptr<Iter<Arg>> Vector<Arg>::Iterator::deep_copy() const {
        return Shared_ptr<Vector<Arg>::Iterator>(*this);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator &Vector<Arg>::Iterator::operator-=(Size size) {
//        if (target - size < val_begin) throw outOfRange("You subtracted an out-of-range value to the function\n");
        target -= size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator &Vector<Arg>::Iterator::operator+=(Size size) {
//        if (target + size >= val_end) throw outOfRange("You added an out-of-range value to the function\n");
        target += size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::Iterator::operator-(Size size) {
//        if (target - size < val_begin) throw outOfRange("You subtracted an out-of-range value to the function\n");
        return Vector<Arg>::Iterator(target - size);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::Iterator::operator+(Size size) {
//        if (target + size >= val_end) throw outOfRange("You added an out-of-range value to the function\n");
        return Vector<Arg>::Iterator(target + size);
    }


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    Shared_ptr<cIter<Arg>> Vector<Arg>::cIterator::deep_copy() const {
        return Shared_ptr<Vector<Arg>::cIterator>(*this);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator Vector<Arg>::cIterator::operator-(Size size) {
//        if (target - size < val_begin) throw outOfRange("You subtracted an out-of-range value to the function\n");
        return Vector<Arg>::cIterator(target - size);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator Vector<Arg>::cIterator::operator+(Size size) {
//        if (target + size >= val_end) throw outOfRange("You added an out-of-range value to the function\n");
        return Vector<Arg>::cIterator(target + size);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator &Vector<Arg>::cIterator::operator+=(Size size) {
//        if (target + size >= val_end) throw outOfRange("You added an out-of-range value to the function\n");
        target += size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator &Vector<Arg>::cIterator::operator-=(Size size) {
//        if (target - size < val_begin) throw outOfRange("You subtracted an out-of-range value to the function\n");
        target -= size;
        return *this;
    }

//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    Shared_ptr<Iter<Arg>> Vector<Arg>::rIterator::deep_copy() const {
        return Shared_ptr<Vector<Arg>::rIterator>(*this);
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator &Vector<Arg>::rIterator::operator-=(Size size) {
//        if (target + size >= val_end) throw outOfRange("You subtracted an out-of-range value to the function\n");
        target += size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator &Vector<Arg>::rIterator::operator+=(Size size) {
//        if (target - size < val_begin) throw outOfRange("You added an out-of-range value to the function\n");
        target -= size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator Vector<Arg>::rIterator::operator-(Size size) {
//        if (target + size >= val_end) throw outOfRange("You subtracted an out-of-range value to the function\n");
        return Vector<Arg>::rIterator(target + size);
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator Vector<Arg>::rIterator::operator+(Size size) {
//        if (target - size < val_begin) throw outOfRange("You added an out-of-range value to the function\n");
        return Vector<Arg>::rIterator(target - size);
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Shared_ptr<cIter<Arg>> Vector<Arg>::crIterator::deep_copy() const {
        return Shared_ptr<Vector<Arg>::crIterator>(*this);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator &Vector<Arg>::crIterator::operator-=(Size size) {
//        if (target + size >= val_end) throw outOfRange("You subtracted an out-of-range value to the function\n");
        target += size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator &Vector<Arg>::crIterator::operator+=(Size size) {
//        if (target - size < val_begin) throw outOfRange("You added an out-of-range value to the function\n");
        target -= size;
        return *this;
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator Vector<Arg>::crIterator::operator-(Size size) {
//        if (target + size >= val_end) throw outOfRange("You subtracted an out-of-range value to the function\n");
        return Vector<Arg>::crIterator(target + size);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator Vector<Arg>::crIterator::operator+(Size size) {
//        if (target - size < val_begin) throw outOfRange("You added an out-of-range value to the function\n");
        return Vector<Arg>::crIterator(target - size);
    }

//----------------------------------------------------------------------------------------------------------------------

}

#endif //TINYSTL_VECTOR_HPP
