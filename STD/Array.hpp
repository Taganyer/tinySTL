//
// Created by 86152 on 2023/9/18.
//

#ifndef TINYSTL_ARRAY_HPP
#define TINYSTL_ARRAY_HPP

#include "Iter.hpp"
#include "Move.hpp"
#include "Allocater.hpp"
#include "Memory.hpp"

namespace STD {
    template<typename Arg, Size size_>
    class Array {
    private:

        Arg *val_begin;

    public:

        class Iterator;

        class cIterator;

        class rIterator;

        class crIterator;

        Array();

        Array(const std::initializer_list<Arg> &list);

        Array(const Iter<Arg> &begin, const Iter<Arg> &end);

        Array(const cIter<Arg> &begin, const cIter<Arg> &end);

        Array(const Array<Arg, size_> &other);

        Array(Array<Arg, size_> &&other) noexcept;

        ~Array<Arg, size_>();

        void fill(const Arg &target);

        Arg &operator[](Size pos) const { return *(val_begin + pos); };

        Arg &at(Size pos) const {
            if (pos >= size_) throw outOfRange("You provided an out-of-range subscript int the 'Array::at' function");
            return *(val_begin + pos);
        };

        Arg &front() const { return *val_begin; };

        Arg &back() const { return *(val_begin + size_ - 1); };

        bool empty() const { return !size_; };

        const Arg *data() { return val_begin; };

        Array<Arg, size_> &operator=(const Array<Arg, size_> &other);

        Array<Arg, size_> &operator=(Array<Arg, size_> &&other) noexcept;

        template<typename Type>
        friend bool operator==(const Array<Arg, size_> &left, const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator!=(const Array<Arg, size_> &left, const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator<(const Array<Arg, size_> &left, const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator<=(const Array<Arg, size_> &left, const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator>(const Array<Arg, size_> &left, const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator>=(const Array<Arg, size_> &left, const Array<Arg, size_> &right);

        Iterator begin() const { return Array<Arg, size_>::Iterator(val_begin); };

        Iterator end() const { return Array<Arg, size_>::Iterator(val_begin + size_); };

        cIterator cbegin() const { return Array<Arg, size_>::cIterator(val_begin); };

        cIterator cend() const { return Array<Arg, size_>::cIterator(val_begin + size_); };

        rIterator rbegin() const { return Array<Arg, size_>::rIterator(val_begin + size_ - 1); };

        rIterator rend() const { return Array<Arg, size_>::rIterator(val_begin - 1); };

        crIterator crbegin() const { return Array<Arg, size_>::crIterator(val_begin + size_ - 1); };

        crIterator crend() const { return Array<Arg, size_>::crIterator(val_begin - 1); };
    };

    template<typename Arg, Size size_>
    Array<Arg, size_>::Array() : val_begin(Allocate_n<Arg>(size_)) {
        auto temp = val_begin;
        for (int i = 0; i < size_; ++i) {
            *temp = Arg();
            ++temp;
        }
    }

    template<typename Arg, Size size_>
    Array<Arg, size_>::Array(const std::initializer_list<Arg> &list) : val_begin(Allocate_n<Arg>(size_)) {
        auto temp = val_begin;
        auto index = list.begin();
        Size i, size = list.size() > size_ ? size_ : list.size();
        for (i = 0; i < size; ++i) {
            *temp = *index;
            ++temp, ++index;
        }
        for (; i < size_; ++i) {
            *temp = Arg();
            ++temp;
        }
    }

    template<typename Arg, Size size_>
    Array<Arg, size_>::Array(const Iter<Arg> &begin, const Iter<Arg> &end) : val_begin(Allocate_n<Arg>(size_)) {
        auto temp = val_begin;
        auto iter = begin.deep_copy();
        for (int i = 0; i < size_; ++i) {
            *temp = **iter;
            ++temp, ++(*iter);
        }
    }

    template<typename Arg, Size size_>
    Array<Arg, size_>::Array(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto temp = val_begin;
        auto iter = begin.deep_copy();
        for (int i = 0; i < size_; ++i) {
            *temp = **iter;
            ++temp, ++(*iter);
        }
    }

    template<typename Arg, Size size_>
    Array<Arg, size_>::Array(const Array<Arg, size_> &other) {
        auto temp = val_begin;
        auto iter = other.val_begin;
        for (int i = 0; i < size_; ++i) {
            *temp = *iter;
            ++temp, ++iter;
        }
    }

    template<typename Arg, Size size_>
    Array<Arg, size_>::Array(Array<Arg, size_> &&other) noexcept : val_begin(other.val_begin) {
        other.val_begin = Allocate_n<Arg>(size_);
    }

    template<typename Arg, Size size_>
    Array<Arg, size_>::~Array<Arg, size_>() {
        Deallocate_n(val_begin);
    }

    template<typename Arg, Size size_>
    Array<Arg, size_> &Array<Arg, size_>::operator=(const Array<Arg, size_> &other) {
        if (this == &other) return *this;
        auto temp = val_begin, iter = other.val_begin;
        for (Size i = 0; i < size_; ++i) {
            *temp = *iter;
            ++temp, ++iter;
        }
        return *this;
    }

    template<typename Arg, Size size_>
    Array<Arg, size_> &Array<Arg, size_>::operator=(Array<Arg, size_> &&other)  noexcept {
        if (this == &other) return *this;
        val_begin = other.val_begin;
        other.val_begin = Allocate_n<Arg>(size_);
        return *this;
    }

    template<typename Type, typename Arg, Size size_>
    bool operator==(const Array<Arg, size_> &left, const Array<Arg, size_> &right) {
        auto l = left.val_begin, r = right.val_begin;
        for (Size i = 0; i < size_; ++i) {
            if (*l != *r) return false;
            ++l, ++r;
        }
        return true;
    }

    template<typename Type, typename Arg, Size size_>
    bool operator!=(const Array<Arg, size_> &left, const Array<Arg, size_> &right) {
        return !(left == right);
    }

    template<typename Type, typename Arg, Size size_>
    bool operator<(const Array<Arg, size_> &left, const Array<Arg, size_> &right) {
        auto l = left.val_begin, r = right.val_begin;
        for (Size i = 0; i < size_; ++i) {
            if (*l != *r) return *l < *r;
            ++l, ++r;
        }
        return false;
    }

    template<typename Type, typename Arg, Size size_>
    bool operator<=(const Array<Arg, size_> &left, const Array<Arg, size_> &right) {
        return !(left > right);
    }

    template<typename Type, typename Arg, Size size_>
    bool operator>(const Array<Arg, size_> &left, const Array<Arg, size_> &right) {
        auto l = left.val_begin, r = right.val_begin;
        for (Size i = 0; i < size_; ++i) {
            if (*l != *r) return *l > *r;
            ++l, ++r;
        }
        return false;
    }

    template<typename Type, typename Arg, Size size_>
    bool operator>=(const Array<Arg, size_> &left, const Array<Arg, size_> &right) {
        return !(left < right);
    }

    template<typename Arg, Size size_>
    void Array<Arg, size_>::fill(const Arg &target) {
        auto temp = val_begin;
        for (int i = 0; i < size_; ++i) {
            *temp = target;
            ++temp;
        }
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg, Size size_>
    class Array<Arg, size_>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        Iterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit Iterator(Arg *ptr) : Iter<Arg>(ptr) {};

    public:
        friend class Array<Arg, size_>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Array<Arg, size_>::Iterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Array<Arg, size_>::cIterator>(Array<Arg, size_>::cIterator(target));
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

        Iterator operator++(int) { return Array<Arg, size_>::Iterator(target++); };

        Iterator &operator--() {
            --target;
            return *this;
        };

        Iterator operator--(int) { return Array<Arg, size_>::Iterator(target--); };

        Iterator operator+(Size size) const { return Array<Arg, size_>::Iterator(target + size); };

        Iterator operator-(Size size) const { return Array<Arg, size_>::Iterator(target - size); };

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

    template<typename Arg, Size size_>
    class Array<Arg, size_>::cIterator : public cIter<Arg> {
    protected:
        using cIter<Arg>::target;

        cIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit cIterator(Arg *ptr) : cIter<Arg>(ptr) {};

    public:
        friend class Array<Arg, size_>;

        friend class Array<Arg, size_>::Iterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Array<Arg, size_>::cIterator>(*this); };

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

        cIterator operator++(int) { return Array<Arg, size_>::cIterator(target++); };

        cIterator operator+(Size size) const { return Array<Arg, size_>::cIterator(target + size); };

        cIterator operator-(Size size) const { return Array<Arg, size_>::cIterator(target - size); };

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

        cIterator operator--(int) { return Array<Arg, size_>::cIterator(target--); };

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

    template<typename Arg, Size size_>
    class Array<Arg, size_>::rIterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        rIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        }

        explicit rIterator(Arg *ptr) : Iter<Arg>(ptr) {};

    public:
        friend class Array<Arg, size_>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Array<Arg, size_>::rIterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Array<Arg, size_>::crIterator>(Array<Arg, size_>::crIterator(target));
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

        rIterator operator++(int) { return Array<Arg, size_>::rIterator(target--); };

        rIterator &operator--() {
            ++target;
            return *this;
        };

        rIterator operator--(int) { return Array<Arg, size_>::rIterator(target++); };

        rIterator operator+(Size size) const { return Array<Arg, size_>::rIterator(target - size); };

        rIterator operator-(Size size) const { return Array<Arg, size_>::rIterator(target + size); };

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

    template<typename Arg, Size size_>
    class Array<Arg, size_>::crIterator : public cIter<Arg> {
    protected:
        using cIter<Arg>::target;

        crIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit crIterator(Arg *ptr) : cIter<Arg>(ptr) {};

    public:
        friend class Array<Arg, size_>;

        friend class Array<Arg, size_>::rIterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Array<Arg, size_>::crIterator>(*this); };

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

        crIterator operator++(int) { return Array<Arg, size_>::crIterator(target--); };

        crIterator &operator--() {
            ++target;
            return *this;
        };

        crIterator operator--(int) { return Array<Arg, size_>::crIterator(target++); };

        crIterator operator+(Size size) const { return Array<Arg, size_>::crIterator(target - size); };

        crIterator operator-(Size size) const { return Array<Arg, size_>::crIterator(target + size); };

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

}

#endif //TINYSTL_ARRAY_HPP
