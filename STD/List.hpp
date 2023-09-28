//
// Created by 86152 on 2023/9/11.
//

#ifndef TINYSTL_LIST_HPP
#define TINYSTL_LIST_HPP

#include "Iterator.hpp"
#include "Allocater.hpp"
#include <initializer_list>

namespace STD {

    template<typename Arg>
    class List;

    namespace Detail {

        template<typename Arg>
        struct List_Iterator : public Iterator<Bidirectional_iterator_tag, Arg> {
        public:
            using Basic = Iterator<Bidirectional_iterator_tag, Arg>;

            using Self = List_Iterator<Arg>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            using Node = typename List<Arg>::Node;

            friend class List<Arg>;

            friend class rIterator<Self, Random_iterator_tag>;

            friend class cIterator<Self, Random_iterator_tag>;

            friend class crIterator<Self, Random_iterator_tag>;

        private:
            Node *target;

            explicit List_Iterator(Node *target) : target(target) {};

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

            Self &operator--() {
                target = target->last;
                return *this;
            };

            Self operator--(int) {
                Self temp = Self(target);
                target = target->last;
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
    class List {
    public:
        friend class Detail::List_Iterator<Arg>;

        using Iterator = Detail::List_Iterator<Arg>;

        using cIterator = STD::cIterator<Iterator, Bidirectional_iterator_tag>;

        using rIterator = STD::rIterator<Iterator, Bidirectional_iterator_tag>;

        using crIterator = STD::crIterator<Iterator, Bidirectional_iterator_tag>;

        List() {
            val_begin->next = val_end;
            val_end->last = val_begin;
        };

        List(Size size, Arg target = Arg());

        List(const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        List(Input_iterator begin, const Input_iterator &end);

        List(const List<Arg> &other);

        List(List<Arg> &&other) noexcept;

        ~List<Arg>();

        Size size() const { return size_; };

        Size capacity() const { return size_; };

        void clear();

        template<typename Input_iterator>
        List<Arg> &assign(Input_iterator begin, const Input_iterator &end);

        List<Arg> &assign(const std::initializer_list<Arg> &list);

        template<typename ...args>
        void emplace_front(args &&...vals);

        void push_front(const Arg &val);

        void push_front(Arg &&val);

        void push_front(const Arg &val, Size size);

        template<typename Input_iterator>
        void push_front(Input_iterator begin, const Input_iterator &end);

        void pop_front();

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(const Arg &val);

        void push_back(Arg &&val);

        void push_back(const Arg &val, Size size);

        template<typename Input_iterator>
        void push_back(Input_iterator begin, const Input_iterator &end);

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

        template<typename Input_iterator>
        Iterator insert(const Iterator &pos, Input_iterator begin, const Input_iterator &end);

        template<typename Input_iterator>
        cIterator insert(const cIterator &pos, Input_iterator begin, const Input_iterator &end);

        rIterator insert(const rIterator &pos, const Arg &target);

        crIterator insert(const crIterator &pos, const Arg &target);

        rIterator insert(const rIterator &pos, const std::initializer_list<Arg> &list);

        crIterator insert(const crIterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        rIterator insert(const rIterator &pos, Input_iterator begin, const Input_iterator &end);

        template<typename Input_iterator>
        crIterator insert(const crIterator &pos, Input_iterator begin, const Input_iterator &end);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        rIterator erase(const rIterator &iter);

        crIterator erase(const crIterator &iter);

        rIterator erase(const rIterator &begin, const rIterator &end);

        crIterator erase(const crIterator &begin, const crIterator &end);

        Arg &front() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'List::front' function");
            return *(val_begin->next->value);
        };

        Arg &back() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'List::back' function");
            return *(val_end->last->value);
        };

        bool empty() const { return !size_; };

        void swap(List<Arg> &other) noexcept {
            Size temp1 = size_;
            Node *temp2 = val_begin, *temp3 = val_end;
            size_ = other.size_;
            val_begin = other.val_begin;
            val_end = other.val_end;
            other.size_ = temp1;
            other.val_begin = temp2;
            other.val_end = temp3;
        }

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

        template<typename Type>
        friend inline void swap(List<Arg> &left, List<Arg> &right) noexcept;

        Iterator begin() const { return Iterator(val_begin->next); };

        Iterator end() const { return Iterator(val_end); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin->next)); };

        cIterator cend() const { return cIterator(Iterator(val_end)); };

        rIterator rbegin() const { return rIterator(Iterator(val_end->last)); };

        rIterator rend() const { return rIterator(Iterator(val_begin)); };

        crIterator crbegin() const { return crIterator(Iterator(val_end->last)); };

        crIterator crend() const { return crIterator(Iterator(val_begin)); };

    private:
        struct Node {
            Arg *value;

            Node *last = nullptr, *next = nullptr;

            explicit Node(Arg &&target) : value(Allocate<Arg>(move(target))) {};

            Node(const Arg &target, Node *last, Node *next) : value(Allocate<Arg>(target)), last(last), next(next) {};

            Node(Arg &&target, Node *last, Node *next) : value(Allocate<Arg>(move(target))), last(last), next(next) {};

            Node(Arg *value, Node *last, Node *next) : value(value), last(last), next(next) {};

            ~Node() { Deallocate(value); };

            Node *copy() { return Allocate<Node>(*value); }

        };

        Size size_ = 0;

        Node *val_begin = Allocate<Node>(nullptr, nullptr, nullptr),
                *val_end = Allocate<Node>(nullptr, val_begin, nullptr);

        void release(Node *begin, Node *end);

        Iterator insert_Helper(Node *location, Step size, Arg target, bool reverse);

        Iterator insert_Helper(Node *location, const std::initializer_list<Arg> &list, bool reverse);

        template<typename Input_iterator>
        Iterator insert_Helper(Node *location, Input_iterator begin,
                               const Input_iterator &end, bool reverse, False_type);

        template<typename Input_iterator>
        Iterator insert_Helper(Node *location, Input_iterator size,
                               const Input_iterator &target, bool reverse, True_type);

    };


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    void List<Arg>::release(Node *begin, Node *end) {
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
        size_ -= count;
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert_Helper(Node *location,
                             Step size, Arg target, bool reverse) {
        Node *record = location;
        size_ += size;
        if (reverse) {
            Node *next = location->next, *now = location->next;
            while (size > 0) {
                now = Allocate<Node>(target, nullptr, next);
                if (record == location) record = now;
                next->last = now;
                next = now;
                --size;
            }
            location->next = now;
            now->last = location;
        } else {
            Node *last = location->last, *now = location->last;
            while (size > 0) {
                now = Allocate<Node>(target, last, nullptr);
                if (record == location) record = now;
                last->next = now;
                last = now;
                --size;
            }
            location->last = now;
            now->next = location;
        }
        return Iterator(record);
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert_Helper(Node *location,
                             const std::initializer_list<Arg> &list, bool reverse) {
        Node *record = location;
        Size size = list.size();
        size_ += size;
        auto target = list.begin();
        if (reverse) {
            Node *next = location->next, *now = location->next;
            while (size > 0) {
                now = Allocate<Node>(*target, nullptr, next);
                if (record == location) record = now;
                next->last = now;
                next = now;
                --size, ++target;
            }
            location->next = now;
            now->last = location;
        } else {
            Node *last = location->last, *now = location->last;
            while (size > 0) {
                now = Allocate<Node>(*target, last, nullptr);
                if (record == location) record = now;
                last->next = now;
                last = now;
                --size, ++target;
            }
            location->last = now;
            now->next = location;
        }
        return Iterator(record);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::Iterator
    List<Arg>::insert_Helper(Node *location, Input_iterator begin,
                             const Input_iterator &end, bool reverse, False_type) {
        Node *record = location;
        if (reverse) {
            Node *next = location->next, *now = location->next;
            while (begin != end) {
                now = Allocate<Node>(*begin, nullptr, next);
                if (record == location) record = now;
                next->last = now;
                next = now;
                ++size_, ++begin;
            }
            location->next = now;
            now->last = location;
        } else {
            Node *last = location->last, *now = location->last;
            while (begin != end) {
                now = Allocate<Node>(*begin, last, nullptr);
                if (record == location) record = now;
                last->next = now;
                last = now;
                ++size_, ++begin;
            }
            location->last = now;
            now->next = location;
        }
        return Iterator(record);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::Iterator
    List<Arg>::insert_Helper(Node *location, Input_iterator size,
                             const Input_iterator &target, bool reverse, True_type) {
        return insert_Helper(location, size, target, reverse);
    }

    template<typename Arg>
    List<Arg>::List(Size size, Arg target) {
        val_begin->next = val_end;
        insert_Helper(val_end, size, target, false);
    }

    template<typename Arg>
    List<Arg>::List(const std::initializer_list<Arg> &list) {
        val_begin->next = val_end;
        insert_Helper(val_end, list, false);
    }

    template<typename Arg>
    template<typename Input_iterator>
    List<Arg>::List(Input_iterator begin, const Input_iterator &end) {
        val_begin->next = val_end;
        insert_Helper(val_end, begin, end, false, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    List<Arg>::List(const List<Arg> &other) {
        val_begin->next = val_end;
        insert_Helper(val_end, other.begin(), other.end(), false, False_type());
    }

    template<typename Arg>
    List<Arg>::List(List<Arg> &&other) noexcept :
            size_(other.size_), val_begin(other.val_begin),
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
    }


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    List<Arg> &List<Arg>::assign(const std::initializer_list<Arg> &list) {
        clear();
        insert_Helper(val_end, list, false);
        return *this;
    }

    template<typename Arg>
    template<typename Input_iterator>
    List<Arg> &List<Arg>::assign(Input_iterator begin, const Input_iterator &end) {
        clear();
        insert_Helper(val_end, begin, end, false);
        return *this;
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
        ++size_;
        val_begin->next = Allocate<Node>(val, val_begin, val_begin->next);
        val_begin->next->next->last = val_begin->next;
    }

    template<typename Arg>
    void List<Arg>::push_front(Arg &&val) {
        ++size_;
        val_begin->next = Allocate<Node>(move(val), val_begin, val_begin->next);
        val_begin->next->next->last = val_begin->next;
    }

    template<typename Arg>
    void List<Arg>::push_front(const Arg &val, Size size) {
        insert_Helper(val_begin->next, size, val, false);
    }

    template<typename Arg>
    template<typename Input_iterator>
    void List<Arg>::push_front(Input_iterator begin, const Input_iterator &end) {
        insert_Helper(val_begin->next, begin, end, false);
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
        ++size_;
        val_end->last = Allocate<Node>(val, val_end->last, val_end);
        val_end->last->last->next = val_end->last;
    }

    template<typename Arg>
    void List<Arg>::push_back(Arg &&val) {
        ++size_;
        val_end->last = Allocate<Node>(move(val), val_end->last, val_end);
        val_end->last->last->next = val_end->last;
    }

    template<typename Arg>
    void List<Arg>::push_back(const Arg &val, Size size) {
        insert_Helper(val_end, size, val, false);
    }

    template<typename Arg>
    template<typename Input_iterator>
    void List<Arg>::push_back(Input_iterator begin, const Input_iterator &end) {
        insert_Helper(val_end, begin, end, false);
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
    typename List<Arg>::Iterator List<Arg>::emplace(const Iterator &pos, args &&... vals) {
        if (!pos.target || !pos.target->last)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), pos.target->last, pos.target);
        pos.target->last->next = temp;
        pos.target->last = temp;
        ++size_;
        return Iterator(temp);
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::cIterator List<Arg>::emplace(const cIterator &pos, args &&... vals) {
        return cIterator(emplace(pos.target, forward<args>(vals)...));
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::rIterator List<Arg>::emplace(const rIterator &pos, args &&... vals) {
        return rIterator(emplace((--pos).target, forward<args>(vals)...));
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::crIterator List<Arg>::emplace(const crIterator &pos, args &&... vals) {
        return crIterator(emplace((--pos).target, forward<args>(vals)...));
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::insert(const Iterator &pos, const Arg &target) {
        if (!pos.target || !pos.target->last)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::emplace' function");
        auto temp = Allocate<List<Arg>::Node>(target, pos.target->last, pos.target);
        pos.target->last->next = temp;
        pos.target->last = temp;
        ++size_;
        return Iterator(temp);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::insert(const cIterator &pos, const Arg &target) {
        return cIterator(insert(pos.target, target));
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert(const Iterator &pos, const std::initializer_list<Arg> &list) {
        if (!pos.target || !pos.target->last)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        return insert_Helper(pos.target, list, false);
    }

    template<typename Arg>
    typename List<Arg>::cIterator
    List<Arg>::insert(const cIterator &pos, const std::initializer_list<Arg> &list) {
        return cIterator(insert(pos.target, list));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::Iterator
    List<Arg>::insert(const Iterator &pos,
                      Input_iterator begin, const Input_iterator &end) {
        if (!pos.target || !pos.target->last)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        return insert_Helper(pos.target, begin, end, false, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::cIterator
    List<Arg>::insert(const cIterator &pos,
                      Input_iterator begin, const Input_iterator &end) {
        return cIterator(insert(pos.target, begin, end));
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::insert(const rIterator &pos, const Arg &target) {
        return rIterator(insert((--pos).target, target));
    }

    template<typename Arg>
    typename List<Arg>::crIterator List<Arg>::insert(const crIterator &pos, const Arg &target) {
        return crIterator(insert((--pos).target, target));
    }

    template<typename Arg>
    typename List<Arg>::rIterator
    List<Arg>::insert(const rIterator &pos, const std::initializer_list<Arg> &list) {
        if (!pos.target.target || !pos.target.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        return rIterator(insert_Helper(pos.target.target, list, true));
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::insert(const crIterator &pos, const std::initializer_list<Arg> &list) {
        if (!pos.target.target || !pos.target.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        return crIterator(insert_Helper(pos.target.target, list, true));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::rIterator
    List<Arg>::insert(const rIterator &pos,
                      Input_iterator begin, const Input_iterator &end) {
        if (!pos.target.target || !pos.target.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        return rIterator(insert_Helper(pos.target.target, begin, end, true, Is_integral<Input_iterator>()));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::crIterator
    List<Arg>::insert(const crIterator &pos,
                      Input_iterator begin, const Input_iterator &end) {
        if (!pos.target.target || !pos.target.target->next)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::insert' function");
        return crIterator(insert_Helper(pos.target.target, begin, end, true, Is_integral<Input_iterator>()));
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::erase(const Iterator &iter) {
        if (!iter.target || !iter.target->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        iter.target->last->next = iter.target->next;
        iter.target->next->last = iter.target->last;
        Node *record = iter.target->next;
        Deallocate(iter.target);
        return Iterator(record);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::erase(const cIterator &iter) {
        return cIterator(erase(iter));
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::erase(const Iterator &begin, const Iterator &end) {
        if (!begin.target || !begin.target->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        release(begin.target, end.target);
        return end;
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::erase(const cIterator &begin, const cIterator &end) {
        erase(begin.target, end.target);
        return end;
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::erase(const rIterator &iter) {
        return rIterator(--erase(iter.target));
    }

    template<typename Arg>
    typename List<Arg>::crIterator List<Arg>::erase(const crIterator &iter) {
        return crIterator(--erase(iter.target));
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::erase(const rIterator &begin, const rIterator &end) {
        if (!begin.target.target || !begin.target.target->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        release(end.target.target->next, begin.target.target->next);
        return end;
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::erase(const crIterator &begin, const crIterator &end) {
        if (!begin.target.target || !begin.target.target->value)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        release(end.target.target->next, begin.target.target->next);
        return end;
    }

    template<typename Arg>
    List<Arg> &List<Arg>::operator=(const List<Arg> &other) {
        if (this == &other) return *this;
        clear();
        insert_Helper(val_end, other.begin(), other.end(), false, False_type());
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
        return true;
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

    template<typename Type>
    inline void swap(List<Type> &left, List<Type> &right) noexcept {
        left.swap(right);
    }


//----------------------------------------------------------------------------------------------------------------------

}

#endif //TINYSTL_LIST_HPP
