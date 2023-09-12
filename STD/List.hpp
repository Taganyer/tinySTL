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
            Arg value;

            Node *last = nullptr, *next = nullptr;

            Node(const Arg &target) : value(target) {};

            explicit Node(Arg &&target) : value(move(target)) {};

            Node(const Arg &target, Node *last, Node *next) : value(target), last(last), next(next) {};

            Node(Arg &&target, Node *last, Node *next) : value(move(target)), last(last), next(next) {};

            Node *copy() { return Allocate<Node>(value); }

        };

        Size size_ = 0;

        Node *val_begin = nullptr, *val_end = nullptr;

        Size release(Node *begin, Node *end);

    public:

        class Iterator;

        class cIterator;

        class rIterator;

        class crIterator;

        List() = default;

        List(Size size, Arg target = Arg());

        List(std::initializer_list<Arg> list);

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

        Iterator insert(const Iterator &pos, const Arg &target);

        cIterator insert(const cIterator &pos, const Arg &target);

        Iterator insert(const Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        cIterator insert(const cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert(const Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        cIterator insert(const cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        Arg &front() {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'List::front' function");
            return val_begin->value;
        };

        Arg &back() {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'List::back' function");
            return val_end->value;
        };

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

        Iterator begin() { return Iterator(val_begin); };

        Iterator end() { return Iterator(nullptr); };

        cIterator cbegin() { return cIterator(val_begin); };

        cIterator cend() { return cIterator(nullptr); };

        rIterator rbegin() { return rIterator(val_end); };

        rIterator rend() { return rIterator(nullptr); };

        crIterator crbegin() { return crIterator(val_end); };

        crIterator crend() { return crIterator(nullptr); };

    };


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    Size List<Arg>::release(List<Arg>::Node *begin, List<Arg>::Node *end) {
        if (!begin) return 0;
        Size count = 0;
        Node *prv = begin->last, *temp;
        while (begin != end) {
            temp = begin;
            begin = begin->next;
            Deallocate(temp);
            ++count;
        }
        if (prv) prv->next = end;
        if (end) end->last = prv;
        return count;
    }

    template<typename Arg>
    List<Arg>::List(Size size, Arg target) : size_(size) {
        Node *last = nullptr, *now = nullptr;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(target, last, nullptr);
            if (last) last->next = now;
            else val_begin = now;
            last = now;
        }
        val_end = now;
    }

    template<typename Arg>
    List<Arg>::List(std::initializer_list<Arg> list) : size_(list.size()) {
        if (size_) {
            auto temp = const_cast<Arg *>(list.begin());
            Node *last = nullptr, *now = nullptr;
            for (int i = 0; i < size_; ++i) {
                now = Allocate<List<Arg>::Node>(*temp, last, nullptr);
                if (last) last->next = now;
                else val_begin = now;
                ++temp;
                last = now;
            }
            val_end = now;
        }
    }

    template<typename Arg>
    List<Arg>::List(const Iter<Arg> &begin, const Iter<Arg> &end) {
        auto temp = begin.deep_copy();
        Node *last = nullptr, *now = nullptr;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else val_begin = now;
            last = now;
            ++(*temp), ++size_;
        }
        val_end = now;
    }

    template<typename Arg>
    List<Arg>::List(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto temp = begin.deep_copy();
        Node *last = nullptr, *now = nullptr;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else val_begin = now;
            last = now;
            ++(*temp), ++size_;
        }
        val_end = now;
    }

    template<typename Arg>
    List<Arg>::List(const List<Arg> &other) : size_(other.size_) {
        Node *last = nullptr, *now = nullptr, *temp = other.val_begin;
        while (temp) {
            now = temp->copy();
            now->last = last;
            if (last) last->next = now;
            else val_begin = now;
            last = now;
            temp = temp->next;
        }
        val_end = now;
    }

    template<typename Arg>
    List<Arg>::List(List<Arg> &&other) noexcept : size_(other.size_), val_begin(other.val_begin),
                                                  val_end(other.val_end) {
        other.size_ = 0;
        other.val_begin = other.val_end = nullptr;
    }

    template<typename Arg>
    List<Arg>::~List<Arg>() {
        clear();
    }

    template<typename Arg>
    void List<Arg>::clear() {
        release(val_begin, nullptr);
        size_ = 0;
        val_begin = val_end = nullptr;
    }


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    List<Arg> &List<Arg>::assign(const Iter<Arg> &begin, const Iter<Arg> &end) {
        clear();
        auto temp = begin.deep_copy();
        Node *last = nullptr, *now = nullptr;
        while (*temp != end) {
            now = Allocate<Node>(**temp);
            if (last) last->next = now;
            else val_begin = now;
            last = now;
            ++(*temp), ++size_;
        }
        val_end = now;
    }

    template<typename Arg>
    List<Arg> &List<Arg>::assign(const cIter<Arg> &begin, const cIter<Arg> &end) {
        clear();
        auto temp = begin.deep_copy();
        Node *last = nullptr, *now = nullptr;
        while (*temp != end) {
            now = Allocate<Node>(**temp);
            if (last) last->next = now;
            else val_begin = now;
            last = now;
            ++(*temp), ++size_;
        }
        val_end = now;
    }

    template<typename Arg>
    template<typename... args>
    void List<Arg>::emplace_front(args &&... vals) {
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), nullptr, val_begin);
        if (++size_ == 1) val_end = temp;
        else val_begin->last = temp;
        val_begin = temp;
    }

    template<typename Arg>
    void List<Arg>::push_front(const Arg &val) {
        auto temp = Allocate<Node>(val, nullptr, val_begin);
        if (++size_ == 1) val_end = temp;
        else val_begin->last = temp;
        val_begin = temp;
    }

    template<typename Arg>
    void List<Arg>::push_front(const Arg &val, Size size) {
        if (!size) return;
        Node *last = nullptr, *now = nullptr, *head = nullptr;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(val, last, nullptr);
            if (last) last->next = now;
            else head = now;
            last = now;
        }
        now->next = val_begin;
        size_ += size;
        if (val_begin) val_begin->last = now;
        else val_end = now;
        val_begin = head;
    }

    template<typename Arg>
    void List<Arg>::push_front(const Iter<Arg> &begin, const Iter<Arg> &end) {
        if (begin == end) return;
        auto temp = begin.deep_copy();
        Node *last = nullptr, *now = nullptr, *head = nullptr;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else head = now;
            last = now;
            ++(*temp), ++size_;
        }
        now->next = val_begin;
        if (val_begin) val_begin->last = now;
        else val_end = now;
        val_begin = head;
    }

    template<typename Arg>
    void List<Arg>::push_front(const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (begin == end) return;
        auto temp = begin.deep_copy();
        Node *last = nullptr, *now = nullptr, *head = nullptr;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else head = now;
            last = now;
            ++(*temp), ++size_;
        }
        now->next = val_begin;
        if (val_begin) val_begin->last = now;
        else val_end = now;
        val_begin = head;
    }

    template<typename Arg>
    void List<Arg>::pop_front() {
        if (val_begin) {
            Node *temp = val_begin;
            val_begin = val_begin->next;
            if (!--size_) val_end = nullptr;
            else val_begin->last = nullptr;
            Deallocate(temp);
        }
    }

    template<typename Arg>
    template<typename... args>
    void List<Arg>::emplace_back(args &&... vals) {
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), val_end, nullptr);
        if (++size_ == 1) val_begin = temp;
        else val_end->next = temp;
        val_end = temp;
    }

    template<typename Arg>
    void List<Arg>::push_back(const Arg &val) {
        auto temp = Allocate<Node>(val, val_end, nullptr);
        if (++size_ == 1) val_begin = temp;
        else val_end->next = temp;
        val_end = temp;
    }

    template<typename Arg>
    void List<Arg>::push_back(const Arg &val, Size size) {
        if (!size) return;
        Node *last = val_end, *now = nullptr, *head = nullptr;
        for (int i = 0; i < size; ++i) {
            now = Allocate<Node>(val, last, nullptr);
            if (last) last->next = now;
            else head = now;
            last = now;
        }
        if (val_end) val_end->next = head;
        else val_begin = head;
        val_end = now;
    }

    template<typename Arg>
    void List<Arg>::push_back(const Iter<Arg> &begin, const Iter<Arg> &end) {
        if (begin == end) return;
        auto temp = begin.deep_copy();
        Node *last = val_end, *now = nullptr, *head = nullptr;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else head = now;
            last = now;
            ++(*temp), ++size_;
        }
        if (val_end) val_end->next = head;
        else val_begin = head;
        val_end = head;
    }

    template<typename Arg>
    void List<Arg>::push_back(const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (begin == end) return;
        auto temp = begin.deep_copy();
        Node *last = val_end, *now = nullptr, *head = nullptr;
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else head = now;
            last = now;
            ++(*temp), ++size_;
        }
        if (val_end) val_end->next = head;
        else val_begin = head;
        val_end = head;
    }

    template<typename Arg>
    void List<Arg>::pop_back() {
        if (val_end) {
            Node *temp = val_end;
            val_end = val_end->last;
            if (!--size_) val_begin = nullptr;
            else val_end->next = nullptr;
            Deallocate(temp);
        }
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::Iterator List<Arg>::emplace(const List::Iterator &pos, args &&... vals) {
        if (!pos.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), pos.node->last, pos.node);
        if (pos.node->last) {
            pos.node->last->next = temp;
        } else {
            val_begin = temp;
            if (!size_) val_end = temp;
        }
        pos.node->last = temp;
        ++size_;
        return List::Iterator(temp);
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::cIterator List<Arg>::emplace(const List::cIterator &pos, args &&... vals) {
        if (!pos.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), pos.node->last, pos.node);
        if (pos.node->last) {
            pos.node->last->next = temp;
        } else {
            val_begin = temp;
            if (!size_) val_end = temp;
        }
        pos.node->last = temp;
        ++size_;
        return List::Iterator(temp);
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::insert(const List::Iterator &pos, const Arg &target) {
        if (!pos.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        auto temp = Allocate<Node>(target, pos.node->last, pos.node);
        if (pos.node->last) {
            pos.node->last->next = temp;
        } else {
            val_begin = temp;
            if (!size_) val_end = temp;
        }
        pos.node->last = temp;
        ++size_;
        return List<Arg>::Iterator(temp);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::insert(const List<Arg>::cIterator &pos, const Arg &target) {
        if (!pos.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        auto temp = Allocate<Node>(target, pos.node->last, pos.node);
        if (pos.node->last) {
            pos.node->last->next = temp;
        } else {
            val_begin = temp;
            if (!size_) val_end = temp;
        }
        pos.node->last = temp;
        ++size_;
        return List<Arg>::cIterator(temp);
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert(const List<Arg>::Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        if (begin == end) return List<Arg>::Iterator(begin.node);
        if (!pos.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        Node *last = nullptr, *now = nullptr, *head;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else head = now;
            now->last = last;
            last = now;
            ++size_;
        }
        if (pos.node->last) {
            head->last = pos.node->last;
            pos.node->last->next = head;
        } else {
            val_begin = head;
        }
        now->next = pos.node;
        pos.node->last = now;
        return List<Arg>::Iterator(head);
    }

    template<typename Arg>
    typename List<Arg>::cIterator
    List<Arg>::insert(const List<Arg>::cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        if (begin == end) return List<Arg>::cIterator(begin.node);
        if (!pos.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        Node *last = nullptr, *now = nullptr, *head;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else head = now;
            now->last = last;
            last = now;
            ++size_;
        }
        if (pos.node->last) {
            head->last = pos.node->last;
            pos.node->last->next = head;
        } else {
            val_begin = head;
        }
        now->next = pos.node;
        pos.node->last = now;
        return List<Arg>::cIterator(head);
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert(const List<Arg>::Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (begin == end) return List<Arg>::Iterator(begin.node);
        if (!pos.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        Node *last = nullptr, *now = nullptr, *head;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else head = now;
            now->last = last;
            last = now;
            ++size_;
        }
        if (pos.node->last) {
            head->last = pos.node->last;
            pos.node->last->next = head;
        } else {
            val_begin = head;
        }
        now->next = pos.node;
        pos.node->last = now;
        return List<Arg>::Iterator(head);
    }

    template<typename Arg>
    typename List<Arg>::cIterator
    List<Arg>::insert(const List<Arg>::cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (begin == end) return List<Arg>::cIterator(begin.node);
        if (!pos.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        Node *last = nullptr, *now = nullptr, *head;
        auto temp = begin.deep_copy();
        while (*temp != end) {
            now = Allocate<Node>(**temp, last, nullptr);
            if (last) last->next = now;
            else head = now;
            now->last = last;
            last = now;
            ++size_;
        }
        if (pos.node->last) {
            head->last = pos.node->last;
            pos.node->last->next = head;
        } else {
            val_begin = head;
        }
        now->next = pos.node;
        pos.node->last = now;
        return List<Arg>::cIterator(head);
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::erase(const List<Arg>::Iterator &iter) {
        if (!iter.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        auto temp = iter.node;
        if (temp->last) temp->last = temp->next;
        if (temp->next) temp->next = temp->last;
        Deallocate(temp);
        return List<Arg>::Iterator(temp->next);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::erase(const List<Arg>::cIterator &iter) {
        if (!iter.node) throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        auto temp = iter.node;
        if (temp->last) temp->last = temp->next;
        if (temp->next) temp->next = temp->last;
        Deallocate(temp);
        return List<Arg>::cIterator(temp->next);
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::erase(const List<Arg>::Iterator &begin, const List<Arg>::Iterator &end) {
        auto last = begin.node->last;
        auto temp = static_pointer_cast<List<Arg>::Iterator>(begin.deep_copy());
        while (*temp != end) {
            auto t = temp->node;
            if (!t) break;
            ++(*temp), --size_;
            Deallocate(t);
        }
        if (last) last->next = temp->node;
        else val_begin = temp->node;
        if (temp->node) temp->node->last = last;
        else val_end = last;
        return List<Arg>::Iterator(temp->node);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::erase(const List<Arg>::cIterator &begin, const List<Arg>::cIterator &end) {
        auto last = begin.node->last;
        auto temp = static_pointer_cast<List<Arg>::cIterator>(begin.deep_copy());
        while (*temp != end) {
            auto t = temp->node;
            if (!t) break;
            ++(*temp), --size_;
            Deallocate(t);
        }
        if (last) last->next = temp->node;
        else val_begin = temp->node;
        if (temp->node) temp->node->last = last;
        else val_end = last;
        return List<Arg>::cIterator(temp->node);
    }

    template<typename Arg>
    List<Arg> &List<Arg>::operator=(const List<Arg> &other) {
        clear();
        size_ = other.size_;
        Node *last = nullptr, *now = nullptr, *temp = other.val_begin;
        while (temp) {
            now = temp->copy();
            now->last = last;
            if (last) last->next = now;
            else val_begin = now;
            last = now;
            temp = temp->next;
        }
        val_end = now;
        return *this;
    }

    template<typename Arg>
    List<Arg> &List<Arg>::operator=(List<Arg> &&other) noexcept {
        clear();
        size_ = other.size_;
        val_begin = other.val_begin;
        val_end = other.val_end;
        other.size_ = 0;
        other.val_begin = other.val_end = nullptr;
        return *this;
    }

    template<typename Type>
    bool operator==(const List<Type> &left, const List<Type> &right) {
        if (left.size_ != right.size_) return false;
        auto l = left.val_begin, r = right.val_begin;
        while (l != nullptr) {
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
        auto l = left.val_begin, r = right.val_begin;
        while (l && r) {
            if (l->value != r->value) return l->value < r->value;
            l = l->next, r = r->next;
        }
        return !l && r;
    }

    template<typename Type>
    bool operator<=(const List<Type> &left, const List<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l && r) {
            if (l->value != r->value) return l->value < r->value;
            l = l->next, r = r->next;
        }
        return !l;
    }

    template<typename Type>
    bool operator>(const List<Type> &left, const List<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l && r) {
            if (l->value != r->value) return l->value > r->value;
            l = l->next, r = r->next;
        }
        return !r && l;
    }

    template<typename Type>
    bool operator>=(const List<Type> &left, const List<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l && r) {
            if (l->value != r->value) return l->value > r->value;
            l = l->next, r = r->next;
        }
        return !r;
    }


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    class List<Arg>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        List<Arg>::Node *node = nullptr;

        virtual Iterator &operator=(List<Arg>::Node *ptr) {
            if (ptr) target = &(ptr->value);
            else target = nullptr;
            node = ptr;
            return *this;
        };

        explicit Iterator(List<Arg>::Node *ptr) : Iter<Arg>(ptr ? &(ptr->value) : nullptr), node(ptr) {};

    public:
        friend class List<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<List<Arg>::Iterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override { return make_shared<cIter<Arg>>(cIterator(node)); };

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
            if (!node) throw outOfRange("List::Iterator out of range\n");
            node = node->next;
            if (node) target = &(node->value);
            else target = nullptr;
            return *this;
        };

        Iterator operator++(int) {
            if (!node) throw outOfRange("List::Iterator out of range\n");
            auto temp = node;
            node = node->next;
            if (node) target = &(node->value);
            else target = nullptr;
            return List<Arg>::Iterator(temp);
        };

        virtual Iterator &operator--() {
            if (!node) throw outOfRange("List::Iterator out of range\n");
            node = node->last;
            if (node) target = &(node->value);
            else target = nullptr;
            return *this;
        };

        Iterator operator--(int) {
            if (!node) throw outOfRange("List::Iterator out of range\n");
            auto temp = node;
            node = node->last;
            if (node) target = &(node->value);
            else target = nullptr;
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
            if (ptr) target = &(ptr->value);
            else target = nullptr;
            node = ptr;
            return *this;
        };

        explicit cIterator(List<Arg>::Node *ptr) : cIter<Arg>(ptr ? &(ptr->value) : nullptr), node(ptr) {};

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
            if (!node) throw outOfRange("List::cIterator out of range\n");
            node = node->next;
            if (node) target = &(node->value);
            else target = nullptr;
            return *this;
        };

        cIterator operator++(int) {
            if (!node) throw outOfRange("List::cIterator out of range\n");
            auto temp = node;
            node = node->next;
            if (node) target = &(node->value);
            else target = nullptr;
            return List<Arg>::cIterator(temp);
        };

        virtual cIterator &operator--() {
            if (!node) throw outOfRange("List::cIterator out of range\n");
            node = node->last;
            if (node) target = &(node->value);
            else target = nullptr;
            return *this;
        };

        cIterator operator--(int) {
            if (!node) throw outOfRange("List::cIterator out of range\n");
            auto temp = node;
            node = node->last;
            if (node) target = &(node->value);
            else target = nullptr;
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
            if (ptr) target = &(ptr->value);
            else target = nullptr;
            node = ptr;
            return *this;
        };

        explicit rIterator(List<Arg>::Node *ptr) : Iter<Arg>(ptr ? &(ptr->value) : nullptr), node(ptr) {};

    public:
        friend class List<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<List<Arg>::rIterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override { return make_shared<cIter<Arg>>(crIterator(node)); };

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
            if (!node) throw outOfRange("List::rIterator out of range\n");
            node = node->last;
            if (node) target = &(node->value);
            else target = nullptr;
            return *this;
        };

        rIterator operator++(int) {
            if (!node) throw outOfRange("List::rIterator out of range\n");
            auto temp = node;
            node = node->last;
            if (node) target = &(node->value);
            else target = nullptr;
            return List<Arg>::rIterator(temp);
        };

        rIterator &operator--() {
            if (!node) throw outOfRange("List::rIterator out of range\n");
            node = node->next;
            if (node) target = &(node->value);
            else target = nullptr;
            return *this;
        };

        rIterator operator--(int) {
            if (!node) throw outOfRange("List::rIterator out of range\n");
            auto temp = node;
            node = node->next;
            if (node) target = &(node->value);
            else target = nullptr;
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
            if (ptr) target = &(ptr->value);
            else target = nullptr;
            node = ptr;
            return *this;
        };

        explicit crIterator(List<Arg>::Node *ptr) : cIter<Arg>(ptr ? &(ptr->value) : nullptr), node(ptr) {};

    public:
        friend class List<Arg>;

        friend class List<Arg>::rIterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<List<Arg>::crIterator>(*this); };

        crIterator(const crIterator &other) : List<Arg>::cIterator(other.node) {};

        ~crIterator() = default;

        crIterator &operator=(const crIterator &other) {
            target = other.target;
            node = other.node;
            return *this;
        };


        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        crIterator &operator++() override {
            if (!node) throw outOfRange("List::crIterator out of range\n");
            node = node->last;
            if (node) target = &(node->value);
            else target = nullptr;
            return *this;
        };

        crIterator operator++(int) {
            if (!node) throw outOfRange("List::crIterator out of range\n");
            auto temp = node;
            node = node->last;
            if (node) target = &(node->value);
            else target = nullptr;
            return List<Arg>::crIterator(temp);
        };

        crIterator &operator--() {
            if (!node) throw outOfRange("List::crIterator out of range\n");
            node = node->next;
            if (node) target = &(node->value);
            else target = nullptr;
            return *this;
        };

        crIterator operator--(int) {
            if (!node) throw outOfRange("List::crIterator out of range\n");
            auto temp = node;
            node = node->next;
            if (node) target = &(node->value);
            else target = nullptr;
            return List<Arg>::crIterator(temp);
        };

        friend bool
        operator==(const crIterator &left, const crIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const crIterator &left, const crIterator &right) { return left.target != right.target; };
    };

}

#endif //TINYSTL_LIST_HPP
