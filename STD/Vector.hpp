//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_VECTOR_HPP
#define TINYSTL_VECTOR_HPP

#include "Algorithms/source/Container_algorithms.hpp"
#include "Iterator.hpp"
#include "Allocater.hpp"

namespace STD {

    template<typename Arg>
    class Vector;

    namespace Detail {

        template<typename Arg>
        struct Vector_Iterator : public Iterator<Random_iterator_tag, Arg> {
        public:
            using Basic = Iterator<Random_iterator_tag, Arg>;

            using Self = Vector_Iterator<Arg>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            friend class Vector<Arg>;

            friend class rIterator<Self, Random_iterator_tag>;

            friend class cIterator<Self, Random_iterator_tag>;

            friend class crIterator<Self, Random_iterator_tag>;

        private:
            Arg *target;

            explicit Vector_Iterator(Arg *target) : target(target) {};

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

    template<typename Arg>
    class Vector {
    public:
        using Iterator = Detail::Vector_Iterator<Arg>;

        using cIterator = STD::cIterator<Iterator, Random_iterator_tag>;

        using rIterator = STD::rIterator<Iterator, Random_iterator_tag>;

        using crIterator = STD::crIterator<Iterator, Random_iterator_tag>;

        Vector() = default;

        explicit Vector(Size size, Arg target = Arg());

        //编译器给这个类开后门了，不用它没办法实现相同的效果。
        Vector(const std::initializer_list<Arg> &list);

        //当Arg的类型是数字时，编译器一定会调用这个函数而不是Vector(Size, Arg)，所以需要另作处理。
        template<typename Input_iterator>
        Vector(const Input_iterator &begin, const Input_iterator &end);

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

        Vector<Arg> &assign(Size size, const Arg &val);

        //当Arg的类型是数字时，编译器一定会调用这个函数而不是assign(Size, Arg)，所以需要另作处理。
        template<typename Input_iterator>
        Vector<Arg> &assign(const Input_iterator &begin, const Input_iterator &end);

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(const Arg &val);

        void push_back(Arg &&val);

        void push_back(Size size, const Arg &val);

        template<typename Input_iterator>
        void push_back(const Input_iterator &begin, const Input_iterator &end);

        void pop_back();

        template<typename ...args>
        Iterator emplace(Size pos, args &&...vals);

        template<typename ...args>
        Iterator emplace(const Iterator &pos, args &&...vals);

        template<typename ...args>
        cIterator emplace(const cIterator &pos, args &&...vals);

        template<typename ...args>
        rIterator emplace(const rIterator &pos, args &&...vals);

        template<typename ...args>
        crIterator emplace(const crIterator &pos, args &&...vals);

        Iterator insert(Size pos, const Arg &val, Size size = 1);

        Iterator insert(Size pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Iterator insert(Size pos, const Input_iterator &begin, const Input_iterator &end);

        Iterator insert(const Iterator &pos, Size size, const Arg &val);

        Iterator insert(const Iterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Iterator insert(const Iterator &pos, const Input_iterator &begin, const Input_iterator &end);

        cIterator insert(const cIterator &pos, Size size, const Arg &val);

        cIterator insert(const cIterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        cIterator insert(const cIterator &pos, const Input_iterator &begin, const Input_iterator &end);

        rIterator insert(const rIterator &pos, Size size, const Arg &val);

        rIterator insert(const rIterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        rIterator insert(const rIterator &pos, const Input_iterator &begin, const Input_iterator &end);

        crIterator insert(const crIterator &pos, Size size, const Arg &val);

        crIterator insert(const crIterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        crIterator insert(const crIterator &pos, const Input_iterator &begin, const Input_iterator &end);

        Iterator erase(Size pos, Size size);

        Iterator erase(Size pos);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        rIterator erase(const rIterator &iter);

        crIterator erase(const crIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        rIterator erase(const rIterator &begin, const rIterator &end);

        crIterator erase(const crIterator &begin, const crIterator &end);

        Arg &operator[](Size pos) const { return *(val_begin + pos); };

        Arg &at(Size pos) const {
            if (pos >= size_)
                throw outOfRange("You provided an out-of-range subscript int the 'Vector::at' function");
            return *(val_begin + pos);
        };

        Arg &front() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'Vector::front' function");
            return *val_begin;
        };

        Arg &back() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'Vector::back' function");
            return *(val_end - 1);
        };

        bool empty() const { return !size_; };

        void swap(Vector<Arg> &other) noexcept {
            Size temp1 = size_;
            Arg *temp2 = val_begin, *temp3 = val_end, *temp4 = store_end;
            size_ = other.size_;
            val_begin = other.val_begin;
            val_end = other.val_end;
            store_end = other.store_end;
            other.size_ = temp1;
            other.val_begin = temp2;
            other.val_end = temp3;
            other.store_end = temp4;
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

        template<typename Type>
        friend inline void swap(Vector<Type> &left, Vector<Type> &right) noexcept;

        Iterator begin() const { return Iterator(val_begin); };

        Iterator end() const { return Iterator(val_end); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin)); };

        cIterator cend() const { return cIterator(Iterator(val_end)); };

        rIterator rbegin() const { return rIterator(Iterator(val_end - 1)); };

        rIterator rend() const { return rIterator(Iterator(val_begin - 1)); };

        crIterator crbegin() const { return crIterator(Iterator(val_end - 1)); };

        crIterator crend() const { return crIterator(Iterator(val_begin - 1)); };

    private:

        Size size_ = 0;

        Arg *val_begin = nullptr, *val_end = nullptr, *store_end = nullptr;

        void reallocate(Size size);

        Arg *backward(Size pos_from, Size pos_to);

        rIterator to_rIterator(const crIterator &val) {
            return rIterator(Iterator(const_cast<Arg *>(val.target.target)));
        };

        template<typename Input_iterator>
        Vector(const Input_iterator &begin, const Input_iterator &end, False_type);

        template<typename Input_iterator>
        Vector(const Input_iterator &size, const Input_iterator &target, True_type);

        template<typename Input_iterator>
        Vector<Arg> &assign_Helper(const Input_iterator &begin,
                               const Input_iterator &end, False_type);

        template<typename Input_iterator>
        Vector<Arg> &assign_Helper(const Input_iterator &size,
                               const Input_iterator &val, True_type);

        template<typename Input_iterator>
        void push_back_Helper(const Input_iterator &begin,
                                   const Input_iterator &end, False_type);

        template<typename Input_iterator>
        void push_back_Helper(const Input_iterator &size,
                                   const Input_iterator &val, True_type);

        template<typename Input_iterator>
        Iterator insert_Helper(Size pos, const Input_iterator &begin,
                               const Input_iterator &end, False_type);

        template<typename Input_iterator>
        Iterator insert_Helper(Size pos, const Input_iterator &size,
                               const Input_iterator &val, True_type);

        template<typename Input_iterator>
        rIterator reserve_insert_Helper(const rIterator &pos, const Input_iterator &begin,
                                       const Input_iterator &end, False_type);

        template<typename Input_iterator>
        rIterator reserve_insert_Helper(const rIterator &pos, const Input_iterator &size,
                               const Input_iterator &val, True_type);

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    template<typename Input_iterator>
    Vector<Arg>::Vector(const Input_iterator &begin, const Input_iterator &end, False_type) :
            size_(calculate_Length(begin, end)) {
        val_begin = Allocate_n<Arg>(size_);
        store_end = val_end = val_begin + size_;
        fill_with(val_begin, begin, end);
    }

    template<typename Arg>
    template<typename Input_iterator>
    Vector<Arg>::Vector(const Input_iterator &size, const Input_iterator &target, True_type)
            : val_begin(Allocate_n<Arg>(size + size / 5)), size_(size) {
        val_end = val_begin + size;
        store_end = val_begin + size + size / 5;
        fill_with(val_begin, target, size);
    }

    template<typename Arg>
    Vector<Arg>::Vector(Size size, Arg target) :
            val_begin(Allocate_n<Arg>(size + size / 5)), size_(size) {
        val_end = val_begin + size;
        store_end = val_begin + size + size / 5;
        fill_with(val_begin, target, size);
    }

    template<typename Arg>
    Vector<Arg>::Vector(const std::initializer_list<Arg> &list) :
            size_(list.size()), val_begin(Allocate_n<Arg>(size_)),
            val_end(val_begin + list.size()) {
        fill_with(val_begin, list);
        store_end = val_end;
    }

    template<typename Arg>
    template<typename Input_iterator>
    Vector<Arg>::Vector(const Input_iterator &begin, const Input_iterator &end) :
            Vector(begin, end, Is_integral<Input_iterator>()) {}

    template<typename Arg>
    Vector<Arg>::Vector(const Vector<Arg> &other) :
            size_(other.size_), val_begin(Allocate_n<Arg>(other.capacity())) {
        val_end = val_begin + size_;
        store_end = val_begin + other.capacity();
        fill_with(val_begin, other.val_begin, other.val_end);
    }

    template<typename Arg>
    Vector<Arg>::Vector(Vector<Arg> &&other) noexcept :
            size_(other.size_), val_begin(other.val_begin),
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
    Arg *Vector<Arg>::backward(Size pos_from, Size pos_to) {
        Size new_size = size_ + pos_to - pos_from;
        if (new_size > capacity()) {
            auto the_new = Allocate_n<Arg>(new_size), temp = the_new;
            for (int i = 0; i < pos_from; ++i) {
                *temp = move(*val_begin);
                ++temp, ++val_begin;
            }
            temp = the_new + pos_to;
            while (val_begin < val_end) {
                *temp = *val_begin;
                ++temp, ++val_begin;
            }
            val_begin = the_new;
            store_end = val_end = temp;
        } else {
            auto temp1 = val_end - 1, temp2 = val_end - 1 + pos_to - pos_from;
            auto target_end = val_begin + pos_from;
            val_end = temp2 + 1;
            while (temp1 >= target_end) {
                *temp2 = move(*temp1);
                --temp1, --temp2;
            }
        }
        size_ += pos_to - pos_from;
        return val_begin + pos_from;
    }

    template<typename Arg>
    void Vector<Arg>::clear(bool whether_to_release) {
        if (whether_to_release) {
            Deallocate_n(val_begin);
            val_begin = val_end = store_end = nullptr;
        } else {
            Arg *temp = val_begin;
            while (temp != val_end) *(temp++).~Arg();
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
        fill_with(temp, val_begin, size_);
        Deallocate_n(val_begin);
        val_begin = temp;
        store_end = val_end = temp + size_;
    }

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::assign(Size size, const Arg &val) {
        Deallocate_n(val_begin);
        val_begin = Allocate_n<Arg>(size);
        fill_with(val_begin, val, size);
        store_end = val_end = val_begin + size;
        size_ = size;
        return *this;
    }

    template<typename Arg>
    template<typename Input_iterator>
    Vector<Arg> &
    Vector<Arg>::assign_Helper(const Input_iterator &begin,
                               const Input_iterator &end, False_type) {
        Deallocate_n(val_begin);
        size_ = calculate_Length(begin, end);
        val_begin = Allocate_n<Arg>(size_);
        fill_with(val_begin, begin, size_);
        store_end = val_end = val_begin + size_;
        return *this;
    }

    template<typename Arg>
    template<typename Input_iterator>
    Vector<Arg> &
    Vector<Arg>::assign_Helper(const Input_iterator &size,
                               const Input_iterator &val, True_type) {
        Deallocate_n(val_begin);
        val_begin = Allocate_n<Arg>(size);
        fill_with(val_begin, val, size);
        store_end = val_end = val_begin + size;
        size_ = size;
        return *this;
    }

    template<typename Arg>
    template<typename Input_iterator>
    Vector<Arg> &Vector<Arg>::assign(const Input_iterator &begin, const Input_iterator &end) {
        return assign_Helper(begin, end, Is_integral<Input_iterator>());
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
    void Vector<Arg>::push_back(const Arg &val) {
        if (capacity() <= size_)
            reallocate(size_ + 1 > size_ + size_ / 5 ? size_ + 1 : size_ + size_ / 5);
        *(val_begin + size_) = val;
        ++size_;
        ++val_end;
    }

    template<typename Arg>
    void Vector<Arg>::push_back(Arg &&val) {
        if (capacity() <= size_)
            reallocate(size_ + 1 > size_ + size_ / 5 ? size_ + 1 : size_ + size_ / 5);
        *(val_begin + size_) = move(val);
        ++size_;
        ++val_end;
    }

    template<typename Arg>
    void Vector<Arg>::push_back(Size size, const Arg &val) {
        if (capacity() - size_ < size)
            reallocate(capacity() + size);
        fill_with(val_end, val, size);
        size_ += size;
        val_end += size;
    }

    template<typename Arg>
    template<typename Input_iterator>
    void Vector<Arg>::push_back_Helper(const Input_iterator &begin,
                               const Input_iterator &end, False_type) {
        Size count = calculate_Length(begin, end);
        if (capacity() - size_ < count)
            reallocate(capacity() + count);
        fill_with(val_end, begin, end);
        size_ += count;
        val_end += count;
    }

    template<typename Arg>
    template<typename Input_iterator>
    void Vector<Arg>::push_back_Helper(const Input_iterator &size,
                               const Input_iterator &val, True_type) {
        if (capacity() - size_ < size)
            reallocate(capacity() + size);
        fill_with(val_end, val, size);
        size_ += size;
        val_end += size;
    }

    template<typename Arg>
    template<typename Input_iterator>
    void Vector<Arg>::push_back(const Input_iterator &begin, const Input_iterator &end) {
        push_back_Helper(begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    void Vector<Arg>::pop_back() {
        if (size_) {
            (--val_end)->~Arg();
            --size_;
        }
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::Iterator
    Vector<Arg>::emplace(Size pos, args &&... vals) {
        if (pos > size_)
            throw outOfRange("Your function 'Vector::emplace' was passed an out-of-range position\n");
        *backward(pos, pos + 1) = Arg(forward<args>(vals)...);
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::Iterator
    Vector<Arg>::emplace(const Iterator &pos, args &&... vals) {
        return emplace(pos.target - val_begin, forward<args>(vals)...);
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::cIterator
    Vector<Arg>::emplace(const cIterator &pos, args &&... vals) {
        return cIterator(emplace(pos.target - val_begin, forward<args>(vals)...));
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::rIterator
    Vector<Arg>::emplace(const rIterator &pos, args &&... vals) {
        return rIterator(--emplace(pos.target.target - val_begin + 1, forward<args>(vals)...));
    }

    template<typename Arg>
    template<typename... args>
    typename Vector<Arg>::crIterator
    Vector<Arg>::emplace(const crIterator &pos, args &&... vals) {
        return crIterator(--emplace(pos.target.target - val_begin + 1, forward<args>(vals)...));
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(Size pos, const Arg &val, Size size) {
        if (pos > size_)
            throw outOfRange("You passed an out-of-range value in the 'Vector::insert' function");
        if (!size) return Iterator(val_begin + pos);
        fill_with(backward(pos, pos + size), val, size);
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(Size pos, const std::initializer_list<Arg> &list) {
        if (!list.size()) return Vector<Arg>::Iterator(val_begin + pos);
        if (pos > size_)
            throw outOfRange("You passed an out-of-range value in the 'Vector::insert' function");
        fill_with(backward(pos, pos + list.size()), list);
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert_Helper(Size pos, const Input_iterator &begin, const Input_iterator &end, False_type) {
        Size size = calculate_Length(begin, end);
        fill_with(backward(pos, pos + size), begin, end);
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert_Helper(Size pos, const Input_iterator &size, const Input_iterator &val, True_type) {
        if (!size) return Iterator(val_begin + pos);
        fill_with(backward(pos, pos + size), val, size);
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(Size pos, const Input_iterator &begin, const Input_iterator &end) {
        if (pos > size_)
            throw outOfRange("You passed an out-of-range value in the 'Vector::insert' function");
        return insert_Helper(pos, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(const Iterator &pos, Size size, const Arg &val) {
        return insert(pos.target - val_begin, size, val);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(const Iterator &pos, const std::initializer_list<Arg> &list) {
        return insert(pos.target - val_begin, list);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::Iterator
    Vector<Arg>::insert(const Iterator &pos, const Input_iterator &begin, const Input_iterator &end) {
        return insert(pos.target - val_begin, begin, end);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator
    Vector<Arg>::insert(const cIterator &pos, Size size, const Arg &val) {
        return cIterator(insert(pos.target, size, val));
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator
    Vector<Arg>::insert(const cIterator &pos, const std::initializer_list<Arg> &list) {
        return cIterator(insert(pos.target, list));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::cIterator
    Vector<Arg>::insert(const cIterator &pos, const Input_iterator &begin, const Input_iterator &end) {
        return cIterator(insert(pos.target - val_begin, begin, end));
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator
    Vector<Arg>::insert(const rIterator &pos, Size size, const Arg &val) {
        if (pos.target < val_begin - 1 || pos.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        Size pos_from = pos.target.target - val_begin + 1;
        Size pos_to = pos_from + size;
        auto ptr = backward(pos_from, pos_to);
        fill_with(ptr, val, size);
        return rIterator(Iterator(ptr + size - 1));
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator
    Vector<Arg>::insert(const rIterator &pos, const std::initializer_list<Arg> &list) {
        if (!list.size()) return pos;
        if (pos.target.target < val_begin - 1 || pos.target.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        Size target_len = list.size();
        Size pos_from = pos.target.target - val_begin + 1;
        Size pos_to = pos_from + target_len;
        auto ptr = backward(pos_from, pos_to) + target_len - 1;
        rfill_with(ptr, list);
        return rIterator(Iterator(ptr));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::rIterator
    Vector<Arg>::reserve_insert_Helper(const rIterator &pos, const Input_iterator &begin,
                                       const Input_iterator &end, False_type) {
        Size target_len = calculate_Length(begin, end);
        Size pos_from = pos.target.target - val_begin + 1;
        Size pos_to = pos_from + target_len;
        auto ptr = backward(pos_from, pos_to) + target_len - 1;
        rfill_with(ptr, begin, end);
        return rIterator(Iterator(ptr));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::rIterator
    Vector<Arg>::reserve_insert_Helper(const rIterator &pos, const Input_iterator &size,
                                       const Input_iterator &val, True_type) {
        Size pos_from = pos.target.target - val_begin + 1;
        Size pos_to = pos_from + size;
        auto ptr = backward(pos_from, pos_to);
        fill_with(ptr, val, size);
        return rIterator(Iterator(ptr + size - 1));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::rIterator
    Vector<Arg>::insert(const rIterator &pos, const Input_iterator &begin, const Input_iterator &end) {
        if (pos.target.target < val_begin - 1 || pos.target.target >= val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'Vector::insert' function");
        return reserve_insert_Helper(pos, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator
    Vector<Arg>::insert(const crIterator &pos, Size size, const Arg &val) {
        return crIterator(insert(rIterator(pos.target), size, val).target);
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator
    Vector<Arg>::insert(const crIterator &pos, const std::initializer_list<Arg> &list) {
        return crIterator(insert(rIterator(pos.target), list).target);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Vector<Arg>::crIterator
    Vector<Arg>::insert(const crIterator &pos, const Input_iterator &begin, const Input_iterator &end) {
        return crIterator(insert(rIterator(pos.target), begin, end).target);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::erase(Size pos, Size size) {
        if (!size) return Iterator(val_begin + pos);
        if (pos + size > size_)
            throw outOfRange("You passed an out-of-range value in the 'Vector::erase' function");
        auto temp1 = val_begin + pos, temp2 = val_begin + pos;
        size_ -= size;
        for (int i = 0; i < size; ++i) {
            temp1->~Arg();
            ++temp1;
        }
        while (temp1 != val_end) {
            *temp2 = *temp1;
            ++temp1, ++temp2;
        }
        val_end = temp2;
        return Iterator(val_begin + pos);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::erase(Size pos) {
        return erase(pos, 1);
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator Vector<Arg>::erase(const Iterator &iter) {
        return erase(iter.target - val_begin, 1);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator Vector<Arg>::erase(const cIterator &iter) {
        return erase(iter.target - val_begin, 1);
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator Vector<Arg>::erase(const rIterator &iter) {
        return rIterator(--erase(iter.target.target - val_begin + 1, 1));
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator Vector<Arg>::erase(const crIterator &iter) {
        return crIterator(--erase(iter.target.target - val_begin + 1, 1));
    }

    template<typename Arg>
    typename Vector<Arg>::Iterator
    Vector<Arg>::erase(const Iterator &begin, const Iterator &end) {
        return erase(begin.target - val_begin, end - begin);
    }

    template<typename Arg>
    typename Vector<Arg>::cIterator
    Vector<Arg>::erase(const cIterator &begin, const cIterator &end) {
        return erase(begin.target.target - val_begin, end - begin).to_const();
    }

    template<typename Arg>
    typename Vector<Arg>::rIterator
    Vector<Arg>::erase(const rIterator &begin, const rIterator &end) {
        return rIterator(--erase(end.target.target - val_begin + 1, end - begin));
    }

    template<typename Arg>
    typename Vector<Arg>::crIterator
    Vector<Arg>::erase(const crIterator &begin, const crIterator &end) {
        return crIterator(--erase(end.target.target - val_begin + 1, end - begin));
    }

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::operator=(const Vector<Arg> &other) {
        if (this == &other) return *this;
        Deallocate_n(val_begin);
        size_ = other.size_;
        val_begin = Allocate_n<Arg>(other.capacity());
        val_end = val_begin + size_;
        store_end = val_begin + other.capacity();
        fill_with(val_begin, other.val_begin, size_);
        return *this;
    }

    template<typename Arg>
    Vector<Arg> &Vector<Arg>::operator=(Vector<Arg> &&other) noexcept {
        if (this == &other) return *this;
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
        return !(left > right);
    }

    template<typename Type>
    bool operator>(const Vector<Type> &left, const Vector<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l != left.val_end && r != right.val_end) {
            if (*l != *r) return *l > *r;
            ++l, ++r;
        }
        return r == right.val_end && l != left.val_end;
    }

    template<typename Type>
    bool operator>=(const Vector<Type> &left, const Vector<Type> &right) {
        return !(left < right);
    }

    template<typename Type>
    inline void swap(Vector<Type> &left, Vector<Type> &right) noexcept {
        left.swap(right);
    }

//----------------------------------------------------------------------------------------------------------------------

}

#endif //TINYSTL_VECTOR_HPP
