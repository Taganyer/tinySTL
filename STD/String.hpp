//
// Created by 86152 on 2023/9/9.
//

#ifndef TINYSTL_STRING_HPP
#define TINYSTL_STRING_HPP

#include "Iter.hpp"
#include "Warning.hpp"
#include "Memory.hpp"
#include "Allocater.hpp"
#include "Algorithms.hpp"

namespace STD {


    class String {
    private:

        Size size_ = 0;

        char *val_begin = nullptr, *val_end = nullptr, *store_end = nullptr;

        void reallocate(Size size);

        void forward(Size pos_from, Size pos_to);

        char *backward(Size pos_from, Size pos_to);

        static void fill_with(char *pos, char target, Size size);

        static void fill_with(char *pos, const char *target, Size size);

        static void fill_with(char *pos, const std::initializer_list<char> &list);

        static void fill_with(char *pos, const cIter<char> &begin, Size size);

        static void rfill_with(char *pos, const char *target, Size size);

        static void rfill_with(char *pos, const std::initializer_list<char> &list);

        static void rfill_with(char *pos, const cIter<char> &begin, Size size);

    public:

        static const Size Npos = -1;

        class Iterator : public Iter<char> {
        protected:
            using Iter<char>::target;

            virtual Iterator &operator=(char *ptr) {
                target = ptr;
                return *this;
            };

            explicit Iterator(char *ptr) : Iter<char>(ptr) {};

        public:
            friend class String;

            Shared_ptr<Iter<char>> deep_copy() const override { return make_shared<String::Iterator>(*this); };

            Shared_ptr<cIter<char>> to_const() const override {
                return make_shared<String::cIterator>(cIterator(target));
            };

            Iterator(const Iterator &other) : Iter<char>(other.target) {};

            ~Iterator() = default;

            //该函数如果传入rIterator作为参数可能会有意外的结果
            Iterator &operator=(const Iterator &other) {
                target = other.target;
                return *this;
            };


            using Iter<char>::operator*;

            using Iter<char>::operator->;

            Iterator &operator++() override {
                ++target;
                return *this;
            };

            Iterator operator++(int) { return String::Iterator(target++); };

            virtual Iterator &operator--() {
                --target;
                return *this;
            };

            Iterator operator--(int) { return String::Iterator(target--); };

            Iterator operator+(Size size) const { return Iterator(target + size); };

            Iterator operator-(Size size) const { return Iterator(target - size); };

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

        class cIterator : public cIter<char> {
        protected:
            using cIter<char>::target;

            virtual cIterator &operator=(char *ptr) {
                target = ptr;
                return *this;
            };

            explicit cIterator(char *ptr) : cIter<char>(ptr) {};

        public:
            friend class String;

            friend class String::Iterator;

            Shared_ptr<cIter<char>> deep_copy() const override { return make_shared<String::cIterator>(*this); };

            cIterator(const cIterator &other) : cIter<char>(other.target) {};

            ~cIterator() = default;

            //该函数如果传入rIterator作为参数可能会有意外的结果
            cIterator &operator=(const cIterator &other) {
                target = other.target;
                return *this;
            };


            using cIter<char>::operator*;

            using cIter<char>::operator->;

            cIterator &operator++() override {
                ++target;
                return *this;
            };

            cIterator operator++(int) { return String::cIterator(target++); };

            virtual cIterator &operator--() {
                --target;
                return *this;
            };

            cIterator operator--(int) { return String::cIterator(target--); };

            cIterator operator+(Size size) const { return cIterator(target + size); };

            cIterator operator-(Size size) const { return cIterator(target - size); };

            cIterator &operator+=(Size size) {
                target += size;
                return *this;
            };

            cIterator &operator-=(Size size) {
                target -= size;
                return *this;
            };

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

        class rIterator : public Iter<char> {
        protected:
            using Iter<char>::target;

            rIterator &operator=(char *ptr) {
                target = ptr;
                return *this;
            }

            explicit rIterator(char *ptr) : Iter<char>(ptr) {};

        public:
            friend class String;

            Shared_ptr<Iter<char>> deep_copy() const override { return make_shared<String::rIterator>(*this); };

            Shared_ptr<cIter<char>> to_const() const override {
                return make_shared<String::crIterator>(crIterator(target));
            };

            rIterator(const rIterator &other) : Iter<char>(other.target) {};

            ~rIterator() = default;

            rIterator &operator=(const rIterator &other) {
                target = other.target;
                return *this;
            };

            using Iter<char>::operator*;

            using Iter<char>::operator->;

            rIterator &operator++() override {
                --target;
                return *this;
            };

            rIterator operator++(int) { return String::rIterator(target--); };

            rIterator &operator--() {
                ++target;
                return *this;
            };

            rIterator operator--(int) { return String::rIterator(target++); };

            rIterator operator+(Size size) const { return String::rIterator(target - size); };

            rIterator operator-(Size size) const { return String::rIterator(target + size); };

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

        class crIterator : public cIter<char> {
        protected:
            using cIter<char>::target;

            crIterator &operator=(char *ptr) {
                target = ptr;
                return *this;
            };

            explicit crIterator(char *ptr) : cIter<char>(ptr) {};

        public:
            friend class String;

            friend class String::cIterator;

            Shared_ptr<cIter<char>> deep_copy() const override { return make_shared<String::crIterator>(*this); };

            crIterator(const crIterator &other) : cIter<char>(other.target) {};

            ~crIterator() = default;

            crIterator &operator=(const crIterator &other) {
                target = other.target;
                return *this;
            };

            using cIter<char>::operator*;

            using cIter<char>::operator->;

            crIterator &operator++() override {
                --target;
                return *this;
            };

            crIterator operator++(int) { return String::crIterator(target--); };

            crIterator &operator--() {
                ++target;
                return *this;
            };

            crIterator operator--(int) { return String::crIterator(target++); };

            crIterator operator+(Size size) const { return String::crIterator(target - size); };

            crIterator operator-(Size size) const { return String::crIterator(target + size); };

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


        String();

        String(Size size, char target);

        String(const char *target, Size len);

        //编译器给这个类开后门了，不用它没办法实现相同的效果。
        String(const std::initializer_list<char> &list);

        String(const char *target);

        String(const String &other, Size pos);

        String(const String &other, Size pos, Size len);

        String(const String &other);

        String(String &&other) noexcept;

        String(const Iter<char> &begin, const Iter<char> &end);

        String(const cIter<char> &begin, const cIter<char> &end);

        ~String() {
            Deallocate_n(val_begin);
        };

        Size size() const { return size_; };

        Size length() const { return size_; };

        Size capacity() const {
            if (!store_end) return 0;
            return store_end - val_begin;
        };

        const char *c_str() const { return val_begin; };

        String sub_str(Size begin, Size len);

        String &assign(const Iter<char> &begin, const Iter<char> &end);

        String &assign(const cIter<char> &begin, const cIter<char> &end);

        void clear(bool whether_to_release = false);

        void append(char t, Size len = 1);

        void append(const char *target);

        void append(const char *target, Size len);

        void append(const char *target, Size pos, Size len);

        void append(const String &target);

        void append(const String &target, Size pos);

        void append(const String &target, Size pos, Size len);

        void append(const Iter<char> &begin, const Iter<char> &end);

        void append(const cIter<char> &begin, const cIter<char> &end);

        void push_back(char t, Size len = 1);

        void push_back(const char *target);

        void push_back(const char *target, Size len);

        void push_back(const char *target, Size pos, Size len);

        void push_back(const String &target);

        void push_back(const String &target, Size pos);

        void push_back(const String &target, Size pos, Size len);

        void push_back(const Iter<char> &begin, const Iter<char> &end);

        void push_back(const cIter<char> &begin, const cIter<char> &end);

        void pop_back() { if (size_) *--val_end = '\0'; };

        Iterator insert(Size pos, char t, Size size = 1);

        Iterator insert(Size pos, const std::initializer_list<char> &list);

        Iterator insert(Size pos, const char *target);

        Iterator insert(Size pos, const char *target, Size target_len);

        Iterator insert(Size pos, const char *target, Size target_pos, Size target_len);

        Iterator insert(Size pos, const String &target, Size target_pos = 0, Size target_len = Npos);

        Iterator insert(Size pos, const Iter<char> &begin, const Iter<char> &end);

        Iterator insert(Size pos, const cIter<char> &begin, const cIter<char> &end);

        Iterator insert(const Iterator &iter, char t, Size size = 1);

        Iterator insert(const Iterator &iter, const std::initializer_list<char> &list);

        Iterator insert(const Iterator &iter, const char *target);

        Iterator insert(const Iterator &iter, const char *target, Size target_len);

        Iterator insert(const Iterator &iter, const char *target, Size target_pos, Size target_len);

        Iterator insert(const Iterator &iter, const String &target, Size target_pos = 0, Size target_len = Npos);

        Iterator insert(const Iterator &iter, const Iter<char> &begin, const Iter<char> &end);

        Iterator insert(const Iterator &iter, const cIter<char> &begin, const cIter<char> &end);

        cIterator insert(const cIterator &iter, char t, Size size = 1);

        cIterator insert(const cIterator &iter, const std::initializer_list<char> &list);

        cIterator insert(const cIterator &iter, const char *target);

        cIterator insert(const cIterator &iter, const char *target, Size target_len);

        cIterator insert(const cIterator &iter, const char *target, Size target_pos, Size target_len);

        cIterator insert(const cIterator &iter, const String &target, Size target_pos = 0, Size target_len = Npos);

        cIterator insert(const cIterator &iter, const Iter<char> &begin, const Iter<char> &end);

        cIterator insert(const cIterator &iter, const cIter<char> &begin, const cIter<char> &end);

        rIterator insert(const rIterator &iter, char t, Size size = 1);

        rIterator insert(const rIterator &iter, const std::initializer_list<char> &list);

        rIterator insert(const rIterator &iter, const char *target);

        rIterator insert(const rIterator &iter, const char *target, Size target_len);

        rIterator insert(const rIterator &iter, const char *target, Size target_pos, Size target_len);

        rIterator insert(const rIterator &iter, const String &target, Size target_pos = 0, Size target_len = Npos);

        rIterator insert(const rIterator &iter, const Iter<char> &begin, const Iter<char> &end);

        rIterator insert(const rIterator &iter, const cIter<char> &begin, const cIter<char> &end);

        crIterator insert(const crIterator &iter, char t, Size size = 1);

        crIterator insert(const crIterator &iter, const std::initializer_list<char> &list);

        crIterator insert(const crIterator &iter, const char *target);

        crIterator insert(const crIterator &iter, const char *target, Size target_len);

        crIterator insert(const crIterator &iter, const char *target, Size target_pos, Size target_len);

        crIterator insert(const crIterator &iter, const String &target, Size target_pos = 0, Size target_len = Npos);

        crIterator insert(const crIterator &iter, const Iter<char> &begin, const Iter<char> &end);

        crIterator insert(const crIterator &iter, const cIter<char> &begin, const cIter<char> &end);

        Iterator erase(Size pos, Size size = 1);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        rIterator erase(const rIterator &iter);

        crIterator erase(const crIterator &iter);

        rIterator erase(const rIterator &begin, const rIterator &end);

        crIterator erase(const crIterator &begin, const crIterator &end);

        String &replace(Size pos, Size len, Size n, char t);

        String &replace(Size pos, Size len, const std::initializer_list<char> &list);

        String &replace(Size pos, Size len, const char *target);

        String &replace(Size pos, Size len, const char *target, Size target_len);

        String &replace(Size pos, Size len, const String &target);

        String &replace(Size pos, Size len, const Iter<char> &begin, const Iter<char> &end);

        String &replace(Size pos, Size len, const cIter<char> &begin, const cIter<char> &end);

        String &replace(Size pos, Size len, const String &target, Size target_pos, Size target_len);

        String &replace(const Iterator &begin, const Iterator &end, Size n, char t);

        String &replace(const Iterator &begin, const Iterator &end, const std::initializer_list<char> &list);

        String &replace(const Iterator &begin, const Iterator &end, const char *target, Size target_len);

        String &replace(const Iterator &begin, const Iterator &end, const char *target);

        String &replace(const Iterator &begin, const Iterator &end, const String &target);

        String &
        replace(const Iterator &begin, const Iterator &end, const String &target, Size target_pos, Size target_len);

        String &replace(const Iterator &begin, const Iterator &end, const Iter<char> &target_begin,
                        const Iter<char> &target_end);

        String &replace(const Iterator &begin, const Iterator &end, const cIter<char> &target_begin,
                        const cIter<char> &target_end);

        String &replace(const cIterator &begin, const cIterator &end, Size n, char t);

        String &replace(const cIterator &begin, const cIterator &end, const std::initializer_list<char> &list);

        String &replace(const cIterator &begin, const cIterator &end, const char *target);

        String &replace(const cIterator &begin, const cIterator &end, const char *target, Size target_len);

        String &replace(const cIterator &begin, const cIterator &end, const String &target);

        String &
        replace(const cIterator &begin, const cIterator &end, const String &target, Size target_pos, Size target_len);

        String &replace(const cIterator &begin, const cIterator &end, const Iter<char> &target_begin,
                        const Iter<char> &target_end);

        String &replace(const cIterator &begin, const cIterator &end, const cIter<char> &target_begin,
                        const cIter<char> &target_end);

        String &replace(const rIterator &begin, const rIterator &end, Size n, char t);

        String &replace(const rIterator &begin, const rIterator &end, const std::initializer_list<char> &list);

        String &replace(const rIterator &begin, const rIterator &end, const char *target, Size target_len);

        String &replace(const rIterator &begin, const rIterator &end, const char *target);

        String &replace(const rIterator &begin, const rIterator &end, const String &target);

        String &
        replace(const rIterator &begin, const rIterator &end, const String &target, Size target_pos, Size target_len);

        String &replace(const rIterator &begin, const rIterator &end, const Iter<char> &target_begin,
                        const Iter<char> &target_end);

        String &replace(const rIterator &begin, const rIterator &end, const cIter<char> &target_begin,
                        const cIter<char> &target_end);

        String &replace(const crIterator &begin, const crIterator &end, Size n, char t);

        String &replace(const crIterator &begin, const crIterator &end, const std::initializer_list<char> &list);

        String &replace(const crIterator &begin, const crIterator &end, const char *target, Size target_len);

        String &replace(const crIterator &begin, const crIterator &end, const char *target);

        String &replace(const crIterator &begin, const crIterator &end, const String &target);

        String &
        replace(const crIterator &begin, const crIterator &end, const String &target, Size target_pos, Size target_len);

        String &replace(const crIterator &begin, const crIterator &end, const Iter<char> &target_begin,
                        const Iter<char> &target_end);

        String &replace(const crIterator &begin, const crIterator &end, const cIter<char> &target_begin,
                        const cIter<char> &target_end);

        char &operator[](Size pos) const { return *(val_begin + pos); };

        char &at(Size pos) const {
            if (pos >= size_) throw outOfRange("You provided an out-of-range subscript int the 'String::at' function");
            return *(val_begin + pos);
        };

        char &front() {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'String::front' function");
            return *val_begin;
        };

        char &back() {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'String::back' function");
            return *(val_end - 1);
        };

        Size find(char t, Size pos = 0) const;

        Size find(const char *target, Size pos = 0) const;

        Size find(const char *target, Size pos, Size size) const;

        Size find(const String &target, Size pos = 0) const;

        Size find(const String &target, Size pos, Size size) const;

        Size rfind(char t, Size pos = 0) const;

        Size rfind(const char *target, Size pos = 0) const;

        Size rfind(const char *target, Size pos, Size size) const;

        Size rfind(const String &target, Size pos = 0) const;

        Size rfind(const String &target, Size pos, Size size) const;

        Size find_first_of(const char *target, Size pos = 0) const;

        Size find_first_of(const char *target, Size pos, Size size) const;

        Size find_first_of(const String &target, Size pos = 0) const;

        Size find_first_of(const String &target, Size pos, Size size) const;

        Size find_last_of(const char *target, Size pos = 0) const;

        Size find_last_of(const char *target, Size pos, Size size) const;

        Size find_last_of(const String &target, Size pos = 0) const;

        Size find_last_of(const String &target, Size pos, Size size) const;

        Size find_first_not_of(const char *target, Size pos = 0) const;

        Size find_first_not_of(const char *target, Size pos, Size size) const;

        Size find_first_not_of(const String &target, Size pos = 0) const;

        Size find_first_not_of(const String &target, Size pos, Size size) const;

        Size find_last_not_of(const char *target, Size pos = 0) const;

        Size find_last_not_of(const char *target, Size pos, Size size) const;

        Size find_last_not_of(const String &target, Size pos = 0) const;

        Size find_last_not_of(const String &target, Size pos, Size size) const;

        String &operator=(const String &other);

        String &operator=(String &&other) noexcept;

        String &operator+=(const String &other);

        String &operator*=(int size);

        int compare(const char *target) const;

        int compare(Size pos1, Size n1, const char *target) const;

        int compare(Size pos1, Size n1, const char *target, Size n2) const;

        int compare(const String &target) const;

        int compare(Size pos1, Size n1, const String &target) const;

        int compare(Size pos1, Size n1, const String &target, Size pos2, Size n2) const;

        friend String operator+(const String &left, const String &right);

        friend String operator*(const String &target, int size);

        friend std::ostream &operator<<(std::ostream &out, const String &target);

        friend std::istream &operator>>(std::istream &in, String &target);

        friend bool operator==(const String &left, const String &right);

        friend bool operator!=(const String &left, const String &right);

        friend bool operator<(const String &left, const String &right);

        friend bool operator<=(const String &left, const String &right);

        friend bool operator>(const String &left, const String &right);

        friend bool operator>=(const String &left, const String &right);

        Iterator begin() { return String::Iterator(val_begin); };

        Iterator end() { return String::Iterator(val_end); };

        cIterator cbegin() { return String::cIterator(val_begin); };

        cIterator cend() { return String::cIterator(val_end); };

        rIterator rbegin() { return String::rIterator(val_end - 1); };

        rIterator rend() { return String::rIterator(val_begin - 1); };

        crIterator crbegin() { return String::crIterator(val_end - 1); };

        crIterator crend() { return String::crIterator(val_begin - 1); }

    };


}

#endif //TINYSTL_STRING_HPP
