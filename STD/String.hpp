//
// Created by 86152 on 2023/9/9.
//

#ifndef TINYSTL_STRING_HPP
#define TINYSTL_STRING_HPP

#include "Algorithms/String_Algorithms1.hpp"
#include "Iterator.hpp"
#include <iostream>

namespace STD {

    class String;

    namespace Detail {

        struct String_Iterator : public Iterator<Random_iterator_tag, char> {
            using Basic = Iterator<Random_iterator_tag, char>;

            using Self = String_Iterator;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            friend class STD::String;

            friend class rIterator<Self, Random_iterator_tag>;

            friend class cIterator<Self, Random_iterator_tag>;

            friend class crIterator<Self, Random_iterator_tag>;

        private:
            char *target;

            explicit String_Iterator(char *target) : target(target) {};

        public:
            Reference operator*() const {
                return *target;
            };

            Pointer operator->() const {
                return target;
            };

            Reference operator[](Signed_Size size) const {
                return target[size];
            }

            Self &operator++() {
                ++target;
                return *this;
            }

            Self operator++(int) {
                auto temp = Self(target++);
                return temp;
            };

            Self &operator--() {
                --target;
                return *this;
            };

            Self operator--(int) {
                auto temp = Self(target--);
                return temp;
            };

            Self &operator+=(Signed_Size size) {
                target += size;
                return *this;
            };

            Self &operator-=(Signed_Size size) {
                target -= size;
                return *this;
            };

            friend Self operator+(const Self &Iterator, Signed_Size size) {
                return Self(Iterator.target + size);
            };

            friend Self operator-(const Self &Iterator, Signed_Size size) {
                return Self(Iterator.target - size);
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

    class String {
    public:
        static const Size Npos = -1;

        using Iterator = Detail::String_Iterator;

        using cIterator = STD::cIterator<Iterator, Random_iterator_tag>;

        using rIterator = STD::rIterator<Iterator, Random_iterator_tag>;

        using crIterator = STD::crIterator<Iterator, Random_iterator_tag>;

    private:
        Size size_ = 0;

        char *val_begin = nullptr, *val_end = nullptr, *store_end = nullptr;

        void reallocate(Size size);

        void forward(Size pos_from, Size pos_to);

        char *backward(Size pos_from, Size pos_to);

    public:
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

        template<typename Input_iterator>
        String(const Input_iterator &begin, const Input_iterator &end);

        ~String() { Deallocate_n(val_begin); };

        Size size() const { return size_; };

        Size length() const { return size_; };

        Size capacity() const {
            if (!store_end)
                return 0;
            return store_end - val_begin;
        };

        const char *c_str() const { return val_begin; };

        String sub_str(Size begin, Size len) const;

        template<typename Input_iterator>
        String &assign(const Input_iterator &begin, const Input_iterator &end);

        void clear(bool whether_to_release = false);

        void append(char t);

        void append(Size len, char t);

        void append(const char *target);

        void append(const char *target, Size len);

        void append(const char *target, Size pos, Size len);

        void append(const String &target, Size pos = 0, Size len = Npos);

        template<typename Input_iterator>
        void append(const Input_iterator &begin, const Input_iterator &end);

        void push_back(char t);

        void push_back(Size len, char t);

        void push_back(const char *target);

        void push_back(const char *target, Size len);

        void push_back(const char *target, Size pos, Size len);

        void push_back(const String &target, Size pos = 0, Size len = Npos);

        template<typename Input_iterator>
        void push_back(const Input_iterator &begin, const Input_iterator &end);

        void pop_back() {
            if (size_)
                *--val_end = '\0';
        };

        Iterator insert(Size pos, char t);

        Iterator insert(Size pos, Size size, char t);

        Iterator insert(Size pos, const std::initializer_list<char> &list);

        Iterator insert(Size pos, const char *target);

        Iterator insert(Size pos, const char *target, Size target_len);

        Iterator insert(Size pos, const char *target, Size target_pos, Size target_len);

        Iterator insert(Size pos, const String &target, Size target_pos = 0, Size target_len = Npos);

        template<typename Input_iterator>
        Iterator insert(Size pos, const Input_iterator &begin, const Input_iterator &end);

        Iterator insert(const Iterator &iter, char t);

        Iterator insert(const Iterator &iter, Size size, char t);

        Iterator insert(const Iterator &iter, const std::initializer_list<char> &list);

        Iterator insert(const Iterator &iter, const char *target);

        Iterator insert(const Iterator &iter, const char *target, Size target_len);

        Iterator insert(const Iterator &iter, const char *target, Size target_pos, Size target_len);

        Iterator insert(const Iterator &iter, const String &target,
                        Size target_pos = 0, Size target_len = Npos);

        template<typename Input_iterator>
        Iterator insert(const Iterator &iter, const Input_iterator &begin, const Input_iterator &end);

        cIterator insert(const cIterator &iter, char t);

        cIterator insert(const cIterator &iter, Size size, char t);

        cIterator insert(const cIterator &iter, const std::initializer_list<char> &list);

        cIterator insert(const cIterator &iter, const char *target);

        cIterator insert(const cIterator &iter, const char *target, Size target_len);

        cIterator insert(const cIterator &iter, const char *target, Size target_pos, Size target_len);

        cIterator insert(const cIterator &iter, const String &target,
                         Size target_pos = 0, Size target_len = Npos);

        template<typename Input_iterator>
        cIterator insert(const cIterator &iter, const Input_iterator &begin, const Input_iterator &end);

        rIterator insert(const rIterator &iter, char t);

        rIterator insert(const rIterator &iter, Size size, char t);

        rIterator insert(const rIterator &iter, const std::initializer_list<char> &list);

        rIterator insert(const rIterator &iter, const char *target);

        rIterator insert(const rIterator &iter, const char *target, Size target_len);

        rIterator insert(const rIterator &iter, const char *target, Size target_pos, Size target_len);

        rIterator insert(const rIterator &iter, const String &target,
                         Size target_pos = 0, Size target_len = Npos);

        template<typename Input_iterator>
        rIterator insert(const rIterator &iter, const Input_iterator &begin, const Input_iterator &end);

        crIterator insert(const crIterator &iter, char t);

        crIterator insert(const crIterator &iter, Size size, char t);

        crIterator insert(const crIterator &iter, const std::initializer_list<char> &list);

        crIterator insert(const crIterator &iter, const char *target);

        crIterator insert(const crIterator &iter, const char *target, Size target_len);

        crIterator insert(const crIterator &iter, const char *target,
                          Size target_pos, Size target_len);

        crIterator insert(const crIterator &iter, const String &target,
                          Size target_pos = 0, Size target_len = Npos);

        template<typename Input_iterator>
        crIterator insert(const crIterator &iter, const Input_iterator &begin, const Input_iterator &end);

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

        template<typename Input_iterator>
        String &replace(Size pos, Size len, const Input_iterator &begin, const Input_iterator &end);

        String &replace(Size pos, Size len, const String &target, Size target_pos, Size target_len);

        String &replace(const Iterator &begin, const Iterator &end, Size n, char t);

        String &replace(const Iterator &begin, const Iterator &end,
                        const std::initializer_list<char> &list);

        String &replace(const Iterator &begin, const Iterator &end,
                        const char *target, Size target_len);

        String &replace(const Iterator &begin, const Iterator &end, const char *target);

        String &replace(const Iterator &begin, const Iterator &end, const String &target);

        String &replace(const Iterator &begin, const Iterator &end,
                        const String &target, Size target_pos, Size target_len);

        template<typename Input_iterator>
        String &replace(const Iterator &begin, const Iterator &end,
                        const Input_iterator &target_begin, const Input_iterator &target_end);

        String &replace(const cIterator &begin, const cIterator &end, Size n, char t);

        String &replace(const cIterator &begin, const cIterator &end,
                        const std::initializer_list<char> &list);

        String &replace(const cIterator &begin, const cIterator &end, const char *target);

        String &replace(const cIterator &begin, const cIterator &end,
                        const char *target, Size target_len);

        String &replace(const cIterator &begin, const cIterator &end, const String &target);

        String &replace(const cIterator &begin, const cIterator &end,
                        const String &target, Size target_pos, Size target_len);

        template<typename Input_iterator>
        String &replace(const cIterator &begin, const cIterator &end,
                        const Input_iterator &target_begin, const Input_iterator &target_end);

        String &replace(const rIterator &begin, const rIterator &end, Size n, char t);

        String &replace(const rIterator &begin, const rIterator &end,
                        const std::initializer_list<char> &list);

        String &replace(const rIterator &begin, const rIterator &end,
                        const char *target, Size target_len);

        String &replace(const rIterator &begin, const rIterator &end, const char *target);

        String &replace(const rIterator &begin, const rIterator &end, const String &target);

        String &replace(const rIterator &begin, const rIterator &end,
                        const String &target, Size target_pos, Size target_len);

        template<typename Input_iterator>
        String &replace(const rIterator &begin, const rIterator &end,
                        const Input_iterator &target_begin, const Input_iterator &target_end);

        String &replace(const crIterator &begin, const crIterator &end, Size n, char t);

        String &replace(const crIterator &begin, const crIterator &end,
                        const std::initializer_list<char> &list);

        String &replace(const crIterator &begin, const crIterator &end,
                        const char *target, Size target_len);

        String &replace(const crIterator &begin, const crIterator &end, const char *target);

        String &replace(const crIterator &begin, const crIterator &end, const String &target);

        String &replace(const crIterator &begin, const crIterator &end,
                        const String &target, Size target_pos, Size target_len);

        template<typename Input_iterator>
        String &replace(const crIterator &begin, const crIterator &end,
                        const Input_iterator &target_begin, const Input_iterator &target_end);

        char &operator[](Size pos) const { return val_begin[pos]; };

        char &at(Size pos) const {
            if (pos >= size_)
                throw outOfRange("You provided an out-of-range subscript int the 'String::at' function");
            return val_begin[pos];
        };

        char &front() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'String::front' function");
            return *val_begin;
        };

        char &back() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'String::back' function");
            return val_end[-1];
        };

        bool empty() const { return !size_; };

        void swap(String &other) noexcept {
            Size temp1 = size_;
            char *temp2 = val_begin, *temp3 = val_end, *temp4 = store_end;
            size_ = other.size_;
            val_begin = other.val_begin;
            val_end = other.val_end;
            store_end = other.store_end;
            other.size_ = temp1;
            other.val_begin = temp2;
            other.val_end = temp3;
            other.store_end = temp4;
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

        int compare(Size pos1, Size n1, const String &target, Size pos2,
                    Size n2) const;

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

        friend void swap(String &left, String &right) noexcept {
            left.swap(right);
        };

        Iterator begin() const { return Iterator(val_begin); };

        Iterator end() const { return Iterator(val_end); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin)); };

        cIterator cend() const { return cIterator(Iterator(val_end)); };

        rIterator rbegin() const { return rIterator(Iterator(val_end - 1)); };

        rIterator rend() const { return rIterator(Iterator(val_begin - 1)); };

        crIterator crbegin() const { return crIterator(Iterator(val_end - 1)); };

        crIterator crend() const { return crIterator(Iterator(val_begin - 1)); };

    };


//----------------------------------------------------------------------------------------------------------------------

    template<typename Input_iterator>
    String::String(const Input_iterator &begin, const Input_iterator &end)
            : size_(Detail::Get_Size(begin, end)) {
        val_begin = Allocate_n<char>(size_ + 1);
        store_end = val_begin + size_ + 1;
        val_end = val_begin + size_;
        Fill_With(val_begin, begin, end);
        *val_end = '\0';
    }

    template<typename Input_iterator>
    String &String::assign(const Input_iterator &begin, const Input_iterator &end) {
        Deallocate_n(val_begin);
        size_ = Detail::Get_Size(begin, end);
        val_begin = Allocate_n<char>(size_ + 1);
        val_end = val_begin + size_;
        store_end = val_end + 1;
        Fill_With(val_begin, begin, end);
        *val_end = '\0';
        return *this;
    }

    template<typename Input_iterator>
    void String::append(const Input_iterator &begin, const Input_iterator &end) {
        Size count = Detail::Get_Size(begin, end);
        if (capacity() - size_ - 1 < count)
            reallocate(size_ + count + 1 + (size_ + count) / 5);
        Fill_With(val_end, begin, end);
        size_ += count;
        val_end += count;
        *val_end = '\0';
    }

    template<typename Input_iterator>
    void String::push_back(const Input_iterator &begin, const Input_iterator &end) {
        append(begin, end);
    }

    template<typename Input_iterator>
    String::Iterator
    String::insert(Size pos, const Input_iterator &begin, const Input_iterator &end) {
        if (begin == end)
            return Iterator(val_begin + pos);
        if (pos > size_)
            throw outOfRange("You passed an out-of-range basic in the 'String::insert' function");
        Size target_len = Detail::Get_Size(begin, end);
        Fill_With(backward(pos, pos + target_len), begin, end);
        return Iterator(val_begin + pos);
    }

    template<typename Input_iterator>
    String::Iterator
    String::insert(const Iterator &iter, const Input_iterator &begin,
                   const Input_iterator &end) {
        return insert(iter.target - val_begin, begin, end);
    }

    template<typename Input_iterator>
    String::cIterator
    String::insert(const cIterator &iter, const Input_iterator &begin,
                   const Input_iterator &end) {
        return cIterator(insert(iter.target.target - val_begin, begin, end));
    }

    template<typename Input_iterator>
    String::rIterator
    String::insert(const rIterator &iter, const Input_iterator &begin,
                   const Input_iterator &end) {
        Size size = Detail::Get_Size(begin, end);
        if (!size) return iter;
        if (iter.target.target >= val_end || iter.target.target < val_begin - 1)
            throw outOfRange("You passed an out-of-range basic in the 'String::insert' function");
        Size pos_from = iter.target.target - val_begin + 1;
        auto ptr = backward(pos_from, pos_from + size) + size - 1;
        rFill_With(ptr, begin, end);
        return rIterator(Iterator(ptr));
    }

    template<typename Input_iterator>
    String::crIterator
    String::insert(const crIterator &iter, const Input_iterator &begin,
                   const Input_iterator &end) {
        return crIterator(insert(rIterator(iter.target), begin, end).target);
    }

    template<typename Input_iterator>
    String &String::replace(Size pos, Size len, const Input_iterator &begin, const Input_iterator &end) {
        if (pos + len > size_)
            throw outOfRange("You selected an out-of-range basic in the 'String::replace' function");
        Size target_len = Detail::Get_Size(begin, end);
        if (len < target_len)
            backward(pos + len, pos + target_len);
        else if (len > target_len)
            forward(pos + len, pos + target_len);
        Fill_With(val_begin + pos, begin, end);
        return *this;
    }

    template<typename Input_iterator>
    String &String::replace(const Iterator &begin, const Iterator &end,
                            const Input_iterator &target_begin, const Input_iterator &target_end) {
        return replace(begin.target - val_begin, end - begin, target_begin, target_end);
    }

    template<typename Input_iterator>
    String &String::replace(const cIterator &begin, const cIterator &end,
                            const Input_iterator &target_begin, const Input_iterator &target_end) {
        return replace(begin.target.target - val_begin, end - begin, target_begin, target_end);
    }

    template<typename Input_iterator>
    String &String::replace(const rIterator &begin, const rIterator &end,
                            const Input_iterator &target_begin, const Input_iterator &target_end) {
        if (begin.target.target >= val_end || begin.target.target < val_begin ||
            end.target.target < val_begin - 1 || end.target.target >= val_end ||
            begin.target < end.target)
            throw outOfRange("You selected an out-of-range basic in the 'String::replace' function");
        Size len = end - begin;
        Size target_len = Detail::Get_Size(target_begin, target_end);
        if (len < target_len) {
            Size pos_from = end.target.target - val_begin + len + 1;
            Size pos_to = pos_from + target_len - len;
            backward(pos_from, pos_to);
            rFill_With(val_begin + pos_to - 1, target_begin, target_end);
        } else if (len > target_len) {
            Size pos_from = begin.target.target - val_begin + 1;
            Size pos_to = pos_from + target_len - len;
            forward(pos_from, pos_to);
            rFill_With(val_begin + pos_to - 1, target_begin, target_end);
        } else
            rFill_With(begin.target.target, target_begin, target_end);
        return *this;
    }

    template<typename Input_iterator>
    String &String::replace(const crIterator &begin, const crIterator &end,
                            const Input_iterator &target_begin, const Input_iterator &target_end) {
        return replace(rIterator(begin.target), rIterator(end.target), target_begin, target_end);
    }

//----------------------------------------------------------------------------------------------------------------------

    String operator+(const String &left, const String &right);

    String operator*(const String &target, int size);

    std::ostream &operator<<(std::ostream &out, const String &target);

    std::istream &operator>>(std::istream &in, String &target);

    bool operator==(const String &left, const String &right);

    bool operator!=(const String &left, const String &right);

    bool operator<(const String &left, const String &right);

    bool operator<=(const String &left, const String &right);

    bool operator>(const String &left, const String &right);

    bool operator>=(const String &left, const String &right);

} // namespace STD

#endif // TINYSTL_STRING_HPP
