//
// Created by 86152 on 2023/9/15.
//

#ifndef TINYSTL_FORWARD_LIST_HPP
#define TINYSTL_FORWARD_LIST_HPP

#include "Iter.hpp"
#include "Memory.hpp"
#include "Move.hpp"
#include "Warning.hpp"

namespace STD {

    template<typename Arg>
    class Forward_list {
    private:
        struct Node {
            Arg *value;

            Node *next;

            Node(const Arg &target, Node *next = nullptr) : value(Allocate(target)), next(next) {};

            Node(Arg &&target, Node *next = nullptr) : value(Allocate(move(target))), next(next) {};

            Node(Arg *target, Node *next = nullptr) : value(target), next(next) {};

            ~Node() { Deallocate(value); };

            Node *copy() { return Allocate<Node>(*value); }
        };


        Size size_ = 0;

        Node *val_end = Allocate<Node>(nullptr, nullptr), *val_begin = Allocate<Node>(nullptr, val_end);

        Size release_after(Node *begin, Node *end);

    public:

        class Iterator;

        class cIterator;

        Forward_list() = default;

        Forward_list(Size size, Arg target = Arg());

        Forward_list(const std::initializer_list<Arg> &list);

        Forward_list(const Iter<Arg> &begin, const Iter<Arg> &end);

        Forward_list(const cIter<Arg> &begin, const cIter<Arg> &end);

        Forward_list(const Forward_list<Arg> &other);

        Forward_list(Forward_list<Arg> &&other) noexcept;

        ~Forward_list();

        Size size() const { return size_; };

        Size capacity() const { return size_; };

        void clear();

        Forward_list<Arg> &assign(const Iter<Arg> &begin, const Iter<Arg> &end);

        Forward_list<Arg> &assign(const cIter<Arg> &begin, const cIter<Arg> &end);

        Forward_list<Arg> &assign(const std::initializer_list<Arg> &list);

        template<typename ...args>
        void emplace_front(args &&...vals);

        void push_front(const Arg &val);

        void push_front(Arg &&val);

        void push_front(const Arg &val, Size size);

        void push_front(const Iter<Arg> &begin, const Iter<Arg> &end);

        void push_front(const cIter<Arg> &begin, const cIter<Arg> &end);

        void pop_front();

        template<typename ...args>
        Iterator emplace_after(const Iterator &pos, args &&...vals);

        template<typename ...args>
        cIterator emplace_after(const cIterator &pos, args &&...vals);

        Iterator insert_after(const Iterator &pos, const Arg &target);

        cIterator insert_after(const cIterator &pos, const Arg &target);

        Iterator insert_after(const Iterator &pos, Size size, const Arg &target);

        cIterator insert_after(const cIterator &pos, Size size, const Arg &target);

        Iterator insert_after(const Iterator &pos, const std::initializer_list<Arg> &list);

        cIterator insert_after(const cIterator &pos, const std::initializer_list<Arg> &list);

        Iterator insert_after(const Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        cIterator insert_after(const cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert_after(const Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        cIterator insert_after(const cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator erase_after(const Iterator &pos);

        Iterator erase_after(const Iterator &begin, const Iterator &end);

        cIterator erase_after(const cIterator &pos);

        cIterator erase_after(const cIterator &begin, const cIterator &end);

        Arg &front() const {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'List::front' function");
            return val_begin->next->value;
        };

        bool empty() const { return !size_; };

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

        Iterator begin() const { return Iterator(val_begin->next); };

        Iterator end() const { return Iterator(val_end); };

        cIterator cbegin() const { return cIterator(val_begin->next); };

        cIterator cend() const { return cIterator(val_end); };

        Iterator before_begin() const { return Iterator(val_begin); };

        cIterator before_cbegin() const { return cIterator(val_begin); };
    };

//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    Forward_list<Arg>::Forward_list(Size size, Arg target) : size_(size) {
        auto last = val_begin, now = val_begin;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(target);
            last->next = now;
            last = now;
        }
        now->next = val_end;
    }

    template<typename Arg>
    Forward_list<Arg>::Forward_list(const std::initializer_list<Arg> &list) : size_(list.size()) {
        auto last = val_begin, now = val_begin;
        for (auto &t: list) {
            now = Allocate<Node>(t);
            last->next = now;
            last = now;
        }
        now->next = val_end;
    }

    template<typename Arg>
    Forward_list<Arg>::Forward_list(const Iter<Arg> &begin, const Iter<Arg> &end) :
            Forward_list(*begin.to_const(), *end.to_const()) {}

    template<typename Arg>
    Forward_list<Arg>::Forward_list(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto last = val_begin, now = val_begin;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<Node>(*temp);
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        now->next = val_end;
    }

    template<typename Arg>
    Forward_list<Arg>::Forward_list(const Forward_list<Arg> &other) : size_(other.size_) {
        auto last = val_begin, now = val_begin, temp = other.val_begin->next;
        for (int i = 0; i < size_; ++i) {
            now = temp->copy();
            last->next = now;
            last = now;
            temp = temp->next;
        }
        now->next = val_end;
    }

    template<typename Arg>
    Forward_list<Arg>::Forward_list(Forward_list<Arg> &&other) noexcept : size_(other.size_) {
        val_begin->next = other.val_begin->next;
        auto temp = other.val_begin->next;
        for (int i = 0; i < size_ - 1; ++i) temp = temp->next;
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
    Size Forward_list<Arg>::release_after(Node *begin, Node *end) {
        if (begin == end) return 0;
        Size count = 0;
        auto temp = begin->next, t = temp;
        while (temp != end) {
            t = temp;
            temp = temp->next;
            Deallocate(t);
            ++count;
        }
        begin->next = end;
        return count;
    }


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    void Forward_list<Arg>::clear() {
        release_after(val_begin, val_end);
        size_ = 0;
    }

    template<typename Arg>
    Forward_list<Arg> &Forward_list<Arg>::assign(const Iter<Arg> &begin, const Iter<Arg> &end) {
        return assign(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    Forward_list<Arg> &Forward_list<Arg>::assign(const cIter<Arg> &begin, const cIter<Arg> &end) {
        clear();
        auto last = val_begin, now = val_begin;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<Node>(**temp);
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        now->next = val_end;
        return *this;
    }

    template<typename Arg>
    Forward_list<Arg> &Forward_list<Arg>::assign(const std::initializer_list<Arg> &list) {
        clear();
        auto last = val_begin, now = val_begin;
        for (auto &t: list) {
            now = Allocate<Node>(t);
            last->next = now;
            last = now;
        }
        now->next = val_end;
        size_ = list.size();
        return *this;
    }

    template<typename Arg>
    template<typename... args>
    void Forward_list<Arg>::emplace_front(args &&... vals) {
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
        auto last = val_begin, now = val_begin, record = val_begin->next;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(val);
            last->next = now;
            last = now;
        }
        now->next = record;
        size_ += size;
    }

    template<typename Arg>
    void Forward_list<Arg>::push_front(const Iter<Arg> &begin, const Iter<Arg> &end) {
        push_front(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    void Forward_list<Arg>::push_front(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto last = val_begin, now = val_begin, record = val_begin->next;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<Node>(**temp);
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        now->next = record;
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
    Forward_list<Arg>::emplace_after(const Forward_list::Iterator &pos, args &&... vals) {
        if (!pos.node->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::emplace_after' function");
        auto temp = Allocate<Node>(Allocate<Arg>(forward<args>(vals)...), pos.node->next);
        pos.node->next = temp;
        ++size_;
        return Forward_list::Iterator(temp);
    }

    template<typename Arg>
    template<typename... args>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::emplace_after(const Forward_list::cIterator &pos, args &&... vals) {
        if (!pos.node->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::emplace_after' function");
        auto temp = Allocate<Node>(Allocate<Arg>(forward<args>(vals)...), pos.node->next);
        pos.node->next = temp;
        ++size_;
        return Forward_list::cIterator(temp);
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Forward_list::Iterator &pos, const Arg &target) {
        if (!pos.node->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        auto temp = Allocate<Node>(target, pos.node->next);
        pos.node->next = temp;
        ++size_;
        return Forward_list::Iterator(temp);
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const Forward_list::cIterator &pos, const Arg &target) {
        if (!pos.node->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        auto temp = Allocate<Node>(target, pos.node->next);
        pos.node->next = temp;
        ++size_;
        return Forward_list::cIterator(temp);
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Forward_list::Iterator &pos, Size size, const Arg &target) {
        if (!pos.node->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        auto last = pos.node, now = pos.node, record = pos.node->next;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(target);
            last->next = now;
            last = now;
        }
        now->next = record;
        size_ += size;
        return Forward_list::Iterator(now);
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const Forward_list::cIterator &pos, Size size, const Arg &target) {
        return Forward_list::cIterator(insert_after(Iterator(pos.node), size, target).node);
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Forward_list::Iterator &pos, const std::initializer_list<Arg> &list) {
        if (!pos.node->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        auto last = pos.node, now = pos.node, record = pos.node->next;
        for (auto &t: list) {
            now = Allocate<Node>(t);
            last->next = now;
            last = now;
        }
        now->next = record;
        size_ += list.size();
        return Forward_list::Iterator(now);
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const Forward_list::cIterator &pos, const std::initializer_list<Arg> &list) {
        return Forward_list::cIterator(insert_after(Iterator(pos.node), list).node);
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Forward_list::Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return Forward_list::Iterator(insert_after(cIterator(pos.node), *begin.to_const(), *end.to_const()).node);
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const Forward_list::cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return insert_after(pos, *begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::insert_after(const Forward_list::Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        return Forward_list::Iterator(insert_after(cIterator(pos.node), begin, end).node);
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::insert_after(const Forward_list::cIterator &pos, const cIter<Arg> &begin,
                                    const cIter<Arg> &end) {
        if (!pos.node->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::insert_after' function");
        auto last = pos.node, now = pos.node, record = pos.node->next;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<Node>(**temp);
            last->next = now;
            last = now;
            ++size_, ++(*temp);
        }
        now->next = record;
        return Forward_list::cIterator(now);
    }


    template<typename Arg>
    typename Forward_list<Arg>::Iterator Forward_list<Arg>::erase_after(const Forward_list::Iterator &pos) {
        if (!pos.node->next || !pos.node->next->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::erase_after' function");
        auto temp = pos.node->next;
        pos.node->next = temp->next;
        Deallocate(temp);
        --size_;
        return pos;
    }

    template<typename Arg>
    typename Forward_list<Arg>::Iterator
    Forward_list<Arg>::erase_after(const Forward_list::Iterator &begin, const Forward_list::Iterator &end) {
        if (!begin.node->next || !begin.node->next->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::erase_after' function");
        size_ -= release_after(begin.node, end.node);
        return begin;
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator Forward_list<Arg>::erase_after(const Forward_list::cIterator &pos) {
        if (!pos.node->next || !pos.node->next->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::erase_after' function");
        auto temp = pos.node->next;
        pos.node->next = temp->next;
        Deallocate(temp);
        --size_;
        return pos;
    }

    template<typename Arg>
    typename Forward_list<Arg>::cIterator
    Forward_list<Arg>::erase_after(const Forward_list::cIterator &begin, const Forward_list::cIterator &end) {
        if (!begin.node->next || !begin.node->next->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'Forward_list::erase_after' function");
        size_ -= release_after(begin.node, end.node);
        return begin;
    }

    template<typename Arg>
    Forward_list<Arg> &Forward_list<Arg>::operator=(const Forward_list<Arg> &other) {
        if (this == &other) return *this;
        clear();
        size_ = other.size_;
        auto last = val_begin, now = val_begin, temp = other.val_begin->next;
        for (int i = 0; i < size_; ++i) {
            now = temp->copy();
            last->next = now;
            last = now;
            temp = temp->next;
        }
        now->next = val_end;
        return *this;
    }

    template<typename Arg>
    Forward_list<Arg> &Forward_list<Arg>::operator=(Forward_list<Arg> &&other) noexcept {
        if (this == &other) return *this;
        clear();
        val_begin->next = other.val_begin->next;
        size_ = other.size_;
        auto temp = other.val_begin->next;
        for (int i = 0; i < size_ - 1; ++i) temp = temp->next;
        temp->next = val_end;
        other.size_ = 0;
        other.val_begin->next = other.val_end;
        return *this;
    }

    template<typename Type>
    bool operator==(const Forward_list<Type> &left, const Forward_list<Type> &right) {
        if (left.size_ != right.size_) return false;
        auto l = left.val_begin->next, r = right.val_begin->next;
        while (l != left.val_end) {
            if (l->value != r->value) return false;
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
            if (l->value != r->value) return l->value < r->value;
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
            if (l->value != r->value) return l->value > r->value;
            l = l->next, r = r->next;
        }
        return r == right.val_end && l != left.val_end;
    }

    template<typename Type>
    bool operator>=(const Forward_list<Type> &left, const Forward_list<Type> &right) {
        return !(left < right);
    }


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Forward_list<Arg>::Iterator : public Iter<Arg> {
        using Iter<Arg>::target;

        Forward_list<Arg>::Node *node = nullptr;

        Iterator &operator=(Forward_list<Arg>::Node *ptr) {
            target = ptr->value;
            node = ptr;
            return *this;
        };

        explicit Iterator(Forward_list<Arg>::Node *ptr) : Iter<Arg>(ptr->value), node(ptr) {};

    public:
        friend class Forward_list<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Forward_list<Arg>::Iterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Forward_list<Arg>::cIterator>(cIterator(node));
        };

        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        Iterator &operator++() override {
            if (!node->next) throw outOfRange("Forward_list::Iterator out of range\n");
            node = node->next;
            target = node->value;
            return *this;
        };

        Iterator operator++(int) {
            if (!node->next) throw outOfRange("Forward_list::Iterator out of range\n");
            auto temp = node;
            node = node->next;
            target = node->value;
            return Forward_list<Arg>::Iterator(temp);
        };

        friend bool
        operator==(const Iterator &left, const Iterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const Iterator &left, const Iterator &right) { return left.target != right.target; };
    };

    template<typename Arg>
    class Forward_list<Arg>::cIterator : public cIter<Arg> {
        using cIter<Arg>::target;

        Forward_list<Arg>::Node *node = nullptr;

        cIterator &operator=(Forward_list<Arg>::Node *ptr) {
            target = ptr->value;
            node = ptr;
            return *this;
        };

        explicit cIterator(Forward_list<Arg>::Node *ptr) : cIter<Arg>(ptr->value), node(ptr) {};

    public:
        friend class Forward_list<Arg>;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Forward_list<Arg>::cIterator>(*this); };

        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        cIterator &operator++() override {
            if (!node->next) throw outOfRange("Forward_list::Iterator out of range\n");
            node = node->next;
            target = node->value;
            return *this;
        };

        cIterator operator++(int) {
            if (!node->next) throw outOfRange("Forward_list::Iterator out of range\n");
            auto temp = node;
            node = node->next;
            target = node->value;
            return Forward_list<Arg>::cIterator(temp);
        };

        friend bool
        operator==(const cIterator &left, const cIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const cIterator &left, const cIterator &right) { return left.target != right.target; };
    };

}

#endif //TINYSTL_FORWARD_LIST_HPP
