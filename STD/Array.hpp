//
// Created by 86152 on 2023/9/18.
//

#ifndef TINYSTL_ARRAY_HPP
#define TINYSTL_ARRAY_HPP

#include "Allocater.hpp"
#include "Iter.hpp"
#include "Memory.hpp"
#include "Move.hpp"

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
            if (pos >= size_)
                throw outOfRange("You provided an out-of-range subscript int the 'Array::at' function");
            return *(val_begin + pos);
        };

        Arg &front() const { return *val_begin; };

        Arg &back() const { return *(val_begin + size_ - 1); };

        bool empty() const { return !size_; };

        const Arg *data() { return val_begin; };

        Array<Arg, size_> &operator=(const Array<Arg, size_> &other);

        Array<Arg, size_> &operator=(Array<Arg, size_> &&other) noexcept;

        template<typename Type>
        friend bool operator==(const Array<Arg, size_> &left,
                               const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator!=(const Array<Arg, size_> &left,
                               const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator<(const Array<Arg, size_> &left,
                              const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator<=(const Array<Arg, size_> &left,
                               const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator>(const Array<Arg, size_> &left,
                              const Array<Arg, size_> &right);

        template<typename Type>
        friend bool operator>=(const Array<Arg, size_> &left,
                               const Array<Arg, size_> &right);

        Iterator begin() const { return Array<Arg, size_>::Iterator(val_begin); };

        Iterator end() const {
            return Array<Arg, size_>::Iterator(val_begin + size_);
        };

        cIterator cbegin() const {
            return Array<Arg, size_>::cIterator(val_begin);
        };

        cIterator cend() const {
            return Array<Arg, size_>::cIterator(val_begin + size_);
        };

        rIterator rbegin() const {
            return Array<Arg, size_>::rIterator(val_begin + size_ - 1);
        };

        rIterator rend() const {
            return Array<Arg, size_>::rIterator(val_begin - 1);
        };

        crIterator crbegin() const {
            return Array<Arg, size_>::crIterator(val_begin + size_ - 1);
        };

        crIterator crend() const {
            return Array<Arg, size_>::crIterator(val_begin - 1);
        };
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
    Array<Arg, size_>::Array(const std::initializer_list<Arg> &list)
        : val_begin(Allocate_n<Arg>(size_)) {
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
    Array<Arg, size_>::Array(const Iter<Arg> &begin, const Iter<Arg> &end)
        : val_begin(Allocate_n<Arg>(size_)) {
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
    Array<Arg, size_>::Array(Array<Arg, size_> &&other) noexcept
        : val_begin(other.val_begin) {
        other.val_begin = Allocate_n<Arg>(size_);
    }

    template<typename Arg, Size size_>
    Array<Arg, size_>::~Array<Arg, size_>() {
        Deallocate_n(val_begin);
    }

    template<typename Arg, Size size_>
    Array<Arg, size_> &
    Array<Arg, size_>::operator=(const Array<Arg, size_> &other) {
        if (this == &other)
            return *this;
        auto temp = val_begin, iter = other.val_begin;
        for (Size i = 0; i < size_; ++i) {
            *temp = *iter;
            ++temp, ++iter;
        }
        return *this;
    }

    template<typename Arg, Size size_>
    Array<Arg, size_> &Array<Arg, size_>::operator=(Array<Arg, size_> &&other) noexcept {
        if (this == &other)
            return *this;
        val_begin = other.val_begin;
        other.val_begin = Allocate_n<Arg>(size_);
        return *this;
    }

    template<typename Type, typename Arg, Size size_>
    bool operator==(const Array<Arg, size_> &left, const Array<Arg, size_> &right) {
        auto l = left.val_begin, r = right.val_begin;
        for (Size i = 0; i < size_; ++i) {
            if (*l != *r)
                return false;
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
            if (*l != *r)
                return *l < *r;
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
            if (*l != *r)
                return *l > *r;
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
    class Array<Arg, size_>::Iterator : public Random_Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        Iterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit Iterator(Arg *ptr) : Random_Iter<Arg>(ptr){};

    public:
        friend class Array<Arg, size_>;

        Shared_ptr<Iter<Arg>> deep_copy() const override {
            return make_shared<Array<Arg, size_>::Iterator>(*this);
        };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Array<Arg, size_>::cIterator>(
                    Array<Arg, size_>::cIterator(target));
        };

        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        using Random_Iter<Arg>::operator[];

        Iterator &operator++() override {
            ++target;
            return *this;
        };

        Iterator operator++(int) { return Array<Arg, size_>::Iterator(target++); };

        Iterator &operator--() override {
            --target;
            return *this;
        };

        Iterator operator--(int) { return Array<Arg, size_>::Iterator(target--); };

        Iterator operator+(Size size) const {
            return Array<Arg, size_>::Iterator(target + size);
        };

        Iterator operator-(Size size) const {
            return Array<Arg, size_>::Iterator(target - size);
        };

        Iterator &operator+=(Size size) override {
            target += size;
            return *this;
        };

        Iterator &operator-=(Size size) override {
            target -= size;
            return *this;
        };

        friend bool operator==(const Iterator &left, const Iterator &right) {
            return left.target == right.target;
        };

        friend bool operator!=(const Iterator &left, const Iterator &right) {
            return left.target != right.target;
        };

        friend bool operator>(const Iterator &left, const Iterator &right) {
            return left.target > right.target;
        };

        friend bool operator<(const Iterator &left, const Iterator &right) {
            return left.target < right.target;
        };

        friend bool operator>=(const Iterator &left, const Iterator &right) {
            return left.target >= right.target;
        };

        friend bool operator<=(const Iterator &left, const Iterator &right) {
            return left.target <= right.target;
        };

        friend long long operator-(const Iterator &left, const Iterator &right) {
            return left.target - right.target;
        };
    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Arg, Size size_>
    class Array<Arg, size_>::cIterator : public cRandom_Iter<Arg> {
    protected:
        using cIter<Arg>::target;

        cIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit cIterator(Arg *ptr) : cRandom_Iter<Arg>(ptr){};

    public:
        friend class Array<Arg, size_>;

        friend class Array<Arg, size_>::Iterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override {
            return make_shared<Array<Arg, size_>::cIterator>(*this);
        };

        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        using cRandom_Iter<Arg>::operator[];

        cIterator &operator++() override {
            ++target;
            return *this;
        };

        cIterator operator++(int) {
            return Array<Arg, size_>::cIterator(target++);
        };

        cIterator &operator--() override {
            --target;
            return *this;
        };

        cIterator operator--(int) {
            return Array<Arg, size_>::cIterator(target--);
        };

        cIterator operator+(Size size) const {
            return Array<Arg, size_>::cIterator(target + size);
        };

        cIterator operator-(Size size) const {
            return Array<Arg, size_>::cIterator(target - size);
        };

        cIterator &operator+=(Size size) override {
            target += size;
            return *this;
        };

        cIterator &operator-=(Size size) override {
            target -= size;
            return *this;
        };

        friend bool operator==(const cIterator &left, const cIterator &right) {
            return left.target == right.target;
        };

        friend bool operator!=(const cIterator &left, const cIterator &right) {
            return left.target != right.target;
        };

        friend bool operator>(const cIterator &left, const cIterator &right) {
            return left.target > right.target;
        };

        friend bool operator<(const cIterator &left, const cIterator &right) {
            return left.target < right.target;
        };

        friend bool operator>=(const cIterator &left, const cIterator &right) {
            return left.target >= right.target;
        };

        friend bool operator<=(const cIterator &left, const cIterator &right) {
            return left.target <= right.target;
        };

        friend long long operator-(const cIterator &left, const cIterator &right) {
            return left.target - right.target;
        };
    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Arg, Size size_>
    class Array<Arg, size_>::rIterator : public Random_Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        rIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        }

        explicit rIterator(Arg *ptr) : Random_Iter<Arg>(ptr){};

        bool less(const Random_Iter<Arg> &other) const override {
            return target > dynamic_cast<const rIterator &>(other).target;
        }

        bool not_greater_than(const Random_Iter<Arg> &other) const override {
            return target >= dynamic_cast<const rIterator &>(other).target;
        }

    public:
        friend class Array<Arg, size_>;

        Shared_ptr<Iter<Arg>> deep_copy() const override {
            return make_shared<Array<Arg, size_>::rIterator>(*this);
        };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Array<Arg, size_>::crIterator>(
                    Array<Arg, size_>::crIterator(target));
        };

        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        Arg &operator[](Size size) const override { return *(target - size); }

        rIterator &operator++() override {
            --target;
            return *this;
        };

        rIterator operator++(int) {
            return Array<Arg, size_>::rIterator(target--);
        };

        rIterator &operator--() {
            ++target;
            return *this;
        };

        rIterator operator--(int) {
            return Array<Arg, size_>::rIterator(target++);
        };

        rIterator operator+(Size size) const {
            return Array<Arg, size_>::rIterator(target - size);
        };

        rIterator operator-(Size size) const {
            return Array<Arg, size_>::rIterator(target + size);
        };

        rIterator &operator+=(Size size) override {
            target -= size;
            return *this;
        };

        rIterator &operator-=(Size size) override {
            target += size;
            return *this;
        };

        friend bool operator==(const rIterator &left, const rIterator &right) {
            return left.target == right.target;
        };

        friend bool operator!=(const rIterator &left, const rIterator &right) {
            return left.target != right.target;
        };

        friend bool operator>(const rIterator &left, const rIterator &right) {
            return left.target < right.target;
        };

        friend bool operator<(const rIterator &left, const rIterator &right) {
            return left.target > right.target;
        };

        friend bool operator>=(const rIterator &left, const rIterator &right) {
            return left.target <= right.target;
        };

        friend bool operator<=(const rIterator &left, const rIterator &right) {
            return left.target >= right.target;
        };

        friend long long operator-(const rIterator &left, const rIterator &right) {
            return right.target - left.target;
        };
    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Arg, Size size_>
    class Array<Arg, size_>::crIterator : public cRandom_Iter<Arg> {
    protected:
        using cIter<Arg>::target;

        crIterator &operator=(Arg *ptr) {
            target = ptr;
            return *this;
        };

        explicit crIterator(Arg *ptr) : cRandom_Iter<Arg>(ptr){};

        bool less(const cRandom_Iter<Arg> &other) const override {
            return target > dynamic_cast<const crIterator &>(other).target;
        }

        bool not_greater_than(const cRandom_Iter<Arg> &other) const override {
            return target >= dynamic_cast<const crIterator &>(other).target;
        }

    public:
        friend class Array<Arg, size_>;

        friend class Array<Arg, size_>::rIterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override {
            return make_shared<Array<Arg, size_>::crIterator>(*this);
        };

        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        Arg &operator[](Size size) const override { return *(target - size); }

        crIterator &operator++() override {
            --target;
            return *this;
        };

        crIterator operator++(int) {
            return Array<Arg, size_>::crIterator(target--);
        };

        crIterator &operator--() override {
            ++target;
            return *this;
        };

        crIterator operator--(int) {
            return Array<Arg, size_>::crIterator(target++);
        };

        crIterator operator+(Size size) const {
            return Array<Arg, size_>::crIterator(target - size);
        };

        crIterator operator-(Size size) const {
            return Array<Arg, size_>::crIterator(target + size);
        };

        crIterator &operator+=(Size size) override {
            target -= size;
            return *this;
        };

        crIterator &operator-=(Size size) override {
            target += size;
            return *this;
        };

        friend bool operator==(const crIterator &left, const crIterator &right) {
            return left.target == right.target;
        };

        friend bool operator!=(const crIterator &left, const crIterator &right) {
            return left.target != right.target;
        };

        friend bool operator>(const crIterator &left, const crIterator &right) {
            return left.target < right.target;
        };

        friend bool operator<(const crIterator &left, const crIterator &right) {
            return left.target > right.target;
        };

        friend bool operator>=(const crIterator &left, const crIterator &right) {
            return left.target <= right.target;
        };

        friend bool operator<=(const crIterator &left, const crIterator &right) {
            return left.target >= right.target;
        };

        friend long long operator-(const crIterator &left,
                                   const crIterator &right) {
            return right.target - left.target;
        };
    };

}// namespace STD

#endif// TINYSTL_ARRAY_HPP
