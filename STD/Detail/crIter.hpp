//
// Created by 86152 on 2023/9/28.
//

#ifndef TINYSTL_CRITER_HPP
#define TINYSTL_CRITER_HPP

#include "../Config.hpp"
#include "Iter.hpp"
#include "../Type_traits.hpp"

namespace STD {

    template<typename Iterator, typename Iterator_tag>
    struct crIterator {
        using Basic = Iterator;
        using Iterator_category = Iterator_tag;
        using Value_type = typename Add_const<typename Iterator_traits<Basic>::Value_type>::Type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = Value_type *;
        using Reference = Value_type &;
        using Self = crIterator<Iterator, Iterator_category>;

        Basic target;

        crIterator() = default;

        explicit crIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        crIterator &operator++() {
            --target;
            return *this;
        };

        crIterator operator++(int) {
            crIterator temp = Self(*this);
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
    struct crIterator<Iterator, Bidirectional_iterator_tag> {
    public:
        using Basic = Iterator;
        using Iterator_category = Bidirectional_iterator_tag;
        using Value_type = typename Add_const<typename Iterator_traits<Basic>::Value_type>::Type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = Value_type *;
        using Reference = Value_type &;
        using Self = crIterator<Iterator, Iterator_category>;

        Basic target;

        crIterator() = default;

        explicit crIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        crIterator &operator++() {
            --target;
            return *this;
        };

        crIterator operator++(int) {
            crIterator temp = Self(*this);
            --target;
            return temp;
        };

        crIterator &operator--() {
            ++target;
            return *this;
        };

        crIterator operator--(int) {
            crIterator temp = Self(*this);
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
    struct crIterator<Iterator, Random_iterator_tag> {
        using Basic = Iterator;
        using Iterator_category = Random_iterator_tag;
        using Value_type = typename Add_const<typename Iterator_traits<Basic>::Value_type>::Type;
        using Difference_type = typename Iterator_traits<Basic>::Difference_type;
        using Pointer = Value_type *;
        using Reference = Value_type &;
        using Self = crIterator<Iterator, Iterator_category>;

        Basic target;

        crIterator() = default;

        explicit crIterator(const Basic &target) : target(target) {};

        Reference operator*() const {
            return *target;
        };

        Pointer operator->() const {
            return target.operator->();
        };

        Reference operator[](Signed_Size size) const {
            return target[-size];
        }

        crIterator &operator++() {
            --target;
            return *this;
        };

        crIterator operator++(int) {
            crIterator temp = Self(*this);
            --target;
            return temp;
        };

        crIterator &operator--() {
            ++target;
            return *this;
        };

        crIterator operator--(int) {
            crIterator temp = Self(*this);
            ++target;
            return temp;
        };

        crIterator &operator+=(Signed_Size size) {
            target -= size;
            return *this;
        };

        crIterator &operator-=(Signed_Size size) {
            target += size;
            return *this;
        };

        friend crIterator operator+(const Self &rIterator, Signed_Size size) {
            return Self(rIterator.target - size);
        };

        friend crIterator operator-(const Self &rIterator, Signed_Size size) {
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

#endif //TINYSTL_CRITER_HPP
