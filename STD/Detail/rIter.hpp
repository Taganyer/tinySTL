//
// Created by 86152 on 2023/9/26.
//

#ifndef TINYSTL_RITER_HPP
#define TINYSTL_RITER_HPP

#include "../Config.hpp"
#include "Iter.hpp"

namespace STD {

    template<typename Iterator, typename Iterator_tag>
    struct rIterator {
        using Basic = Iterator;
        using Iterator_category = Iterator_tag;
        using Value_type = typename Iterator_traits<Basic>::Value_type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = typename Iterator_traits<Basic>::Pointer;
        using Reference = typename Iterator_traits<Basic>::Reference;
        using Self = rIterator<Iterator, Iterator_category>;

        Basic target;

        rIterator() = default;

        explicit rIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        rIterator &operator++() {
            --target;
            return *this;
        };

        rIterator operator++(int) {
            rIterator temp = Self(*this);
            --target;
            return temp;
        };

        friend bool operator==(const Self &left, const Self &right) {
            return left.target == right.target;
        };

        friend bool operator!=(const Self &left, const Self &right) {
            return left.target != right.target;
        };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Iterator>
    struct rIterator<Iterator, Bidirectional_iterator_tag> {
    public:
        using Basic = Iterator;
        using Iterator_category = Bidirectional_iterator_tag;
        using Value_type = typename Iterator_traits<Basic>::Value_type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = typename Iterator_traits<Basic>::Pointer;
        using Reference = typename Iterator_traits<Basic>::Reference;
        using Self = rIterator<Iterator, Iterator_category>;

        Basic target;

        rIterator() = default;

        explicit rIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        rIterator &operator++() {
            --target;
            return *this;
        };

        rIterator operator++(int) {
            rIterator temp = Self(*this);
            --target;
            return temp;
        };

        rIterator &operator--() {
            ++target;
            return *this;
        };

        rIterator operator--(int) {
            rIterator temp = Self(*this);
            ++target;
            return temp;
        };

        friend bool operator==(const Self &left, const Self &right) {
            return left.target == right.target;
        };

        friend bool operator!=(const Self &left, const Self &right) {
            return left.target != right.target;
        };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Iterator>
    struct rIterator<Iterator, Random_iterator_tag> {
        using Basic = Iterator;
        using Iterator_category = Random_iterator_tag;
        using Value_type = typename Iterator_traits<Basic>::Value_type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = typename Iterator_traits<Basic>::Pointer;
        using Reference = typename Iterator_traits<Basic>::Reference;
        using Self = rIterator<Iterator, Iterator_category>;

        Basic target;

        rIterator() = default;

        explicit rIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        Reference operator[](Step size) const {
            return target[-size];
        }

        rIterator &operator++() {
            --target;
            return *this;
        };

        rIterator operator++(int) {
            rIterator temp = Self(*this);
            --target;
            return temp;
        };

        rIterator &operator--() {
            ++target;
            return *this;
        };

        rIterator operator--(int) {
            rIterator temp = Self(*this);
            ++target;
            return temp;
        };

        rIterator &operator+=(Step size) {
            target -= size;
            return *this;
        };

        rIterator &operator-=(Step size) {
            target += size;
            return *this;
        };

        friend rIterator operator+(const Self &rIterator, Step size) {
            return Self(rIterator.target - size);
        };

        friend rIterator operator-(const Self &rIterator, Step size) {
            return Self(rIterator.target + size);
        };

        friend Difference operator-(const Self &left, const Self &right) {
            return right.target - left.target;
        };

        friend bool operator==(const Self &left, const Self &right) {
            return left.target == right.target;
        };

        friend bool operator!=(const Self &left, const Self &right) {
            return left.target != right.target;
        };

        friend bool operator<(const Self &left, const Self &right) {
            return left.target > right.target;
        };

        friend bool operator<=(const Self &left, const Self &right) {
            return left.target >= right.target;
        };

        friend bool operator>(const Self &left, const Self &right) {
            return left.target < right.target;
        };

        friend bool operator>=(const Self &left, const Self &right) {
            return left.target <= right.target;
        };

    };


}

#endif //TINYSTL_RITER_HPP
