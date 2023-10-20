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

        /* 该迭代器end()和before_begin()和before_cbegin()指向的位置相同，都无法被解引用。这使得该迭
         * 代器的行为更像一个环形迭代器，带来隐患的同时，也可以进行一些特殊的操作。使用时要多加注意。
         */
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

            Forward_List<Arg> *container;

            explicit Forward_List_Iterator(Node *target, Forward_List<Arg> *container) :
                    target(target), container(container) {};

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
                Self temp = Self(target);
                ++(*this);
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
    class Forward_List {
    public:
        friend class Detail::Forward_List_Iterator<Arg>;

        using Iterator = Detail::Forward_List_Iterator<Arg>;

        using cIterator = STD::crIterator<Iterator, Forward_iterator_tag>;

    private:
        struct Node {
            Arg value;

            Node *next;

            Node(const Arg &target, Node *next = nullptr) : value(target), next(next) {};

            Node(Arg &&target, Node *next = nullptr) : value(move(target)), next(next) {};

        };

        Size size_ = 0;

        Node *val_begin = nullptr;

        void release_after(Node *begin, Node *end);

        Iterator insert_Helper(Node *location, Signed_Size size, Arg target);

        Iterator insert_Helper(Node *location, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Iterator insert_Helper(Node *location, Input_iterator begin,
                               const Input_iterator &end, False_type);

        template<typename Input_iterator>
        Iterator insert_Helper(Node *location, Input_iterator size,
                               const Input_iterator &target, True_type);

    public:
        Forward_List() = default;

        Forward_List(Size size, Arg target = Arg());

        Forward_List(const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Forward_List(Input_iterator begin, const Input_iterator &end);

        Forward_List(const Forward_List<Arg> &other);

        Forward_List(Forward_List<Arg> &&other) noexcept;

        ~Forward_List();

        Size size() const { return size_; };

        Size capacity() const { return size_; };

        void clear();

        template<typename Input_iterator>
        Forward_List<Arg> &assign(Input_iterator begin, const Input_iterator &end);

        Forward_List<Arg> &assign(const std::initializer_list<Arg> &list);

        template<typename... args>
        void emplace_front(args &&...vals);

        void emplace_front(const Arg &val);

        void push_front(const Arg &val);

        void push_front(Arg &&val);

        void push_front(Size size, const Arg &val);

        template<typename Input_iterator>
        void push_front(Input_iterator begin, const Input_iterator &end);

        void pop_front();

        template<typename... args>
        Iterator emplace_after(const Iterator &pos, args &&...vals);

        template<typename... args>
        cIterator emplace_after(const cIterator &pos, args &&...vals);

        Iterator emplace_after(const Iterator &pos, const Arg &target);

        cIterator emplace_after(const cIterator &pos, const Arg &target);

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
            return val_begin->value;
        };

        bool empty() const { return !size_; };

        void swap(Forward_List<Arg> &other) noexcept {
            Size temp1 = size_;
            Node *temp2 = val_begin;
            size_ = other.size_;
            val_begin = other.val_begin;
            other.size_ = temp1;
            other.val_begin = temp2;
        }

        Forward_List<Arg> &operator=(const Forward_List<Arg> &other);

        Forward_List<Arg> &operator=(Forward_List<Arg> &&other) noexcept;

        template<typename Type>
        friend bool operator==(const Forward_List<Type> &left, const Forward_List<Type> &right);

        template<typename Type>
        friend bool operator!=(const Forward_List<Type> &left, const Forward_List<Type> &right);

        template<typename Type>
        friend bool operator<(const Forward_List<Type> &left, const Forward_List<Type> &right);

        template<typename Type>
        friend bool operator<=(const Forward_List<Type> &left, const Forward_List<Type> &right);

        template<typename Type>
        friend bool operator>(const Forward_List<Type> &left, const Forward_List<Type> &right);

        template<typename Type>
        friend bool operator>=(const Forward_List<Type> &left, const Forward_List<Type> &right);

        template<typename Type>
        friend inline void swap(Forward_List<Arg> &left, Forward_List<Arg> &right) noexcept;

        Iterator begin() const { return Iterator(val_begin, const_cast<Forward_List<Arg> *>(this)); };

        Iterator end() const { return Iterator(nullptr, const_cast<Forward_List<Arg> *>(this)); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin), const_cast<Forward_List<Arg> *>(this)); };

        cIterator cend() const { return cIterator(Iterator(nullptr, const_cast<Forward_List<Arg> *>(this))); };

        Iterator before_begin() const { return Iterator(nullptr, const_cast<Forward_List<Arg> *>(this)); };

        cIterator before_cbegin() const { return cIterator(Iterator(nullptr, const_cast<Forward_List<Arg> *>(this))); };

    };

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Forward_List<Arg>::Forward_List(Size size, Arg target) {
        insert_Helper(nullptr, size, target);
    }

    template<typename Arg>
    Forward_List<Arg>::Forward_List(const std::initializer_list<Arg> &list) {
        insert_Helper(nullptr, list);
    }

    template<typename Arg>
    template<typename Input_iterator>
    Forward_List<Arg>::Forward_List(Input_iterator begin, const Input_iterator &end) {
        insert_Helper(nullptr, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    Forward_List<Arg>::Forward_List(const Forward_List<Arg> &other) {
        insert_Helper(nullptr, other.begin(), other.end(), False_type());
    }

    template<typename Arg>
    Forward_List<Arg>::Forward_List(Forward_List<Arg> &&other) noexcept
            : size_(other.size_), val_begin(other.val_begin) {
        other.size_ = 0;
        other.val_begin = nullptr;
    }

    template<typename Arg>
    Forward_List<Arg>::~Forward_List() {
        clear();
    }

    template<typename Arg>
    void Forward_List<Arg>::release_after(Node *begin, Node *end) {
        auto temp = begin ? begin->next : val_begin, t = temp;
        while (temp != end) {
            t = temp;
            temp = temp->next;
            Deallocate(t);
            --size_;
        }
        if (begin) begin->next = end;
        else val_begin = end;
    }

    template<typename Arg>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::insert_Helper(Node *location, Signed_Size size, Arg target) {
        if (!size) return Iterator(location, this);
        Node *last = location, *now = location, *store;
        size_ += size;
        if (!location) {
            store = val_begin;
            val_begin = last = now = Allocate<Node>(target);
            --size;
        } else store = location->next;
        while (size > 0) {
            now = Allocate<Node>(target);
            last->next = now;
            last = now;
            --size;
        }
        now->next = store;
        return Iterator(now, this);
    }

    template<typename Arg>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::insert_Helper(Node *location,
                                     const std::initializer_list<Arg> &list) {
        Size size = list.size();
        if (!size) return Iterator(location, this);
        Node *last = location, *now = location, *store;
        auto target = list.begin();
        size_ += size;
        if (!location) {
            store = val_begin;
            val_begin = last = now = Allocate<Node>(*target);
            --size, ++target;
        } else store = location->next;
        while (size > 0) {
            now = Allocate<Node>(*target);
            last->next = now;
            last = now;
            --size, ++target;
        }
        now->next = store;
        return Iterator(now, this);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::insert_Helper(Node *location, Input_iterator begin,
                                     const Input_iterator &end, False_type) {
        if (begin == end) return Iterator(location, this);
        Node *last = location, *now = location, *store;
        if (!location) {
            store = val_begin;
            val_begin = last = now = Allocate<Node>(*begin);
            ++size_, ++begin;
        } else store = location->next;
        while (begin != end) {
            now = Allocate<Node>(*begin);
            last->next = now;
            last = now;
            ++size_, ++begin;
        }
        now->next = store;
        return Iterator(now, this);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::insert_Helper(Node *location, Input_iterator size,
                                     const Input_iterator &target, True_type) {
        return insert_Helper(location, size, target);
    }

    //----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    void Forward_List<Arg>::clear() {
        release_after(nullptr, nullptr);
    }

    template<typename Arg>
    template<typename Input_iterator>
    Forward_List<Arg> &Forward_List<Arg>::assign(Input_iterator begin, const Input_iterator &end) {
        clear();
        insert_Helper(nullptr, begin, end, Is_integral<Input_iterator>());
        return *this;
    }

    template<typename Arg>
    Forward_List<Arg> &
    Forward_List<Arg>::assign(const std::initializer_list<Arg> &list) {
        clear();
        insert_Helper(nullptr, list);
        return *this;
    }

    template<typename Arg>
    template<typename... args>
    void Forward_List<Arg>::emplace_front(args &&...vals) {
        auto temp = Allocate<Node>(Arg(vals...), val_begin);
        val_begin = temp;
        ++size_;
    }

    template<typename Arg>
    void Forward_List<Arg>::emplace_front(const Arg &val) {
        auto temp = Allocate<Node>(val, val_begin);
        val_begin = temp;
        ++size_;
    }

    template<typename Arg>
    void Forward_List<Arg>::push_front(const Arg &val) {
        auto temp = Allocate<Node>(val, val_begin);
        val_begin = temp;
        ++size_;
    }

    template<typename Arg>
    void Forward_List<Arg>::push_front(Arg &&val) {
        auto temp = Allocate<Node>(move(val), val_begin);
        val_begin = temp;
        ++size_;
    }

    template<typename Arg>
    void Forward_List<Arg>::push_front(Size size, const Arg &val) {
        insert_Helper(nullptr, size, val);
    }

    template<typename Arg>
    template<typename Input_iterator>
    void Forward_List<Arg>::push_front(Input_iterator begin, const Input_iterator &end) {
        insert_Helper(nullptr, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    void Forward_List<Arg>::pop_front() {
        if (size_) {
            auto temp = val_begin;
            val_begin = val_begin->next;
            Deallocate(temp);
            --size_;
        }
    }

    template<typename Arg>
    template<typename... args>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::emplace_after(const Iterator &pos, args &&...vals) {
        Node *ptr = pos.target;
        auto temp = Allocate<Node>(Arg(vals...));
        if (!ptr) {
            temp->next = val_begin;
            val_begin = temp;
        } else {
            temp->next = ptr->next;
            ptr->next = temp;
        }
        ++size_;
        return Iterator(temp, this);
    }

    template<typename Arg>
    template<typename... args>
    typename Forward_List<Arg>::cIterator
    Forward_List<Arg>::emplace_after(const cIterator &pos, args &&...vals) {
        return cIterator(emplace_after(pos.target, forward(vals)...));
    }

    template<typename Arg>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::emplace_after(const Iterator &pos, const Arg &target) {
        return insert_after(pos, target);
    }

    template<typename Arg>
    typename Forward_List<Arg>::cIterator
    Forward_List<Arg>::emplace_after(const cIterator &pos, const Arg &target) {
        return insert_after(pos, target);
    }

    template<typename Arg>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::insert_after(const Iterator &pos, const Arg &target) {
        Node *ptr = pos.target;
        auto temp = Allocate<Node>(target);
        if (!ptr) {
            temp->next = val_begin;
            val_begin = temp;
        } else {
            temp->next = ptr->next;
            ptr->next = temp;
        }
        ++size_;
        return Iterator(temp, this);
    }

    template<typename Arg>
    typename Forward_List<Arg>::cIterator
    Forward_List<Arg>::insert_after(const cIterator &pos, const Arg &target) {
        Node *ptr = pos.target.target;
        auto temp = Allocate<Node>(target);
        if (!ptr) {
            temp->next = val_begin;
            val_begin = temp;
        } else {
            temp->next = ptr->next;
            ptr->next = temp;
        }
        ++size_;
        return cIterator(Iterator(temp, this));
    }

    template<typename Arg>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::insert_after(const Iterator &pos, Size size, const Arg &target) {
        return insert_Helper(pos.target, size, target);
    }

    template<typename Arg>
    typename Forward_List<Arg>::cIterator
    Forward_List<Arg>::insert_after(const cIterator &pos, Size size, const Arg &target) {
        return cIterator(insert_Helper(pos.target.target, size, target));
    }

    template<typename Arg>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::insert_after(const Iterator &pos, const std::initializer_list<Arg> &list) {
        return insert_Helper(pos.target, list);
    }

    template<typename Arg>
    typename Forward_List<Arg>::cIterator
    Forward_List<Arg>::insert_after(const cIterator &pos, const std::initializer_list<Arg> &list) {
        return cIterator(insert_Helper(pos.target.target, list));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::insert_after(const Iterator &pos, Input_iterator begin, const Input_iterator &end) {
        return insert_Helper(pos.target, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Forward_List<Arg>::cIterator
    Forward_List<Arg>::insert_after(const cIterator &pos, Input_iterator begin, const Input_iterator &end) {
        return cIterator(insert_Helper(pos.target.target, begin, end, Is_integral<Input_iterator>()));
    }

    template<typename Arg>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::erase_after(const Iterator &pos) {
        auto temp = pos.target->next;
        pos.target->next = temp->next;
        Deallocate(temp);
        --size_;
        return pos;
    }

    template<typename Arg>
    typename Forward_List<Arg>::Iterator
    Forward_List<Arg>::erase_after(const Iterator &begin, const Iterator &end) {
        release_after(begin.target, end.target);
        return begin;
    }

    template<typename Arg>
    typename Forward_List<Arg>::cIterator
    Forward_List<Arg>::erase_after(const cIterator &pos) {
        auto temp = pos.target->next;
        pos.target->next = temp->next;
        Deallocate(temp);
        --size_;
        return pos;
    }

    template<typename Arg>
    typename Forward_List<Arg>::cIterator
    Forward_List<Arg>::erase_after(const cIterator &begin, const cIterator &end) {
        release_after(begin.target.target, end.target.target);
        return begin;
    }

    template<typename Arg>
    Forward_List<Arg> &
    Forward_List<Arg>::operator=(const Forward_List<Arg> &other) {
        if (this == &other) return *this;
        clear();
        insert_Helper(nullptr, other.begin(), other.end(), False_type());
        return *this;
    }

    template<typename Arg>
    Forward_List<Arg> &
    Forward_List<Arg>::operator=(Forward_List<Arg> &&other) noexcept {
        if (this == &other) return *this;
        clear();
        size_ = other.size_;
        val_begin = other.val_begin;
        other.size_ = 0;
        other.val_begin = nullptr;
        return *this;
    }

    template<typename Type>
    bool operator==(const Forward_List<Type> &left, const Forward_List<Type> &right) {
        if (left.size_ != right.size_)
            return false;
        auto l = left.val_begin, r = right.val_begin;
        while (l) {
            if (l->value != r->value)
                return false;
            l = l->next, r = r->next;
        }
        return false;
    }

    template<typename Type>
    bool operator!=(const Forward_List<Type> &left, const Forward_List<Type> &right) {
        return !(left == right);
    }

    template<typename Type>
    bool operator<(const Forward_List<Type> &left, const Forward_List<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l && r) {
            if (l->value != r->value)
                return l->value < r->value;
            l = l->next, r = r->next;
        }
        return !l && r;
    }

    template<typename Type>
    bool operator<=(const Forward_List<Type> &left, const Forward_List<Type> &right) {
        return !(left > right);
    }

    template<typename Type>
    bool operator>(const Forward_List<Type> &left, const Forward_List<Type> &right) {
        auto l = left.val_begin, r = right.val_begin;
        while (l && r) {
            if (l->value != r->value)
                return l->value > r->value;
            l = l->next, r = r->next;
        }
        return !r && l;
    }

    template<typename Type>
    bool operator>=(const Forward_List<Type> &left, const Forward_List<Type> &right) {
        return !(left < right);
    }

    template<typename Type>
    inline void swap(Forward_List<Type> &left, Forward_List<Type> &right) noexcept {
        left.swap(right);
    }

}// namespace STD

#endif// TINYSTL_FORWARD_LIST_HPP
