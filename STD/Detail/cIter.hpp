//
// Created by 86152 on 2023/9/28.
//

#ifndef TINYSTL_CITER_HPP
#define TINYSTL_CITER_HPP

#include "../Config.hpp"
#include "Iter.hpp"
#include "../Type_traits.hpp"

namespace STD {

    template<typename Iterator, typename Iterator_tag>
    struct cIterator {
        using Basic = Iterator;
        using Iterator_category = Iterator_tag;
        using Value_type = typename Add_const<typename Iterator_traits<Basic>::Value_type>::Type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = Value_type *;
        using Reference = Value_type &;
        using Self = cIterator<Iterator, Iterator_category>;

        Basic target;

        cIterator() = default;

        explicit cIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        cIterator &operator++() {
            ++target;
            return *this;
        };

        cIterator operator++(int) {
            cIterator temp = Self(*this);
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
    struct cIterator<Iterator, Bidirectional_iterator_tag> {
    public:
        using Basic = Iterator;
        using Iterator_category = Bidirectional_iterator_tag;
        using Value_type = typename Add_const<typename Iterator_traits<Basic>::Value_type>::Type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = Value_type *;
        using Reference = Value_type &;
        using Self = cIterator<Iterator, Iterator_category>;

        Basic target;

        cIterator() = default;

        explicit cIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        cIterator &operator++() {
            ++target;
            return *this;
        };

        cIterator operator++(int) {
            cIterator temp = Self(*this);
            ++target;
            return temp;
        };

        cIterator &operator--() {
            --target;
            return *this;
        };

        cIterator operator--(int) {
            cIterator temp = Self(*this);
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
    struct cIterator<Iterator, Random_iterator_tag> {
        using Basic = Iterator;
        using Iterator_category = Random_iterator_tag;
        using Value_type = typename Add_const<typename Iterator_traits<Basic>::Value_type>::Type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = Value_type *;
        using Reference = Value_type &;
        using Self = cIterator<Iterator, Iterator_category>;

        Basic target;

        cIterator() = default;

        explicit cIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        Reference operator[](Step size) const {
            return target[size];
        }

        cIterator &operator++() {
            ++target;
            return *this;
        };

        cIterator operator++(int) {
            cIterator temp = Self(*this);
            ++target;
            return temp;
        };

        cIterator &operator--() {
            --target;
            return *this;
        };

        cIterator operator--(int) {
            cIterator temp = Self(*this);
            --target;
            return temp;
        };

        cIterator &operator+=(Step size) {
            target += size;
            return *this;
        };

        cIterator &operator-=(Step size) {
            target -= size;
            return *this;
        };

        friend cIterator operator+(const Self &rIterator, Step size) {
            return Self(rIterator.target + size);
        };

        friend cIterator operator-(const Self &rIterator, Step size) {
            return Self(rIterator.target - size);
        };

        friend Difference operator-(const Self &left, const Self &right) {
            return left.target - right.target;
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

#endif //TINYSTL_CITER_HPP
