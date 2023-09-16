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

        Node *val_begin = Allocate<Node>(nullptr);

        Size release(Node *begin, Node *end);

    public:

        class Iterator;

        class cIterator;

        Forward_list() = default;

        Forward_list(Size size, Arg target = Arg());

        Forward_list(std::initializer_list<Arg> list);

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

        Forward_list<Arg> &assign(std::initializer_list<Arg> list);

        template<typename ...args>
        void emplace_front(args &&...vals);

        void push_front(const Arg &val);

        void push_front(const Arg &val, Size size);

        void push_front(const Iter<Arg> &begin, const Iter<Arg> &end);

        void push_front(const cIter<Arg> &begin, const cIter<Arg> &end);

        void pop_front();

        template<typename ...args>
        Iterator emplace(const Iterator &pos, args &&...vals);

        template<typename ...args>
        cIterator emplace(const cIterator &pos, args &&...vals);

        Iterator insert(const Iterator &pos, const Arg &target);

        cIterator insert(const cIterator &pos, const Arg &target);

        Iterator insert(const Iterator &pos, std::initializer_list<Arg> list);

        cIterator insert(const cIterator &pos, std::initializer_list<Arg> list);

        Iterator insert(const Iterator &pos, Size size, const Arg &target);

        cIterator insert(const cIterator &pos, Size size, const Arg &target);

        Iterator insert(const Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        cIterator insert(const cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert(const Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        cIterator insert(const cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator insert_after(const Iterator &pos, const Arg &target);

        cIterator insert_after(const cIterator &pos, const Arg &target);

        Iterator insert_after(const Iterator &pos, Size size, const Arg &target);

        cIterator insert_after(const cIterator &pos, Size size, const Arg &target);

        Iterator insert_after(const Iterator &pos, std::initializer_list<Arg> list);

        cIterator insert_after(const cIterator &pos, std::initializer_list<Arg> list);

        Iterator insert_after(const Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        cIterator insert_after(const cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert_after(const Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        cIterator insert_after(const cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        Iterator erase_after(const Iterator &pos);

        Iterator erase_after(const Iterator &begin, const Iterator &end);

        cIterator erase_after(const cIterator &pos);

        cIterator erase_after(const cIterator &begin, const cIterator &end);
    };

}

#endif //TINYSTL_FORWARD_LIST_HPP
