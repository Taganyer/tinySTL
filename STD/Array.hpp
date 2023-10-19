//
// Created by 86152 on 2023/9/18.
//

#ifndef TINYSTL_ARRAY_HPP
#define TINYSTL_ARRAY_HPP

#include "Detail/rIter.hpp"
#include "Detail/cIter.hpp"
#include "Detail/crIter.hpp"
#include "Allocater.hpp"
#include <initializer_list>


namespace STD {

    template<typename Arg, Size size_>
    class Array;

    namespace Detail {

        template<typename Arg, Size size_>
        class Array_Iterator : public Iterator<Random_iterator_tag, Arg> {
        public:
            using Basic = Iterator<Random_iterator_tag, Arg>;

            using Self = Array_Iterator<Arg, size_>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            friend class Array<Arg, size_>;

            friend class rIterator<Self, Random_iterator_tag>;

            friend class cIterator<Self, Random_iterator_tag>;

            friend class crIterator<Self, Random_iterator_tag>;

        private:
            Arg *target;

            explicit Array_Iterator(Arg *target) : target(target) {};

        public:
            Reference operator*() const {
                return *target;
            };

            Pointer operator->() const {
                return target;
            };

            Reference operator[](Step size) const {
                return target[size];
            }

            Self &operator++() {
                ++target;
                return *this;
            }

            Self operator++(int) {
                Self temp = Self(target++);
                return temp;
            };

            Self &operator--() {
                --target;
                return *this;
            };

            Self operator--(int) {
                Self temp = Self(target--);
                return temp;
            };

            Self &operator+=(Step size) {
                target += size;
                return *this;
            };

            Self &operator-=(Step size) {
                target -= size;
                return *this;
            };

            friend Self operator+(const Self &Iterator, Step size) {
                return Self(Iterator.target + size);
            };

            friend Self operator-(const Self &Iterator, Step size) {
                return Self(Iterator.target - size);
            };

            friend bool operator==(const Self &left, const Self &right) {
                return left.target == right.target;
            };

            friend bool operator!=(const Self &left, const Self &right) {
                return left.target != right.target;
            };

            friend bool operator<(const Self &left, const Self &right) {
                return left.target < right.target;
            };

            friend bool operator<=(const Self &left, const Self &right) {
                return left.target <= right.target;
            };

            friend bool operator>(const Self &left, const Self &right) {
                return left.target > right.target;
            };

            friend bool operator>=(const Self &left, const Self &right) {
                return left.target >= right.target;
            };
        };

    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg, Size size_>
    class Array {
    private:
        Arg *val_begin = Allocate_n<Arg>(size_);

    public:
        using Iterator = Detail::Array_Iterator<Arg, size_>;

        using cIterator = STD::cIterator<Iterator, Random_iterator_tag>;

        using rIterator = STD::rIterator<Iterator, Random_iterator_tag>;

        using crIterator = STD::crIterator<Iterator, Random_iterator_tag>;

        Array();

        Array(const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Array(const Input_iterator &begin, const Input_iterator &end);

        Array(const Array<Arg, size_> &other);

        Array(Array<Arg, size_> &&other) noexcept;

        ~Array<Arg, size_>();

        Arg &operator[](Size pos) const { return val_begin[pos]; };

        Arg &at(Size pos) const {
            if (pos >= size_)
                throw outOfRange("You provided an out-of-range subscript int the 'Array::at' function");
            return val_begin[pos];
        };

        Arg &front() const { return val_begin[0]; };

        Arg &back() const { return val_begin[size_ - 1]; };

        bool empty() const { return !size_; };

        const Arg *data() { return val_begin; };

        void swap(Array<Arg, size_> &other) noexcept {
            Arg *temp = val_begin;
            val_begin = other.val_begin;
            other.val_begin = temp;
        };

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

        template<typename Type>
        friend void swap(Array<Arg, size_> &left, Array<Arg, size_> &right) noexcept {
            left.swap(right);
        };

        Iterator begin() const { return Iterator(val_begin); };

        Iterator end() const { return Iterator(val_begin + size_); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin)); };

        cIterator cend() const { return cIterator(Iterator(val_begin + size_)); };

        rIterator rbegin() const { return rIterator(Iterator(val_begin + size_ - 1)); };

        rIterator rend() const { return rIterator(Iterator(val_begin - 1)); };

        crIterator crbegin() const { return crIterator(Iterator(val_begin + size_ - 1)); };

        crIterator crend() const { return crIterator(Iterator(val_begin - 1)); };
    };

    template<typename Arg, Size size_>
    Array<Arg, size_>::Array() {
        auto temp = val_begin;
        for (int i = 0; i < size_; ++i) {
            *temp = Arg();
            ++temp;
        }
    }

    template<typename Arg, Size size_>
    Array<Arg, size_>::Array(const std::initializer_list<Arg> &list) {
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
    template<typename Input_iterator>
    Array<Arg, size_>::Array(const Input_iterator &begin, const Input_iterator &end)
            : val_begin(Allocate_n<Arg>(size_)) {
        Fill_With(val_begin, begin, end);
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
    Array<Arg, size_>::Array(Array<Arg, size_> &&other) noexcept {
        Deallocate_n(val_begin);
        val_begin = other.val_begin;
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
    Array<Arg, size_> &Array<Arg, size_>::operator=(Array<Arg, size_> &&other) noexcept {
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

//----------------------------------------------------------------------------------------------------------------------

}

#endif //TINYSTL_ARRAY_HPP
