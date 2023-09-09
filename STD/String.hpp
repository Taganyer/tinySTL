//
// Created by 86152 on 2023/9/9.
//

#ifndef TINYSTL_STRING_HPP
#define TINYSTL_STRING_HPP

#include "Iter.hpp"
#include "Warning.hpp"
#include "Memory.hpp"
#include "Memory.hpp"
#include "Allocater.hpp"

namespace STD {


    class String {
    private:

        Size size_ = 0;

        char *val_begin = nullptr, *val_end = nullptr, *store_end = nullptr;

        void reallocate(Size size);

    public:

        class Iterator;

        class cIterator;

        class rIterator;

        class crIterator;

        String() = default;

        String(Size size, char target);

        String(const Iter<char> &begin, const Iter<char> &end);

        String(const cIter<char> &begin, const cIter<char> &end);

        String(const String &other);

        String(String &&other) noexcept;

        ~String() { Deallocate_n(val_begin); };

        Size size() const { return size_; };

        Size length() const { return size_; };

        Size capacity() const {
            if (!store_end) return 0;
            return store_end - val_begin;
        };

        const char *c_str() const { return val_begin; };

        void assign(const Iter<char> &begin, const Iter<char> &end);

        void assign(const cIter<char> &begin, const cIter<char> &end);

        void append(char t);

        void append(const char *target);

        void append(const String &target);

        void append(const Iter<char> &begin, const Iter<char> &end);

        void append(const cIter<char> &begin, const cIter<char> &end);

        void push_back(char t);

        void push_back(const char *target);

        void push_back(const String &target);

        void push_back(const Iter<char> &begin, const Iter<char> &end);

        void push_back(const cIter<char> &begin, const cIter<char> &end);

        void pop_back();

        Iterator insert(Size pos, const char &t);

        Iterator insert(Size pos, const Iter<char> &begin, const Iter<char> &end);

        cIterator insert(Size pos, const cIter<char> &begin, const cIter<char> &end);

        Iterator erase(Size pos, bool until_end = false);

        Iterator erase(const Iterator &begin);

        cIterator erase(const cIterator &begin);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        char &operator[](Size pos) const { return *(val_begin + pos); };

        char &at(Size pos) const {
            if (pos >= size_) throw outOfRange("You provided an out-of-range subscript");
            return *(val_begin + pos);
        };

        char &front() {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'front()' function");
            return *val_begin;
        };

        char &back() {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'back()' function");
            return *(val_end - 1);
        };

        String &operator=(const String &other);

        String &operator=(String &&other) noexcept;

        String operator+(const String &other) const;

        String &operator+=(const String &other);

        String &operator+=(String &&other) noexcept;

        friend std::ostream &operator<<(std::ostream &out, const String &target);

        friend std::istream &operator>>(std::istream &in, String &target);

        friend bool operator==(const String &left, const String &right);

        friend bool operator!=(const String &left, const String &right);

        friend bool operator<(const String &left, const String &right);

        friend bool operator<=(const String &left, const String &right);

        friend bool operator>(const String &left, const String &right);

        friend bool operator>=(const String &left, const String &right);

//        Iterator begin() { return String::Iterator(val_begin); };
//
//        Iterator end() { return String::Iterator(val_end); };
//
//        cIterator cbegin() { return String::cIterator(val_begin); };
//
//        cIterator cend() { return String::cIterator(val_end); };
//
//        rIterator rbegin() { return String::rIterator(val_end - 1); };
//
//        rIterator rend() { return String::rIterator(val_begin - 1); };
//
//        crIterator crbegin() { return String::crIterator(val_end - 1); };
//
//        crIterator crend() { return String::crIterator(val_begin - 1); }

    };


}

#endif //TINYSTL_STRING_HPP
