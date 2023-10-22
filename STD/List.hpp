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

        /* 该迭代器end()和rend()和cend()和crend()指向的位置相同，都无法被解引用。这使得该迭代器
         * 的行为更像一个环形迭代器，带来隐患的同时，也可以进行一些特殊的操作。使用时要多加注意。
         */
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

            List<Arg> *container;

            explicit List_Iterator(Node *target, List<Arg> *container) : target(target), container(container) {};

        public:
            Reference operator*() const {
                return target->value;
            };

            Pointer operator->() const {
                return &(target->value);
            };

            Self &operator++() {
                if (target) target = target->next;
                else target = container->val_begin;
                return *this;
            }

            Self operator++(int) {
                Self temp = Self(target, container);
                ++(*this);
                return temp;
            };

            Self &operator--() {
                if (target) target = target->last;
                else target = container->val_end;
                return *this;
            };

            Self operator--(int) {
                Self temp = Self(target, target);
                --(*this);
                return temp;
            };

            friend bool operator==(const Self &left, const Self &right) {
                return left.target == right.target && left.container == right.container;
            };

            friend bool operator!=(const Self &left, const Self &right) {
                return left.target != right.target || left.container != right.container;
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

        using Self = List<Arg>;

        List() = default;

        List(Size size, Arg target = Arg());

        List(const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        List(Input_iterator begin, const Input_iterator &end);

        List(const List<Arg> &other);

        List(List<Arg> &&other) noexcept;

        ~List<Arg>();

    private:
        struct Node {
            Arg value;

            Node *last = nullptr, *next = nullptr;

            explicit Node(Arg &&target) : value(move(target)) {};

            Node(const Arg &target, Node *last, Node *next) : value(target), last(last), next(next) {};

            Node(Arg &&target, Node *last, Node *next) : value(move(target)), last(last), next(next) {};

        };

        Size size_ = 0;

        Node *val_begin = nullptr, *val_end = nullptr;

        void release(Node *begin, Node *end);

        Iterator insert_Helper(Node *location, Signed_Size size, Arg target, bool reverse);

        Iterator insert_Helper(Node *location, const std::initializer_list<Arg> &list, bool reverse);

        template<typename Input_iterator>
        Iterator insert_Helper(Node *location, Input_iterator begin,
                               const Input_iterator &end, bool reverse, False_type);

        template<typename Input_iterator>
        Iterator insert_Helper(Node *location, Input_iterator size,
                               const Input_iterator &target, bool reverse, True_type);

    public:
        Size size() const { return size_; };

        Size capacity() const { return size_; };

        void clear();

        List<Arg> &assign(Size size, const Arg &val);

        template<typename Input_iterator>
        List<Arg> &assign(Input_iterator begin, const Input_iterator &end);

        List<Arg> &assign(const std::initializer_list<Arg> &list);

        template<typename ...args>
        void emplace_front(args &&...vals);

        void push_front(const Arg &val);

        void push_front(Arg &&val);

        void push_front(Size size, const Arg &val);

        template<typename Input_iterator>
        void push_front(Input_iterator begin, const Input_iterator &end);

        void pop_front();

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(const Arg &val);

        void push_back(Arg &&val);

        void push_back(Size size, const Arg &val);

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

        Iterator emplace(const Iterator &pos, const Arg &target);

        cIterator emplace(const cIterator &pos, const Arg &target);

        rIterator emplace(const rIterator &pos, const Arg &target);

        crIterator emplace(const crIterator &pos, const Arg &target);

        Iterator insert(const Iterator &pos, const Arg &target);

        cIterator insert(const cIterator &pos, const Arg &target);

        Iterator insert(const Iterator &pos, Size size, const Arg &target);

        cIterator insert(const cIterator &pos, Size size, const Arg &target);

        Iterator insert(const Iterator &pos, const std::initializer_list<Arg> &list);

        cIterator insert(const cIterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Iterator insert(const Iterator &pos, Input_iterator begin, const Input_iterator &end);

        template<typename Input_iterator>
        cIterator insert(const cIterator &pos, Input_iterator begin, const Input_iterator &end);

        rIterator insert(const rIterator &pos, const Arg &target);

        crIterator insert(const crIterator &pos, const Arg &target);

        rIterator insert(const rIterator &pos, Size size, const Arg &target);

        crIterator insert(const crIterator &pos, Size size, const Arg &target);

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
            return val_begin->value;
        };

        Arg &back() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'List::back' function");
            return val_end->value;
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

        template<typename Type>
        friend inline void swap(List<Arg> &left, List<Arg> &right) noexcept;

        Iterator begin() const { return Iterator(val_begin, const_cast<Self *>(this)); };

        Iterator end() const { return Iterator(nullptr, const_cast<Self *>(this)); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin, const_cast<Self *>(this))); };

        cIterator cend() const { return cIterator(Iterator(nullptr, const_cast<Self *>(this))); };

        rIterator rbegin() const { return rIterator(Iterator(val_end, const_cast<Self *>(this))); };

        rIterator rend() const { return rIterator(Iterator(nullptr, const_cast<Self *>(this))); };

        crIterator crbegin() const { return crIterator(Iterator(val_end, const_cast<Self *>(this))); };

        crIterator crend() const { return crIterator(Iterator(nullptr, const_cast<Self *>(this))); };

    };


//----------------------------------------------------------------------------------------------------------------------


    template<typename Arg>
    void List<Arg>::release(Node *begin, Node *end) {
        if (!begin) return;
        Node *prv = begin->last, *temp;
        while (begin != end) {
            temp = begin;
            begin = begin->next;
            Deallocate(temp);
            --size_;
        }
        if (prv) prv->next = end;
        else val_begin = end;
        if (end) end->last = prv;
        else val_end = prv;
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert_Helper(Node *location,
                             Signed_Size size, Arg target, bool reverse) {
        if (!size) return Iterator(location, this);
        Node *record = location;
        size_ += size;
        if (reverse) {
            Node *next = location ? location->next : val_begin, *now = next;
            if (!next) {
                record = next = now = Allocate<Node>(target, nullptr, nullptr);
                val_end = now;
                --size;
            }
            while (size > 0) {
                now = Allocate<Node>(target, nullptr, next);
                if (record == location) record = now;
                next->last = now;
                next = now;
                --size;
            }
            if (location) location->next = now;
            else val_begin = now;
            now->last = location;
        } else {
            Node *last = location ? location->last : val_end, *now = last;
            if (!last) {
                record = last = now = Allocate<Node>(target, nullptr, nullptr);
                val_begin = now;
                --size;
            }
            while (size > 0) {
                now = Allocate<Node>(target, last, nullptr);
                if (record == location) record = now;
                last->next = now;
                last = now;
                --size;
            }
            if (location) location->last = now;
            else val_end = now;
            now->next = location;
        }
        return Iterator(record, this);
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert_Helper(Node *location,
                             const std::initializer_list<Arg> &list, bool reverse) {
        if (!list.size()) return Iterator(location, this);
        Node *record = location;
        Size size = list.size();
        size_ += size;
        auto target = list.begin();
        if (reverse) {
            Node *next = location ? location->next : val_begin, *now = next;
            if (!next) {
                record = next = now = Allocate<Node>(*target, nullptr, nullptr);
                val_end = now;
                --size, ++target;
            }
            while (size > 0) {
                now = Allocate<Node>(*target, nullptr, next);
                if (record == location) record = now;
                next->last = now;
                next = now;
                --size, ++target;
            }
            if (location) location->next = now;
            else val_begin = now;
            now->last = location;
        } else {
            Node *last = location ? location->last : val_end, *now = last;
            if (!last) {
                record = last = now = Allocate<Node>(*target, nullptr, nullptr);
                val_begin = now;
                --size, ++target;
            }
            while (size > 0) {
                now = Allocate<Node>(*target, last, nullptr);
                if (record == location) record = now;
                last->next = now;
                last = now;
                --size, ++target;
            }
            if (location) location->last = now;
            else val_end = now;
            now->next = location;
        }
        return Iterator(record, this);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::Iterator
    List<Arg>::insert_Helper(Node *location, Input_iterator begin,
                             const Input_iterator &end, bool reverse, False_type) {
        if (begin == end) return Iterator(location, this);
        Node *record = location;
        if (reverse) {
            Node *next = location ? location->next : val_begin, *now = next;
            if (!next) {
                record = next = now = Allocate<Node>(*begin, nullptr, nullptr);
                val_end = now;
                ++begin, ++size_;
            }
            while (begin != end) {
                now = Allocate<Node>(*begin, nullptr, next);
                if (record == location) record = now;
                next->last = now;
                next = now;
                ++begin, ++size_;
            }
            if (location) location->next = now;
            else val_begin = now;
            now->last = location;
        } else {
            Node *last = location ? location->last : val_end, *now = last;
            if (!last) {
                record = last = now = Allocate<Node>(*begin, nullptr, nullptr);
                val_begin = now;
                ++begin, ++size_;
            }
            while (begin != end) {
                now = Allocate<Node>(*begin, last, nullptr);
                if (record == location) record = now;
                last->next = now;
                last = now;
                ++begin, ++size_;
            }
            if (location) location->last = now;
            else val_end = now;
            now->next = location;
        }
        return Iterator(record, this);
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
        insert_Helper(val_end, size, target, false);
    }

    template<typename Arg>
    List<Arg>::List(const std::initializer_list<Arg> &list) {
        insert_Helper(val_end, list, false);
    }

    template<typename Arg>
    template<typename Input_iterator>
    List<Arg>::List(Input_iterator begin, const Input_iterator &end) {
        insert_Helper(val_end, begin, end, false, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    List<Arg>::List(const List<Arg> &other) {
        insert_Helper(val_end, other.begin(), other.end(), false, False_type());
    }

    template<typename Arg>
    List<Arg>::List(List<Arg> &&other) noexcept :
            size_(other.size_), val_begin(other.val_begin),
            val_end(other.val_end) {
        if (other.size_) {
            other.size_ = 0;
            other.val_begin = nullptr;
            other.val_end = nullptr;
        }
    }

    template<typename Arg>
    List<Arg>::~List<Arg>() {
        clear();
    }

    template<typename Arg>
    void List<Arg>::clear() {
        release(val_begin, nullptr);
    }


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    List<Arg> &List<Arg>::assign(Size size, const Arg &val) {
        clear();
        insert_Helper(nullptr, size, val, false);
        return *this;
    }

    template<typename Arg>
    List<Arg> &List<Arg>::assign(const std::initializer_list<Arg> &list) {
        clear();
        insert_Helper(nullptr, list, false);
        return *this;
    }

    template<typename Arg>
    template<typename Input_iterator>
    List<Arg> &List<Arg>::assign(Input_iterator begin, const Input_iterator &end) {
        clear();
        insert_Helper(nullptr, begin, end, false);
        return *this;
    }

    template<typename Arg>
    template<typename... args>
    void List<Arg>::emplace_front(args &&... vals) {
        ++size_;
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), nullptr, val_begin);
        if (val_begin) val_begin->last = temp;
        else val_end = temp;
        val_begin = temp;
    }

    template<typename Arg>
    void List<Arg>::push_front(const Arg &val) {
        ++size_;
        auto temp = Allocate<Node>(val, nullptr, val_begin);
        if (val_begin) val_begin->last = temp;
        else val_end = temp;
        val_begin = temp;
    }

    template<typename Arg>
    void List<Arg>::push_front(Arg &&val) {
        ++size_;
        auto temp = Allocate<Node>(move(val), nullptr, val_begin);
        if (val_begin) val_begin->last = temp;
        else val_end = temp;
        val_begin = temp;
    }

    template<typename Arg>
    void List<Arg>::push_front(Size size, const Arg &val) {
        insert_Helper(nullptr, size, val, false);
    }

    template<typename Arg>
    template<typename Input_iterator>
    void List<Arg>::push_front(Input_iterator begin, const Input_iterator &end) {
        insert_Helper(nullptr, begin, end, false);
    }

    template<typename Arg>
    void List<Arg>::pop_front() {
        if (size_) {
            auto temp = val_begin;
            val_begin = temp->next;
            if (val_begin) val_begin->last = nullptr;
            else val_end = nullptr;
            --size_;
            Deallocate(temp);
        }
    }

    template<typename Arg>
    template<typename... args>
    void List<Arg>::emplace_back(args &&... vals) {
        ++size_;
        auto temp = Allocate<Node>(Arg(forward<args>(vals)...), val_end, nullptr);
        if (val_end) val_end->next = temp;
        else val_begin = temp;
        val_end = temp;
    }

    template<typename Arg>
    void List<Arg>::push_back(const Arg &val) {
        ++size_;
        auto temp = Allocate<Node>(val, val_end, nullptr);
        if (val_end) val_end->next = temp;
        else val_begin = temp;
        val_end = temp;
    }

    template<typename Arg>
    void List<Arg>::push_back(Arg &&val) {
        ++size_;
        auto temp = Allocate<Node>(move(val), val_end, nullptr);
        if (val_end) val_end->next = temp;
        else val_begin = temp;
        val_end = temp;
    }

    template<typename Arg>
    void List<Arg>::push_back(Size size, const Arg &val) {
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
            auto temp = val_end;
            val_end = temp->last;
            if (val_end) val_end->next = nullptr;
            else val_begin = nullptr;
            --size_;
            Deallocate(temp);
        }
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::Iterator List<Arg>::emplace(const Iterator &pos, args &&... vals) {
        return insert_Helper(pos.target, 1, Arg(vals...), false);
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::cIterator List<Arg>::emplace(const cIterator &pos, args &&... vals) {
        return cIterator(insert_Helper(pos.target.target, 1, Arg(vals...), false));
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::rIterator List<Arg>::emplace(const rIterator &pos, args &&... vals) {
        return rIterator(insert_Helper(pos.target.target, 1, Arg(vals...), true));
    }

    template<typename Arg>
    template<typename... args>
    typename List<Arg>::crIterator List<Arg>::emplace(const crIterator &pos, args &&... vals) {
        return crIterator(insert_Helper(pos.target.target, 1, Arg(vals...), true));
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::emplace(const Iterator &pos, const Arg &target) {
        return insert_Helper(pos.target, 1, target, false);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::emplace(const cIterator &pos, const Arg &target) {
        return cIterator(insert_Helper(pos.target.target, 1, target, false));
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::emplace(const rIterator &pos, const Arg &target) {
        return rIterator(insert_Helper(pos.target.target, 1, target, true));
    }

    template<typename Arg>
    typename List<Arg>::crIterator List<Arg>::emplace(const crIterator &pos, const Arg &target) {
        return crIterator(insert_Helper(pos.target.target, 1, target, true));
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::insert(const Iterator &pos, const Arg &target) {
        return insert_Helper(pos.target, 1, target, false);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::insert(const cIterator &pos, const Arg &target) {
        return cIterator(insert_Helper(pos.target.target, 1, target, false));
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert(const Iterator &pos, const std::initializer_list<Arg> &list) {
        return insert_Helper(pos.target, list, false);
    }

    template<typename Arg>
    typename List<Arg>::cIterator
    List<Arg>::insert(const cIterator &pos, const std::initializer_list<Arg> &list) {
        return cIterator(insert_Helper(pos.target.target, list, false));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::Iterator
    List<Arg>::insert(const Iterator &pos,
                      Input_iterator begin, const Input_iterator &end) {
        return insert_Helper(pos.target, begin, end, false, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::cIterator
    List<Arg>::insert(const cIterator &pos,
                      Input_iterator begin, const Input_iterator &end) {
        return cIterator(insert_Helper(pos.target.target, begin, end, false, Is_integral<Input_iterator>()));
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::insert(const rIterator &pos, const Arg &target) {
        return rIterator(insert_Helper(pos.target.target, 1, target, true));
    }

    template<typename Arg>
    typename List<Arg>::crIterator List<Arg>::insert(const crIterator &pos, const Arg &target) {
        return crIterator(insert_Helper(pos.target.target, 1, target, true));
    }

    template<typename Arg>
    typename List<Arg>::rIterator
    List<Arg>::insert(const rIterator &pos, Size size, const Arg &target) {
        return rIterator(insert_Helper(pos.target.target), size, target, true);
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::insert(const crIterator &pos, Size size, const Arg &target) {
        return crIterator(insert_Helper(pos.target.target), size, target, true);
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::insert(const Iterator &pos, Size size, const Arg &target) {
        return Iterator(insert_Helper(pos.target), size, target, false);
    }

    template<typename Arg>
    typename List<Arg>::cIterator
    List<Arg>::insert(const cIterator &pos, Size size, const Arg &target) {
        return cIterator(insert_Helper(pos.target.target), size, target, false);
    }

    template<typename Arg>
    typename List<Arg>::rIterator
    List<Arg>::insert(const rIterator &pos, const std::initializer_list<Arg> &list) {
        return rIterator(insert_Helper(pos.target.target, list, true));
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::insert(const crIterator &pos, const std::initializer_list<Arg> &list) {
        return crIterator(insert_Helper(pos.target.target, list, true));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::rIterator
    List<Arg>::insert(const rIterator &pos,
                      Input_iterator begin, const Input_iterator &end) {
        return rIterator(insert_Helper(pos.target.target, begin, end, true, Is_integral<Input_iterator>()));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename List<Arg>::crIterator
    List<Arg>::insert(const crIterator &pos,
                      Input_iterator begin, const Input_iterator &end) {
        return crIterator(insert_Helper(pos.target.target, begin, end, true, Is_integral<Input_iterator>()));
    }

    template<typename Arg>
    typename List<Arg>::Iterator List<Arg>::erase(const Iterator &iter) {
        if (!iter.target)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        Node *now = iter.target, *prv = now->last, *next = now->next;
        if (prv) prv->next = next;
        else val_begin = next;
        if (next) next->last = prv;
        else val_end = prv;
        --size_;
        return Iterator(next, this);
    }

    template<typename Arg>
    typename List<Arg>::cIterator List<Arg>::erase(const cIterator &iter) {
        return cIterator(erase(iter.target));
    }

    template<typename Arg>
    typename List<Arg>::Iterator
    List<Arg>::erase(const Iterator &begin, const Iterator &end) {
        if (!begin.target)
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
        if (!iter.target.target)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        Node *now = iter.target.target, *prv = now->last, *next = now->next;
        if (prv) prv->next = next;
        else val_begin = next;
        if (next) next->last = prv;
        else val_end = prv;
        --size_;
        return Iterator(prv, this);
    }

    template<typename Arg>
    typename List<Arg>::crIterator List<Arg>::erase(const crIterator &iter) {
        return crIterator(erase(rIterator(iter.target)).target);
    }

    template<typename Arg>
    typename List<Arg>::rIterator List<Arg>::erase(const rIterator &begin, const rIterator &end) {
        if (!begin.target.target)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        release(end.target.target->next, begin.target.target->next);
        return end;
    }

    template<typename Arg>
    typename List<Arg>::crIterator
    List<Arg>::erase(const crIterator &begin, const crIterator &end) {
        if (!begin.target.target)
            throw outOfRange("You passed in an out-of-range iterator in the 'List::erase' function");
        release(end.target.target->next, begin.target.target->next);
        return end;
    }

    template<typename Arg>
    List<Arg> &List<Arg>::operator=(const List<Arg> &other) {
        if (this == &other) return *this;
        clear();
        insert_Helper(nullptr, other.begin(), other.end(), false, False_type());
        return *this;
    }

    template<typename Arg>
    List<Arg> &List<Arg>::operator=(List<Arg> &&other) noexcept {
        if (this == &other) return *this;
        clear();
        if (other.size_) {
            size_ = other.size_;
            val_begin = other.val_begin;
            val_end = other.val_end;
            other.size_ = 0;
            other.val_begin = nullptr;
            other.val_end = nullptr;
        }
        return *this;
    }

    template<typename Type>
    bool operator==(const List<Type> &left, const List<Type> &right) {
        if (left.size_ != right.size_) return false;
        auto l = left.val_begin, r = right.val_begin;
        while (l) {
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
        auto l = left.val_begin, r = right.val_begin;
        while (l && r) {
            if (l->value != r->value) return l->value < r->value;
            l = l->next, r = r->next;
        }
        return !l && r;
    }

    template<typename Type>
    bool operator<=(const List<Type> &left, const List<Type> &right) {
        return !(left > right);
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
        return !(left < right);
    }

    template<typename Type>
    inline void swap(List<Type> &left, List<Type> &right) noexcept {
        left.swap(right);
    }


//----------------------------------------------------------------------------------------------------------------------

}

#endif //TINYSTL_LIST_HPP
