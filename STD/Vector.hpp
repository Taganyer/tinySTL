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

    public:

        class Iterator;

        class cIterator;

        class rIterator;

        class crIterator;

        Vector() = default;

        Vector(Size size, Arg target = Arg());

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

        Vector<Arg>& assign(const Iter<Arg> &begin, const Iter<Arg> &end);

        Vector<Arg>& assign(const cIter<Arg> &begin, const cIter<Arg> &end);

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(Arg &val);

        void push_back(const Iter<Arg> &begin, const Iter<Arg> &end);

        void push_back(const cIter<Arg> &begin, const cIter<Arg> &end);

        void pop_back();

        template<typename ...args>
        Iterator emplace(Size pos, args &&...vals);

        Iterator insert(Size pos, const Arg &val);

        Iterator insert(Size pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        cIterator insert(Size pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator erase(Size pos, bool until_end = false);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        Arg &operator[](Size pos) const { return *(val_begin + pos); };

        Arg &at(Size pos) const {
            if (pos >= size_) throw outOfRange("You provided an out-of-range subscript int the 'Vector::at' function");
            return *(val_begin + pos);
        };

        Arg &front() {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'Vector::front' function");
            return *val_begin;
        };

        Arg &back() {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'Vector::back' function");
            return *(val_end - 1);
        };

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

        Iterator begin() { return Vector<Arg>::Iterator(val_begin); };

        Iterator end() { return Vector<Arg>::Iterator(val_end); };

        cIterator cbegin() { return Vector<Arg>::cIterator(val_begin); };

        cIterator cend() { return Vector<Arg>::cIterator(val_end); };

        rIterator rbegin() { return Vector<Arg>::rIterator(val_end - 1); };

        rIterator rend() { return Vector<Arg>::rIterator(val_begin - 1); };

        crIterator crbegin() { return Vector<Arg>::crIterator(val_end - 1); };

        crIterator crend() { return Vector<Arg>::crIterator(val_begin - 1); }

    };


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Vector<Arg>::Vector(Size size, Arg target) : val_begin(Allocate_n<Arg>(size + size / 5)), size_(size) {
        val_end = val_begin + size;
        store_end = val_begin + size + size / 5;
        auto temp = val_begin;
        for (int i = 0; i < size; ++i) {
            *temp = target;
            ++temp;
        }
    }

    template<typename Arg>
    Vector<Arg>::Vector(const Iter<Arg> &begin, const Iter<Arg> &end) : size_(calculateLength(begin, end)) {
        auto iter = begin.deep_copy();
        iter = begin.deep_copy();
        val_begin = Allocate_n<Arg>(size_);
        store_end = val_end = val_begin + size_;
        while (*iter != end) {
            *val_begin = **iter;
            ++val_begin, ++(*iter);
        }
        val_begin = val_end - size_;
    }

    template<typename Arg>
    Vector<Arg>::Vector(const cIter<Arg> &begin, const cIter<Arg> &end) : size_(calculateLength(begin, end)) {
        auto iter = begin.deep_copy();
        val_begin = Allocate_n<Arg>(size_);
        store_end = val_end = val_begin + size_;
        while (*iter != end) {
            *val_begin = **iter;
            ++val_begin, ++(*iter);
        }
        val_begin = val_end - size_;
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
    template<typename... args>
    typename Vector<Arg>::Iterator Vector<Arg>::emplace(Size pos, args &&... vals) {
        if (pos > size_) throw outOfRange("Your function 'Vector::emplace' was passed an out-of-range position\n");
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
        if (pos > size_) throw outOfRange("Your function 'Vector::insert' was passed an out-of-range position\n");
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
    typename Vector<Arg>::Iterator Vector<Arg>::insert(Size pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Vector::insert' function");
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
            while (*temp != end) {
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
            auto temp1 = val_end - 1, temp2 = val_end + count - 1, target_end = val_begin + pos;
            while (temp1 >= target_end) {
                *temp2 = move(*temp1);
                --temp1, --temp2;
            }
            while (*temp != end) {
                *target_end = **temp;
                ++(*temp), ++target_end;
            }
            size_ += count;
            val_end = val_begin + size_;
        }
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator
    Vector<Arg>::insert(Size pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Vector::insert' function");
        auto temp(begin.deep_copy());
        Size count = 0;
        while (*temp != end) ++count, ++(*temp);
        if (!count) return cIterator(val_begin + pos);
        temp = begin.deep_copy();
        if (capacity() - size_ < count) {
            auto t_begin = Allocate_n<Arg>(capacity() + count), t = t_begin;
            store_end = t_begin + capacity() + count;
            for (int i = 0; i < pos; ++i) {
                *t_begin = move(*val_begin);
                ++t_begin, ++val_begin;
            }
            while (*temp != end) {
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
            auto temp1 = val_end - 1, temp2 = val_end + count - 1, target_end = val_begin + pos;
            while (temp1 >= target_end) {
                *temp2 = move(*temp1);
                --temp1, --temp2;
            }
            while (*temp != end) {
                *target_end = **temp;
                ++(*temp), ++target_end;
            }
            size_ += count;
            val_end = val_begin + size_;
        }
        return cIterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::erase(Size pos, bool until_end) {
        if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'Vector::erase' function");
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
    typename Vector<Arg>::Iterator Vector<Arg>::erase(const Vector::Iterator &iter) {
        if (iter.target < val_begin)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::erase' function");
        if (iter.target != val_end) {
            --size_;
            auto temp1 = iter.target, temp2 = temp1 + 1;
            temp1->~Arg();
            while (temp2 != val_end) {
                *temp1 = move(*temp2);
                ++temp1, ++temp2;
            }
            --val_end;
        }
        return iter;
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator Vector<Arg>::erase(const Vector::cIterator &iter) {
        if (iter.target < val_begin)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::erase' function");
        auto temp(iter.target);
        if (iter.target != val_end) {
            --size_;
            auto temp1 = iter.target, temp2 = temp1 + 1;
            temp1->~Arg();
            while (temp2 != val_end) {
                *temp1 = move(*temp2);
                ++temp1, ++temp2;
            }
            --val_end;
        }
        return iter;
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::erase(const Vector::Iterator &begin, const Vector::Iterator &end) {
        if (end < begin || begin.target < val_begin || end.target > val_end)
            throw outOfRange("You passed in a pair of out-of-range iterators in the 'Vector::erase' function");
        auto temp1(begin.target), temp2(end.target);
        while (temp1 != temp2) {
            temp1->~Arg();
            ++temp1;
        }
        temp1 = begin.target;
        while (temp2 != val_end) {
            *temp1 = move(*temp2);
            ++temp1, ++temp2;
        }
        val_end = temp1;
        size_ = val_end - val_begin;
        return begin;
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator
    Vector<Arg>::erase(const Vector::cIterator &begin, const Vector::cIterator &end) {
        if (end < begin || begin.target < val_begin || end.target > val_end)
            throw outOfRange("You passed in a pair of out-of-range iterators in the 'Vector::erase' function");
        auto temp1(begin.target), temp2(end.target);
        while (temp1 != temp2) {
            temp1->~Arg();
            ++temp1;
        }
        temp1 = begin.target;
        while (temp2 != val_end) {
            *temp1 = move(*temp2);
            ++temp1, ++temp2;
        }
        val_end = temp1;
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
        Size count = calculateLength(begin, end);
        if (capacity() - size_ < count) reallocate(capacity() + count);
        size_ += count;
        while (*temp != end) {
            *val_end = **temp;
            ++val_end, ++(*temp);
        }
    }

    template<typename Arg>
    void Vector<Arg>::push_back(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto temp(begin.deep_copy());
        Size count = calculateLength(begin, end);
        if (capacity() - size_ < count) reallocate(capacity() + count);
        size_ += count;
        while (*temp != end) {
            *val_end = **temp;
            ++val_end, ++(*temp);
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
    Vector<Arg>& Vector<Arg>::assign(const Iter<Arg> &begin, const Iter<Arg> &end) {
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
        return *this;
    }

    template<typename Arg>
    Vector<Arg>& Vector<Arg>::assign(const cIter<Arg> &begin, const cIter<Arg> &end) {
        Deallocate_n(val_begin);
        auto temp = begin.deep_copy();
        Size count = calculateLength(begin, end);
        auto t = Allocate_n<Arg>(count);
        while (*temp != end) {
            *t = **temp;
            ++t, ++(*temp);
        }
        store_end = val_end = t;
        val_begin = t - count;
        size_ = count;
        return *this;
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

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::operator=(Vector<Arg> &&other) noexcept {
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


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Vector<Arg>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        virtual Iterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit Iterator(Arg *ptr) : Iter<Arg>(ptr) {};

    public:
        friend class Vector<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Vector<Arg>::Iterator>(*this); };

        Iterator(const Iterator &other) : Iter<Arg>(other.target) {};

        ~Iterator() = default;

        //该函数如果传入rIterator作为参数可能会有意外的结果
        Iterator &operator=(const Iterator &other) {
            target = other.target;
            return *this;
        };


        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        Iterator &operator++() & override {
            ++target;
            return *this;
        };

        Iterator operator++(int) &{ return Vector<Arg>::Iterator(target++); };

        virtual Iterator &operator--() &{
            --target;
            return *this;
        };

        Iterator operator--(int) &{ return Vector<Arg>::Iterator(target--); };

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

        virtual cIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit cIterator(Arg *ptr) : cIter<Arg>(ptr) {};

    public:
        friend class Vector<Arg>;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Vector<Arg>::cIterator>(*this); };

        cIterator(const cIterator &other) : cIter<Arg>(other.target) {};

        ~cIterator() override = default;

        //该函数如果传入crIterator作为参数可能会有意外的结果
        cIterator &operator=(const cIterator &other) {
            target = other.target;
            return *this;
        };

        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        cIterator &operator++() & override {
            ++target;
            return *this;
        };

        cIterator operator++(int) &{ return Vector<Arg>::cIterator(target++); };

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

        virtual cIterator &operator--() &{
            --target;
            return *this;
        };

        cIterator operator--(int) &{ return Vector<Arg>::cIterator(target--); };

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
    class Vector<Arg>::rIterator : public Vector<Arg>::Iterator {
    protected:
        using Iter<Arg>::target;

        rIterator &operator=(Arg *ptr) override {
            target = ptr;
            return *this;
        }

        explicit rIterator(Arg *ptr) : Vector<Arg>::Iterator(ptr) {};

    public:
        friend class Vector<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Vector<Arg>::rIterator>(*this); };

        rIterator(const rIterator &other) : Vector<Arg>::Iterator(other.target) {};

        ~rIterator() = default;

        rIterator &operator=(const rIterator &other) {
            target = other.target;
            return *this;
        };

        using Iterator::operator*;

        using Iterator::operator->;

        rIterator &operator++() & override {
            --target;
            return *this;
        };

        rIterator operator++(int) &{ return Vector<Arg>::rIterator(target--); };

        rIterator &operator--() & override {
            ++target;
            return *this;
        };

        rIterator operator--(int) &{ return Vector<Arg>::rIterator(target++); };

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
    class Vector<Arg>::crIterator : public Vector<Arg>::cIterator {
    protected:
        using cIter<Arg>::target;

        crIterator &operator=(Arg *ptr) override {
            target = ptr;
            return *this;
        };

        explicit crIterator(Arg *ptr) : Vector<Arg>::cIterator(ptr) {};

    public:
        friend class Vector<Arg>;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Vector<Arg>::crIterator>(*this); };

        crIterator(const crIterator &other) : Vector<Arg>::cIterator(other.target) {};

        ~crIterator() = default;

        crIterator &operator=(const crIterator &other) {
            target = other.target;
            return *this;
        };

        using cIterator::operator*;

        using cIterator::operator->;

        crIterator &operator++() & override {
            --target;
            return *this;
        };

        crIterator operator++(int) &{ return Vector<Arg>::crIterator(target--); };

        crIterator &operator--() & override {
            ++target;
            return *this;
        };

        crIterator operator--(int) &{ return Vector<Arg>::crIterator(target++); };

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
