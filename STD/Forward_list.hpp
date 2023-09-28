//
// Created by 86152 on 2023/9/15.
//

#ifndef TINYSTL_FORWARD_LIST_HPP
#define TINYSTL_FORWARD_LIST_HPP

#include "Allocater.hpp"
#include "Iterator.hpp"
#include <initializer_list>

namespace STD {

    template<typename Arg>
    class Forward_List;

    namespace Detail {

        template<typename Arg>
        struct Forward_List_Iterator : public Iterator<Forward_iterator_tag, Arg> {
        public:
            using Basic = Iterator<Forward_iterator_tag, Arg>;

            using Self = Forward_List_Iterator<Arg>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            using Node = typename Forward_List<Arg>::Node;

            friend class Forward_List<Arg>;

            friend class rIterator<Self, Random_iterator_tag>;

            friend class cIterator<Self, Random_iterator_tag>;

            friend class crIterator<Self, Random_iterator_tag>;

        private:
            Node *target;

            explicit Forward_List_Iterator(Node *target) : target(target) {};

        public:
            Reference operator*() const {
                return *(target->value);
            };

            Pointer operator->() const {
                return target->value;
            };

            Self &operator++() {
                target = target->next;
                return *this;
            }

            Self operator++(int) {
                Self temp = Self(target);
                target = target->next;
                return temp;
            };

            friend bool operator==(const Self &left, const Self &right) {
                return left.target == right.target;
            };

            friend bool operator!=(const Self &left, const Self &right) {
                return left.target != right.target;
            };
        };

    }

    template<typename Arg>
    class Forward_list {
    public:
        friend class Detail::Forward_List_Iterator<Arg>;

        using Iterator = Detail::Forward_List_Iterator<Arg>;

        using cIterator = STD::crIterator<Iterator, Forward_iterator_tag>;

        Forward_list() = default;

        Forward_list(Size size, Arg target = Arg());

        Forward_list(const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Forward_list(Input_iterator begin, const Input_iterator &end);

        Forward_list(const Forward_list<Arg> &other);

        Forward_list(Forward_list<Arg> &&other) noexcept;

        ~Forward_list();

        Size size() const { return size_; };

        Size capacity() const { return size_; };

        void clear();

        template<typename Input_iterator>
        Forward_list<Arg> &assign(Input_iterator begin, const Input_iterator &end);

        Forward_list<Arg> &assign(const std::initializer_list<Arg> &list);

        template<typename... args>
        void emplace_front(args &&...vals);

        void push_front(const Arg &val);

        void push_front(Arg &&val);

        void push_front(const Arg &val, Size size);

        template<typename Input_iterator>
        void push_front(Input_iterator begin, const Input_iterator &end);

        void pop_front();

        template<typename... args>
        Iterator emplace_after(const Iterator &pos, args &&...vals);

        template<typename... args>
        cIterator emplace_after(const cIterator &pos, args &&...vals);

        Iterator insert_after(const Iterator &pos, const Arg &target);

        cIterator insert_after(const cIterator &pos, const Arg &target);

        Iterator insert_after(const Iterator &pos, Size size, const Arg &target);

        cIterator insert_after(const cIterator &pos, Size size, const Arg &target);

        Iterator insert_after(const Iterator &pos, const std::initializer_list<Arg> &list);

        cIterator insert_after(const cIterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Iterator insert_after(const Iterator &pos, Input_iterator begin, const Input_iterator &end);

        template<typename Input_iterator>
        cIterator insert_after(const cIterator &pos, Input_iterator begin, const Input_iterator &end);

        Iterator erase_after(const Iterator &pos);

        Iterator erase_after(const Iterator &begin, const Iterator &end);

        cIterator erase_after(const cIterator &pos);

        cIterator erase_after(const cIterator &begin, const cIterator &end);

        Arg &front() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'List::front' function");
            return val_begin->next->value;
        };

        bool empty() const { return !size_; };

        void swap(Forward_list<Arg> &other) noexcept {
            Size temp1 = size_;
            Node *temp2 = val_begin, *temp3 = val_end;
            size_ = other.size_;
            val_begin = other.val_begin;
            val_end = other.val_end;
            other.size_ = temp1;
            other.val_begin = temp2;
            other.val_end = temp3;
        }

        Forward_list<Arg> &operator=(const Forward_list<Arg> &other);

        Forward_list<Arg> &operator=(Forward_list<Arg> &&other) noexcept;

        template<typename Type>
        friend bool operator==(const Forward_list<Type> &left, const Forward_list<Type> &right);

        template<typename Type>
        friend bool operator!=(const Forward_list<Type> &left, const Forward_list<Type> &right);

        template<typename Type>
        friend bool operator<(const Forward_list<Type> &left, const Forward_list<Type> &right);

        template<typename Type>
        friend bool operator<=(const Forward_list<Type> &left, const Forward_list<Type> &right);

        template<typename Type>
        friend bool operator>(const Forward_list<Type> &left, const Forward_list<Type> &right);

        template<typename Type>
        friend bool operator>=(const Forward_list<Type> &left, const Forward_list<Type> &right);

        template<typename Type>
        friend inline void swap(Forward_list<Arg> &left, Forward_list<Arg> &right) noexcept;

        Iterator begin() const { return Iterator(val_begin->next); };

        Iterator end() const { return Iterator(val_end); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin->next)); };

        cIterator cend() const { return cIterator(Iterator(val_end)); };

        Iterator before_begin() const { return Iterator(val_begin); };

        cIterator before_cbegin() const { return cIterator(Iterator(val_begin)); };

    private:
        struct Node {
            Arg *value;

            Node *next;

            Node(const Arg &target, Node *next = nullptr) : value(Allocate(target)), next(next) {};

            Node(Arg &&target, Node *next = nullptr) : value(Allocate(move(target))), next(next) {};

            Node(Arg *target, Node *next = nullptr) : value(target), next(next) {};

            ~Node() { Deallocate(value); };
        };

        Size size_ = 0;

        Node *val_end = Allocate<Node>(nullptr, nullptr),
                *val_begin = Allocate<Node>(nullptr, val_end);

        void release_after(Node *begin, Node *end);

        Iterator insert_Helper(Node *location, Step size, Arg target);

        Iterator insert_Helper(Node *location, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Iterator insert_Helper(Node *location, Input_iterator begin,
                               const Input_iterator &end, False_type);

        template<typename Input_iterator>
        Iterator insert_Helper(Node *location, Input_iterator size,
                               const Input_iterator &target, True_type);

    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Forward_list<Arg>::Forward_list(Size size, Arg target) {
        insert_Helper(val_begin, size, target);
    }

    template<typename Arg>
    Forward_list<Arg>::Forward_list(const std::initializer_list<Arg> &list) {
        insert_Helper(val_begin, list);
    }

    template<typename Arg>
    template<typename Input_iterator>
    Forward_list<Arg>::Forward_list(Input_iterator begin, const Input_iterator &end) {
        insert_Helper(val_begin, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    Forward_list<Arg>::Forward_list(const Forward_list<Arg> &other) {
        insert_Helper(val_begin, other.begin(), other.end(), False_type());
    }

    template<typename Arg>
    Forward_list<Arg>::Forward_list(Forward_list<Arg> &&other) noexcept
            : size_(other.size_) {
        val_begin->next = other.val_begin->next;
        auto temp = other.val_begin->next;
        for (int i = 0; i < size_ - 1; ++i)
            temp = temp->next;
        temp->next = val_end;
        other.size_ = 0;
        other.val_begin->next = nullptr;
    }

    template<typename Arg>
    Forward_list<Arg>::~Forward_list() {
        clear();
        Deallocate(val_begin);
        Deallocate(val_end);
    }

    template<typename Arg>
    void Forward_list<Arg>::release_after(Node *begin, Node *end) {
        Size count = 0;
        auto temp = begin->next, t = temp;
        while (temp != end) {
            t = temp;
            temp = temp->next;
            Deallocate(t);
            ++count;
        }
        begin->next = end;
        size_ -= count;
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_Helper(Node *location, Step size, Arg target) {
        Arg *last = location, *now = location, *store = location->next;
        size_ += size;
        while (size > 0) {
            now = Allocate<Node>(target);
            last->next = now;
            last = now;
            --size;
        }
        now->next = store;
        return Iterator(now);
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_Helper(Node *location,
                                     const std::initializer_list<Arg> &list) {
        Arg *last = location, *now = location, *store = location->next;
        for (auto &target: list) {
            now = Allocate<Node>(target);
            last->next = now;
            last = now;
        }
        now->next = store;
        size_ += list.size();
        return Iterator(now);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_Helper(Node *location, Input_iterator begin,
                                     const Input_iterator &end, False_type) {
        Arg *last = location, *now = location, *store = location->next;
        while (begin != end) {
            now = Allocate<Node>(*begin);
            last->next = now;
            last = now;
            ++size_, ++begin;
        }
        now->next = store;
        return Iterator(now);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_Helper(Node *location, Input_iterator size,
                                     const Input_iterator &target, True_type) {
        return insert_Helper(location, size, target);
    }

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    void Forward_list<Arg>::clear() {
        release_after(val_begin, val_end);
    }

    template<typename Arg>
    template<typename Input_iterator>
    Forward_list<Arg> &Forward_list<Arg>::assign(Input_iterator begin, const Input_iterator &end) {
        clear();
        insert_Helper(val_begin, begin, end, Is_integral<Input_iterator>());
        return *this;
    }

    template<typename Arg>
    Forward_list<Arg> &
    Forward_list<Arg>::assign(const std::initializer_list<Arg> &list) {
        clear();
        insert_Helper(val_begin, list);
        return *this;
    }

    template<typename Arg>
    template<typename... args>
    void Forward_list<Arg>::emplace_front(args &&...vals) {
        auto temp = Allocate<Node>(Allocate<Arg>(forward<args>(vals)...), val_begin->next);
        val_begin->next = temp;
        ++size_;
    }

    template<typename Arg>
    void Forward_list<Arg>::push_front(const Arg &val) {
        auto temp = Allocate<Node>(val, val_begin->next);
        val_begin->next = temp;
        ++size_;
    }

    template<typename Arg>
    void Forward_list<Arg>::push_front(Arg &&val) {
        auto temp = Allocate<Node>(move(val), val_begin->next);
        val_begin->next = temp;
        ++size_;
    }

    template<typename Arg>
    void Forward_list<Arg>::push_front(const Arg &val, Size size) {
        insert_Helper(val_begin, size, val);
    }

    template<typename Arg>
    template<typename Input_iterator>
    void Forward_list<Arg>::push_front(Input_iterator begin, const Input_iterator &end) {
        insert_Helper(val_begin, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    void Forward_list<Arg>::pop_front() {
        if (size_) {
            auto temp = val_begin->next;
            val_begin->next = temp->next;
            Deallocate(temp);
            --size_;
        }
    }

    template<typename Arg>
    template<typename... args>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::emplace_after(const Iterator &pos, args &&...vals) {
        if (!pos.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::emplace_after' function");
        auto temp = Allocate<Node>(Allocate<Arg>(forward<args>(vals)...), pos.target->next);
        pos.target->next = temp;
        ++size_;
        return Iterator(temp);
    }

    template<typename Arg>
    template<typename... args>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::emplace_after(const cIterator &pos, args &&...vals) {
        if (!pos.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::emplace_after' function");
        auto temp = Allocate<Node>(Allocate<Arg>(forward<args>(vals)...), pos.target->next);
        pos.target->next = temp;
        ++size_;
        return cIterator(temp);
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Iterator &pos, const Arg &target) {
        if (!pos.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        auto temp = Allocate<Node>(target, pos.target->next);
        pos.target->next = temp;
        ++size_;
        return Iterator(temp);
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const cIterator &pos, const Arg &target) {
        if (!pos.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        auto temp = Allocate<Node>(target, pos.target->next);
        pos.target->next = temp;
        ++size_;
        return cIterator(Iterator(temp));
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Iterator &pos, Size size, const Arg &target) {
        if (!pos.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        return insert_Helper(pos.target, size, target);
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const cIterator &pos, Size size, const Arg &target) {
        if (!pos.target.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        return cIterator(insert_Helper(pos.target.target, size, target));
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Iterator &pos, const std::initializer_list<Arg> &list) {
        if (!pos.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        return insert_Helper(pos.target, list);
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const cIterator &pos, const std::initializer_list<Arg> &list) {
        return cIterator(insert_Helper(pos.target.target, list));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Iterator &pos, Input_iterator begin, const Input_iterator &end) {
        if (!pos.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        return insert_Helper(pos.target, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const cIterator &pos, Input_iterator begin, const Input_iterator &end) {
        if (!pos.target.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        return cIterator(insert_Helper(pos.target.target, begin, end, Is_integral<Input_iterator>()));
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::erase_after(const Iterator &pos) {
        if (!pos.target->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::erase_after' function");
        auto temp = pos.target->next;
        pos.target->next = temp->next;
        Deallocate(temp);
        --size_;
        return pos;
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::erase_after(const Iterator &begin, const Iterator &end) {
        if (!begin.target->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::erase_after' function");
        release_after(begin.target, end.target);
        return begin;
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::erase_after(const cIterator &pos) {
        if (!pos.target.target->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::erase_after' function");
        auto temp = pos.target->next;
        pos.target->next = temp->next;
        Deallocate(temp);
        --size_;
        return pos;
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::erase_after(const cIterator &begin, const cIterator &end) {
        if (!begin.target.target->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::erase_after' function");
        release_after(begin.target.target, end.target.target);
        return begin;
    }

    template<typename Arg>
    Forward_list<Arg> &
    Forward_list<Arg>::operator=(const Forward_list<Arg> &other) {
        if (this == &other) return *this;
        clear();
        insert_Helper(val_begin, other.begin(), other.end());
        return *this;
    }

    template<typename Arg>
    Forward_list<Arg> &
    Forward_list<Arg>::operator=(Forward_list<Arg> &&other) noexcept {
        if (this == &other) return *this;
        clear();
        val_begin->next = other.val_begin->next;
        size_ = other.size_;
        auto temp = other.val_begin->next;
        for (int i = 0; i < size_ - 1; ++i)
            temp = temp->next;
        temp->next = val_end;
        other.size_ = 0;
        other.val_begin->next = other.val_end;
        return *this;
    }

    template<typename Type>
    bool operator==(const Forward_list<Type> &left, const Forward_list<Type> &right) {
        if (left.size_ != right.size_)
            return false;
        auto l = left.val_begin->next, r = right.val_begin->next;
        while (l != left.val_end) {
            if (l->value != r->value)
                return false;
            l = l->next, r = r->next;
        }
        return false;
    }

    template<typename Type>
    bool operator!=(const Forward_list<Type> &left, const Forward_list<Type> &right) {
        return !(left == right);
    }

    template<typename Type>
    bool operator<(const Forward_list<Type> &left, const Forward_list<Type> &right) {
        auto l = left.val_begin->next, r = right.val_begin->next;
        while (l != left.val_end && r != right.val_end) {
            if (l->value != r->value)
                return l->value < r->value;
            l = l->next, r = r->next;
        }
        return l == left.val_end && r != right.val_end;
    }

    template<typename Type>
    bool operator<=(const Forward_list<Type> &left, const Forward_list<Type> &right) {
        return !(left > right);
    }

    template<typename Type>
    bool operator>(const Forward_list<Type> &left, const Forward_list<Type> &right) {
        auto l = left.val_begin->next, r = right.val_begin->next;
        while (l != left.val_end && r != right.val_end) {
            if (l->value != r->value)
                return l->value > r->value;
            l = l->next, r = r->next;
        }
        return r == right.val_end && l != left.val_end;
    }

    template<typename Type>
    bool operator>=(const Forward_list<Type> &left, const Forward_list<Type> &right) {
        return !(left < right);
    }

    template<typename Type>
    inline void swap(Forward_list<Type> &left, Forward_List<Type> &right) noexcept {
        left.swap(right);
    }

}// namespace STD

#endif// TINYSTL_FORWARD_LIST_HPP
