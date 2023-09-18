//
// Created by 86152 on 2023/9/11.
//

#ifndef TINYSTL_LIST_HPP
#define TINYSTL_LIST_HPP

#include "Allocater.hpp"
#include "Warning.hpp"
#include "Move.hpp"
#include "Iter.hpp"

namespace STD {

    template<typename Arg>
    class List {
    private:

        struct Node {
            Arg *value;

            Node *last = nullptr, *next = nullptr;

            Node(const Arg &target) : value(Allocate<Arg>(target)) {};

            explicit Node(Arg &&target) : value(Allocate<Arg>(move(target))) {};

            Node(const Arg &target, Node *last, Node *next) : value(Allocate<Arg>(target)), last(last), next(next) {};

            Node(Arg &&target, Node *last, Node *next) : value(Allocate<Arg>(move(target))), last(last), next(next) {};

            Node(Arg *value, Node *last, Node *next) : value(value), last(last), next(next) {};

            ~Node() { Deallocate(value); };

            Node *copy() { return Allocate<Node>(*value); }

        };

        Size size_ = 0;

        Node *val_begin = Allocate<Node>(nullptr, nullptr, nullptr), *val_end = Allocate<Node>(nullptr, nullptr,
                                                                                               nullptr);

        Size release(Node *begin, Node *end);

    public:

        class Iterator;

        class cIterator;

        class rIterator;

        class crIterator;

        List() = default;

        List(Size size, Arg target = Arg());

        List(const std::initializer_list<Arg> &list);

        List(const Iter<Arg> &begin, const Iter<Arg> &end);

        List(const cIter<Arg> &begin, const cIter<Arg> &end);

        List(const List<Arg> &other);

        List(List<Arg> &&other) noexcept;

        ~List<Arg>();

        Size size() const { return size_; };

        Size capacity() const { return size_; };

        void clear();

        List<Arg> &assign(const Iter<Arg> &begin, const Iter<Arg> &end);

        List<Arg> &assign(const cIter<Arg> &begin, const cIter<Arg> &end);

        List<Arg> &assign(const std::initializer_list<Arg> &list);

        template<typename ...args>
        void emplace_front(args &&...vals);

        void push_front(const Arg &val);

        void push_front(const Arg &val, Size size);

        void push_front(const Iter<Arg> &begin, const Iter<Arg> &end);

        void push_front(const cIter<Arg> &begin, const cIter<Arg> &end);

        void pop_front();

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(const Arg &val);

        void push_back(const Arg &val, Size size);

        void push_back(const Iter<Arg> &begin, const Iter<Arg> &end);

        void push_back(const cIter<Arg> &begin, const cIter<Arg> &end);

        void pop_back();

        template<typename ...args>
        Iterator emplace(const Iterator &pos, args &&...vals);

        template<typename ...args>
        cIterator emplace(const cIterator &pos, args &&...vals);

        template<typename ...args>
        rIterator emplace(const rIterator &pos, args &&...vals);

        template<typename ...args>
        crIterator emplace(const crIterator &pos, args &&...vals);

        Iterator insert(const Iterator &pos, const Arg &target);

        cIterator insert(const cIterator &pos, const Arg &target);

        Iterator insert(const Iterator &pos, const std::initializer_list<Arg> &list);

        cIterator insert(const cIterator &pos, const std::initializer_list<Arg> &list);

        Iterator insert(const Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        cIterator insert(const cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert(const Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        cIterator insert(const cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        rIterator insert(const rIterator &pos, const Arg &target);

        crIterator insert(const crIterator &pos, const Arg &target);

        rIterator insert(const rIterator &pos, const std::initializer_list<Arg> &list);

        crIterator insert(const crIterator &pos, const std::initializer_list<Arg> &list);

        rIterator insert(const rIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        crIterator insert(const crIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        rIterator insert(const rIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        crIterator insert(const crIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        rIterator erase(const rIterator &iter);

        crIterator erase(const crIterator &iter);

        rIterator erase(const rIterator &begin, const rIterator &end);

        crIterator erase(const crIterator &begin, const crIterator &end);

        Arg &front() const {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'List::front' function");
            return val_begin->next->value;
        };

        Arg &back() const {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'List::back' function");
            return val_end->last->value;
        };

        bool empty() const { return !size_; };

        List<Arg> &operator=(const List<Arg> &other);

        List<Arg> &operator=(List<Arg> &&other) noexcept;

        template<typename Type>
        friend bool operator==(const List<Type> &left, const List<Type> &right);

        template<typename Type>
        friend bool operator!=(const List<Type> &left, const List<Type> &right);

        template<typename Type>
        friend bool operator<(const List<Type> &left, const List<Type> &right);

        template<typename Type>
        friend bool operator<=(const List<Type> &left, const List<Type> &right);

        template<typename Type>
        friend bool operator>(const List<Type> &left, const List<Type> &right);

        template<typename Type>
        friend bool operator>=(const List<Type> &left, const List<Type> &right);

        Iterator begin() const { return Iterator(val_begin->next); };

        Iterator end() const { return Iterator(val_end); };

        cIterator cbegin() const { return cIterator(val_begin->next); };

        cIterator cend() const { return cIterator(val_end); };

        rIterator rbegin() const { return rIterator(val_end->last); };

        rIterator rend() const { return rIterator(val_begin); };

        crIterator crbegin() const { return crIterator(val_end->last); };

        crIterator crend() const { return crIterator(val_begin); };

    };


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    Size List<Arg>::release(List<Arg>::Node *begin, List<Arg>::Node *end) {
        Size count = 0;
        Node *prv = begin->last, *temp;
        while (begin != end) {
            temp = begin;
            begin = begin->next;
            Deallocate(temp);
            ++count;
        }
        prv->next = end;
        end->last = prv;
        return count;
    }

    template<typename Arg>
    List<Arg>::List(Size size, Arg target) : size_(size) {
        val_begin->next = val_end;
        Node *last = val_begin, *now = val_begin;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(target, last, nullptr);
            last->next = now;
            last = now;
        }
        val_end->last = now;
        now->next = val_end;
    }

    template<typename Arg>
    List<Arg>::List(const std::initializer_list<Arg> &list) : size_(list.size()) {
        if (size_) {
            auto temp = const_cast<Arg *>(list.begin());
            Node *last = val_begin, *now = val_begin;
            for (int i = 0; i < size_; ++i) {
                now = Allocate<List<Arg>::Node>(*temp, last, nullptr);
                last->next = now;
                last = now;
                ++temp;
            }
            val_end->last = now;
            now->next = val_end;
        }
    }

    template<typename Arg>
    List<Arg>::List(const Iter<Arg> &begin, const Iter<Arg> &end) {
        auto temp = begin.deep_copy();
        Node *last = val_begin, *now = val_begin;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        val_end->last = now;
        now->next = val_end;
    }

    template<typename Arg>
    List<Arg>::List(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto temp = begin.deep_copy();
        Node *last = val_begin, *now = val_begin;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        val_end->last = now;
        now->next = val_end;
    }

    template<typename Arg>
    List<Arg>::List(const List<Arg> &other) : size_(other.size_) {
        Node *last = val_begin, *now = val_begin, *temp = other.val_begin->next;
        for (int i = 0; i < size_; ++i) {
            now = temp->copy();
            now->last = last;
            last->next = now;
            temp = temp->next;
            last = now;
        }
        val_end->last = now;
        now->next = val_end;
    }

    template<typename Arg>
    List<Arg>::List(List<Arg> &&other) noexcept : size_(other.size_), val_begin(other.val_begin),
                                                  val_end(other.val_end) {
        if (other.size_) {
            val_begin->next = other.val_begin->next;
            other.val_begin->next->last = val_begin;
            val_end->last = other.val_end->last;
            other.val_end->last->next = val_end;
            other.size_ = 0;
            other.val_begin->next = other.val_end;
            other.val_end->last = other.val_begin;
        }
    }

    template<typename Arg>
    List<Arg>::~List<Arg>() {
        clear();
        Deallocate(val_begin);
        Deallocate(val_end);
    }

    template<typename Arg>
    void List<Arg>::clear() {
        release(val_begin->next, val_end);
        size_ = 0;
    }


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    List<Arg> &List<Arg>::assign(const std::initializer_list<Arg> &list) {
        clear();
        size_ = list.size();
        Node *last = val_begin, *now = val_begin;
        for (auto &t: list) {
            now = Allocate<List<Arg>::Node>(t, last, nullptr);
            last->next = now;
            last = now;
        }
        val_end->last = now;
        now->next = val_end;
        return *this;
    }

    template<typename Arg>
    List<Arg> &List<Arg>::assign(const Iter<Arg> &begin, const Iter<Arg> &end) {
        assign(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    List<Arg> &List<Arg>::assign(const cIter<Arg> &begin, const cIter<Arg> &end) {
        clear();
        auto temp = begin.deep_copy();
        Node *last = val_begin, *now = val_begin;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        val_end->last = now;
        now->next = val_end;
    }

    template<typename Arg>
    template<typename... args>
    void List<Arg>::emplace_front(args &&... vals) {
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), nullptr, val_begin);
        auto record = val_begin->next;
        val_begin->next = temp;
        temp->last = val_begin;
        record->last = temp;
        temp->next = record;
        ++size_;
    }

    template<typename Arg>
    void List<Arg>::push_front(const Arg &val) {
        push_front(val, 1);
    }

    template<typename Arg>
    void List<Arg>::push_front(const Arg &val, Size size) {
        Node *last = val_begin, *now = val_begin, *record = val_begin->next;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(val, last, nullptr);
            last->next = now;
            last = now;
        }
        record->last = now;
        now->next = record;
        size_ += size;
    }

    template<typename Arg>
    void List<Arg>::push_front(const Iter<Arg> &begin, const Iter<Arg> &end) {
        push_front(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    void List<Arg>::push_front(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto temp = begin.deep_copy();
        Node *last = val_begin, *now = val_begin, *record = val_begin->next;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        record->last = now;
        now->next = record;
    }

    template<typename Arg>
    void List<Arg>::pop_front() {
        if (size_) {
            auto temp = val_begin->next;
            val_begin->next = temp->next;
            temp->next->last = val_begin;
            --size_;
            Deallocate(temp);
        }
    }

    template<typename Arg>
    template<typename... args>
    void List<Arg>::emplace_back(args &&... vals) {
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), val_end, nullptr);
        val_end->last->next = temp;
        temp->last = val_end->last;
        temp->next = val_end;
        val_end->last = temp;
        ++size_;
    }

    template<typename Arg>
    void List<Arg>::push_back(const Arg &val) {
        push_back(val, 1);
    }

    template<typename Arg>
    void List<Arg>::push_back(const Arg &val, Size size) {
        Node *last = val_end->last, *now = val_end->last;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(val, last, nullptr);
            last->next = now;
            last = now;
        }
        val_end->last = now;
        now->next = val_end;
        size_ += size;
    }

    template<typename Arg>
    void List<Arg>::push_back(const Iter<Arg> &begin, const Iter<Arg> &end) {
        push_back(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    void List<Arg>::push_back(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto temp = begin.deep_copy();
        Node *last = val_end->last, *now = val_end->last;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        val_end->last = now;
        now->next = val_end;
    }

    template<typename Arg>
    void List<Arg>::pop_back() {
        if (size_) {
            Node *temp = val_end->last;
            temp->last->next = val_end;
            val_begin->last = temp->last;
            --size_;
            Deallocate(temp);
        }
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::Iterator List<Arg>::emplace(const List::Iterator &pos, args &&... vals) {
        return List<Arg>::Iterator(emplace(List<Arg>::cIterator(pos.node), forward<args>(vals)...).node);
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::cIterator List<Arg>::emplace(const List::cIterator &pos, args &&... vals) {
        if (!pos.target && pos.node != val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), pos.node->last, pos.node);
        pos.node->last->next = temp;
        pos.node->last = temp;
        ++size_;
        return List<Arg>::cIterator(temp);
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::rIterator List<Arg>::emplace(const List::rIterator &pos, args &&... vals) {
        return List<Arg>::rIterator(emplace(List<Arg>::crIterator(pos.node), forward<args>(vals)...).node);
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::crIterator List<Arg>::emplace(const List::crIterator &pos, args &&... vals) {
        if (!pos.target && pos.node != val_begin)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), pos.node, pos.node->next);
        pos.node->next->last = temp;
        pos.node->next = temp;
        ++size_;
        return List<Arg>::crIterator(temp);
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::insert(const List::Iterator &pos, const Arg &target) {
        return List<Arg>::Iterator(insert(List<Arg>::cIterator(pos.node), target).node);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::insert(const List<Arg>::cIterator &pos, const Arg &target) {
        if (!pos.target && pos.node != val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        auto temp = Allocate<Node>(target, pos.node->last, pos.node);
        pos.node->last->next = temp;
        pos.node->last = temp;
        ++size_;
        return List<Arg>::cIterator(temp);
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert(const List<Arg>::Iterator &pos, const std::initializer_list<Arg> &list) {
        if (!pos.node->last) throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        size_ += list.size();
        auto last = pos.node->last, now = pos.node->last, head = nullptr;
        for (auto &t: list) {
            now = Allocate<List<Arg>::Node>(t, last, nullptr);
            if (!head) head = now;
            last->next = now;
            last = now;
        }
        pos.node->last = now;
        now->next = pos.node;
        return List::Iterator(head);
    }

    template<typename Arg>
    typename List<Arg>::cIterator
    List<Arg>::insert(const List<Arg>::cIterator &pos, const std::initializer_list<Arg> &list) {
        return List<Arg>::cIterator(insert(Iterator(pos.node), list).node);
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert(const List<Arg>::Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return List<Arg>::Iterator(insert(List<Arg>::cIterator(pos.node), *begin.to_const(), *end.to_const()).node);
    }

    template<typename Arg>
    typename List<Arg>::cIterator
    List<Arg>::insert(const List<Arg>::cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return insert(pos, *begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert(const List<Arg>::Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        return List<Arg>::Iterator(insert(List<Arg>::cIterator(pos.node), begin, end).node);
    }

    template<typename Arg>
    typename List<Arg>::cIterator
    List<Arg>::insert(const List<Arg>::cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (begin == end) return pos;
        if (!pos.target && pos.node != val_end)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        Node *last = pos.node->last, *now = pos.node->last, *head = nullptr;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<List<Arg>::Node>(**temp, last, nullptr);
            if (!head) head = now;
            last->next = now;
            last = now;
            ++(*temp), ++size_;
        }
        pos.node->last = now;
        now->next = pos.node;
        return List<Arg>::cIterator(head);
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::insert(const List::rIterator &pos, const Arg &target) {
        return List<Arg>::rIterator(insert(List<Arg>::crIterator(pos.node), target).node);
    }

    template<typename Arg>
    typename List<Arg>::crIterator List<Arg>::insert(const List<Arg>::crIterator &pos, const Arg &target) {
        if (!pos.target && pos.node != val_begin)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        auto temp = Allocate<Node>(target, pos.node, pos.node->next);
        pos.node->next->last = temp;
        pos.node->next = temp;
        ++size_;
        return List<Arg>::crIterator(temp);
    }

    template<typename Arg>
    typename List<Arg>::rIterator
    List<Arg>::insert(const List<Arg>::rIterator &pos, const std::initializer_list<Arg> &list) {
        if (!pos.node->next) throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        size_ += list.size();
        auto next = pos.node->next, now = pos.node->next, tail = nullptr;
        for (auto &t: list) {
            now = Allocate<List<Arg>::Node>(t, nullptr, next);
            if (!tail) tail = now;
            next->last = now;
            next = now;
        }
        pos.node->next = now;
        now->last = pos.node;
        return List::Iterator(tail);
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::insert(const List<Arg>::crIterator &pos, const std::initializer_list<Arg> &list) {
        return List<Arg>::crIterator(insert(rIterator(pos.node), list).node);
    }

    template<typename Arg>
    typename List<Arg>::rIterator
    List<Arg>::insert(const List<Arg>::rIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return List<Arg>::rIterator(insert(List<Arg>::crIterator(pos.node), *begin.to_const(), *end.to_const()).node);
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::insert(const List<Arg>::crIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return insert(pos, *begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    typename List<Arg>::rIterator
    List<Arg>::insert(const List<Arg>::rIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        return List<Arg>::rIterator(insert(List<Arg>::crIterator(pos.node), begin, end).node);
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::insert(const List<Arg>::crIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (begin == end) return pos;
        if (!pos.target && pos.node != val_begin)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        Node *next = pos.node->next, *now = pos.node->next, *tail = nullptr;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<List<Arg>::Node>(**temp, nullptr, next);
            if (!tail) tail = now;
            next->last = now;
            next = now;
            ++(*temp), ++size_;
        }
        pos.node->next = now;
        now->last = pos.node;
        return crIterator(tail);
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::erase(const List<Arg>::Iterator &iter) {
        return List<Arg>::Iterator(erase(List<Arg>::cIterator(iter.node)).node);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::erase(const List<Arg>::cIterator &iter) {
        if (!iter.target) throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        auto record = iter.node->next;
        iter.node->last->next = record;
        record->last = iter.node->last;
        Deallocate(iter.node);
        --size_;
        return List<Arg>::cIterator(record);
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::erase(const List<Arg>::Iterator &begin, const List<Arg>::Iterator &end) {
        return List<Arg>::Iterator(erase(List<Arg>::cIterator(begin.node), List<Arg>::cIterator(end.node)).node);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::erase(const List<Arg>::cIterator &begin, const List<Arg>::cIterator &end) {
        if (!begin.target) throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        size_ -= release(begin.node, end.node);
        return List<Arg>::cIterator(end.node);
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::erase(const List<Arg>::rIterator &iter) {
        return List<Arg>::rIterator(erase(List<Arg>::crIterator(iter.node)).node);
    }

    template<typename Arg>
    typename List<Arg>::crIterator List<Arg>::erase(const List<Arg>::crIterator &iter) {
        if (!iter.target) throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        auto record = iter.node->next;
        iter.node->last->next = record;
        record->last = iter.node->last;
        Deallocate(iter.node);
        --size_;
        return List<Arg>::crIterator(record);
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::erase(const List<Arg>::rIterator &begin, const List<Arg>::rIterator &end) {
        return List<Arg>::rIterator(erase(List<Arg>::crIterator(begin.node), List<Arg>::crIterator(end.node)).node);
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::erase(const List<Arg>::crIterator &begin, const List<Arg>::crIterator &end) {
        if (!begin.target) throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        size_ -= release(end.node->next, begin.node->next);
        return List<Arg>::crIterator(end.node->next);
    }

    template<typename Arg>
    List<Arg> &List<Arg>::operator=(const List<Arg> &other) {
        if (this == &other) return *this;
        clear();
        size_ = other.size_;
        Node *last = val_begin, *now = val_begin, *temp = other.val_begin->next;
        for (int i = 0; i < size_; ++i) {
            now = temp->copy();
            now->last = last;
            last->next = now;
            temp = temp->next;
            last = now;
        }
        val_end->last = now;
        now->next = val_end;
        return *this;
    }

    template<typename Arg>
    List<Arg> &List<Arg>::operator=(List<Arg> &&other) noexcept {
        if (this == &other) return *this;
        clear();
        if (other.size_) {
            size_ = other.size_;
            val_begin->next = other.val_begin->next;
            other.val_begin->next->last = val_begin;
            val_end->last = other.val_end->last;
            other.val_end->last->next = val_end;
            other.size_ = 0;
            other.val_begin->next = other.val_end;
            other.val_end->last = other.val_begin;
        }
        return *this;
    }

    template<typename Type>
    bool operator==(const List<Type> &left, const List<Type> &right) {
        if (left.size_ != right.size_) return false;
        auto l = left.val_begin->next, r = right.val_begin->next;
        while (l != left.val_end) {
            if (l->value != r->value) return false;
            l = l->next, r = r->next;
        }
        return false;
    }

    template<typename Type>
    bool operator!=(const List<Type> &left, const List<Type> &right) {
        return !(left == right);
    }

    template<typename Type>
    bool operator<(const List<Type> &left, const List<Type> &right) {
        auto l = left.val_begin->next, r = right.val_begin->next;
        while (l != left.val_end && r != right.val_end) {
            if (l->value != r->value) return l->value < r->value;
            l = l->next, r = r->next;
        }
        return l == left.val_end && r != right.val_end;
    }

    template<typename Type>
    bool operator<=(const List<Type> &left, const List<Type> &right) {
        return !(left > right);
    }

    template<typename Type>
    bool operator>(const List<Type> &left, const List<Type> &right) {
        auto l = left.val_begin->next, r = right.val_begin->next;
        while (l != left.val_end && r != right.val_end) {
            if (l->value != r->value) return l->value > r->value;
            l = l->next, r = r->next;
        }
        return r == right.val_end && l != left.val_end;
    }

    template<typename Type>
    bool operator>=(const List<Type> &left, const List<Type> &right) {
        return !(left < right);
    }


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    class List<Arg>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        List<Arg>::Node *node = nullptr;

        virtual Iterator &operator=(List<Arg>::Node *ptr) {
            target = ptr->value;
            node = ptr;
            return *this;
        };

        explicit Iterator(List<Arg>::Node *ptr) : Iter<Arg>(ptr->value), node(ptr) {};

    public:
        friend class List<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<List<Arg>::Iterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override { return make_shared<List<Arg>::cIterator>(cIterator(node)); };

        Iterator(const Iterator &other) : Iter<Arg>(other.target), node(other.node) {};

        ~Iterator() = default;

        Iterator &operator=(const Iterator &other) {
            target = other.target;
            node = other.node;
            return *this;
        };

        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        Iterator &operator++() override {
            if (!node->next) throw outOfRange("List::Iterator out of range\n");
            node = node->next;
            target = node->value;
            return *this;
        };

        Iterator operator++(int) {
            if (!node->next) throw outOfRange("List::Iterator out of range\n");
            auto temp = node;
            node = node->next;
            target = node->value;
            return List<Arg>::Iterator(temp);
        };

        virtual Iterator &operator--() {
            if (!node->last) throw outOfRange("List::Iterator out of range\n");
            node = node->last;
            target = node->value;
            return *this;
        };

        Iterator operator--(int) {
            if (!node->last) throw outOfRange("List::Iterator out of range\n");
            auto temp = node;
            node = node->last;
            target = node->value;
            return List<Arg>::Iterator(temp);
        };

        friend bool
        operator==(const Iterator &left, const Iterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const Iterator &left, const Iterator &right) { return left.target != right.target; };
    };

//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    class List<Arg>::cIterator : public cIter<Arg> {
    protected:
        using cIter<Arg>::target;

        List<Arg>::Node *node = nullptr;

        virtual cIterator &operator=(List<Arg>::Node *ptr) {
            target = ptr->value;
            node = ptr;
            return *this;
        };

        explicit cIterator(List<Arg>::Node *ptr) : cIter<Arg>(ptr->value), node(ptr) {};

    public:
        friend class List<Arg>;

        friend class List<Arg>::Iterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<List<Arg>::cIterator>(*this); };

        cIterator(const cIterator &other) : cIter<Arg>(other.target), node(other.node) {};

        ~cIterator() = default;

        cIterator &operator=(const cIterator &other) {
            target = other.target;
            node = other.node;
            return *this;
        };


        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        cIterator &operator++() override {
            if (!node->next) throw outOfRange("List::cIterator out of range\n");
            node = node->next;
            target = node->value;
            return *this;
        };

        cIterator operator++(int) {
            if (!node->next) throw outOfRange("List::cIterator out of range\n");
            auto temp = node;
            node = node->next;
            target = node->value;
            return List<Arg>::cIterator(temp);
        };

        virtual cIterator &operator--() {
            if (!node->last) throw outOfRange("List::cIterator out of range\n");
            node = node->last;
            target = node->value;
            return *this;
        };

        cIterator operator--(int) {
            if (!node->last) throw outOfRange("List::cIterator out of range\n");
            auto temp = node;
            node = node->last;
            target = node->value;
            return List<Arg>::cIterator(temp);
        };

        friend bool
        operator==(const cIterator &left, const cIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const cIterator &left, const cIterator &right) { return left.target != right.target; };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class List<Arg>::rIterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        List<Arg>::Node *node;

        rIterator &operator=(List<Arg>::Node *ptr) {
            target = ptr->value;
            node = ptr;
            return *this;
        };

        explicit rIterator(List<Arg>::Node *ptr) : Iter<Arg>(ptr->value), node(ptr) {};

    public:
        friend class List<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<List<Arg>::rIterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<List<Arg>::crIterator>(crIterator(node));
        };

        rIterator(const rIterator &other) : Iter<Arg>(other.target), node(other.node) {};

        ~rIterator() = default;

        rIterator &operator=(const rIterator &other) {
            target = other.target;
            node = other.node;
            return *this;
        };


        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        rIterator &operator++() override {
            if (!node->last) throw outOfRange("List::rIterator out of range\n");
            node = node->last;
            target = node->value;
            return *this;
        };

        rIterator operator++(int) {
            if (!node->last) throw outOfRange("List::rIterator out of range\n");
            auto temp = node;
            node = node->last;
            target = node->value;
            return List<Arg>::rIterator(temp);
        };

        rIterator &operator--() {
            if (!node->next) throw outOfRange("List::rIterator out of range\n");
            node = node->next;
            target = node->value;
            return *this;
        };

        rIterator operator--(int) {
            if (!node->next) throw outOfRange("List::rIterator out of range\n");
            auto temp = node;
            node = node->next;
            target = node->value;
            return List<Arg>::rIterator(temp);
        };

        friend bool
        operator==(const rIterator &left, const rIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const rIterator &left, const rIterator &right) { return left.target != right.target; };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class List<Arg>::crIterator : public cIter<Arg> {
    protected:
        using cIter<Arg>::target;

        List<Arg>::Node *node;

        crIterator &operator=(List<Arg>::Node *ptr) {
            target = ptr->value;
            node = ptr;
            return *this;
        };

        explicit crIterator(List<Arg>::Node *ptr) : cIter<Arg>(ptr->value), node(ptr) {};

    public:
        friend class List<Arg>;

        friend class List<Arg>::rIterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<List<Arg>::crIterator>(*this); };

        crIterator(const crIterator &other) : cIter<Arg>(other.target), node(other.node) {};

        ~crIterator() = default;

        crIterator &operator=(const crIterator &other) {
            target = other.target;
            node = other.node;
            return *this;
        };


        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        crIterator &operator++() override {
            if (!node->last) throw outOfRange("List::crIterator out of range\n");
            node = node->last;
            target = node->value;
            return *this;
        };

        crIterator operator++(int) {
            if (!node->last) throw outOfRange("List::crIterator out of range\n");
            auto temp = node;
            node = node->last;
            target = node->value;
            return List<Arg>::crIterator(temp);
        };

        crIterator &operator--() {
            if (!node->next) throw outOfRange("List::crIterator out of range\n");
            node = node->next;
            target = node->value;
            return *this;
        };

        crIterator operator--(int) {
            if (!node->next) throw outOfRange("List::crIterator out of range\n");
            auto temp = node;
            node = node->next;
            target = node->value;
            return List<Arg>::crIterator(temp);
        };

        friend bool
        operator==(const crIterator &left, const crIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const crIterator &left, const crIterator &right) { return left.target != right.target; };
    };

}

#endif //TINYSTL_LIST_HPP
