//
// Created by 86152 on 2023/10/1.
//

#ifndef TINYSTL_RED_BLACK_TREE_HPP
#define TINYSTL_RED_BLACK_TREE_HPP

#include "../Iterator.hpp"
#include "../Allocater.hpp"
#include "../Function.hpp"

namespace STD {

    template<typename Key, typename Val, typename Compare, typename Equal_>
    class Red_Black_Tree;

    namespace Detail {

        /* 这个迭代器可能存在一些出乎意料的结果，当你对其进行递增操作时越过了容器末尾的空迭代
         * 器时，它会返回到容器的第一个元素的位置！相同的，递减操作越界时会返回到容器最后一个
         * 元素的位置。这使得该迭代器更像一个环形迭代器，带来隐患的同时，也可以进行一些特殊的操
         * 作。当然，迭代器保证首尾第一个不存在的元素无法解引用。
         */

        template<typename Key, typename Val, typename Compare, typename Equal_>
        struct Red_Black_Tree_Iterator : public Iterator<Bidirectional_iterator_tag, Pair<Key, Val>> {
        public:
            using Basic = Iterator<Bidirectional_iterator_tag, Pair<Key, Val>>;

            using Self = Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            using Container = Red_Black_Tree<Key, Val, Compare, Equal_>;

            using Node = typename Container::Node;

            friend class Red_Black_Tree<Key, Val, Compare, Equal_>;

            friend class rIterator<Self, Random_iterator_tag>;

            friend class cIterator<Self, Random_iterator_tag>;

            friend class crIterator<Self, Random_iterator_tag>;

        private:
            Node *target;

            Container *container;

            explicit Red_Black_Tree_Iterator(Node *target, Container *container) :
                    target(target), container(container) {};

        public:
            Reference operator*() const {
                return target->value;
            };

            Pointer operator->() const {
                return &(target->value);
            };

            Self &operator++();

            Self operator++(int);

            Self &operator--();

            Self operator--(int);

            friend bool operator==(const Self &left, const Self &right) {
                return left.target == right.target;
            };

            friend bool operator!=(const Self &left, const Self &right) {
                return left.target != right.target;
            };
        };

        template<typename Key, typename Val, typename Compare, typename Equal_>
        typename Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>::Self
        &Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>::operator++() {
            if (!target) {
                target = container->val_begin;
                return *this;
            }
            if (target->right) {
                target = Container::the_min(target->right);
            } else {
                Node *temp = target;
                while (temp->last && temp->last->right == temp)
                    temp = temp->last;
                target = temp->last;
            }
            return *this;
        }

        template<typename Key, typename Val, typename Compare, typename Equal_>
        typename Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>::Self
        Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>::operator++(int) {
            Self temp = Self(target, container);
            ++(*this);
        }

        template<typename Key, typename Val, typename Compare, typename Equal_>
        typename Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>::Self
        &Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>::operator--() {
            if (!target) {
                target = container->val_end;
                return *this;
            }
            if (target->left) {
                target = Container::the_max(target->left);
            } else {
                Node *temp = target;
                while (temp->last && temp->last->left == temp)
                    temp = temp->last;
                target = temp->last;
            }
            return *this;
        }

        template<typename Key, typename Val, typename Compare, typename Equal_>
        typename Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>::Self
        Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>::operator--(int) {
            Self temp = Self(target, container);
            --(*this);
        }
    }

    template<typename Key, typename Val, typename Compare = Less<Key>, typename Equal_ = Equal<Key>>
    class Red_Black_Tree {
    public:
        friend class Detail::Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>;

        using Iterator = Detail::Red_Black_Tree_Iterator<Key, Val, Compare, Equal_>;

        using cIterator = STD::cIterator<Iterator, Bidirectional_iterator_tag>;

        using rIterator = STD::rIterator<Iterator, Bidirectional_iterator_tag>;

        using crIterator = STD::crIterator<Iterator, Bidirectional_iterator_tag>;

        using Self = Red_Black_Tree<Key, Val, Compare, Equal_>;

    private:
        struct Node {
            Pair<Key, Val> value;

            Node *left = nullptr, *right = nullptr, *last = nullptr;

            bool Red = false;

            explicit Node(const Key &key, const Val &val, bool red) :
                    value({key, val}), Red(red) {};

            explicit Node(const Key &key, Val &&val, bool red) :
                    value({key, move(val)}), Red(red) {};

            const Key &key() const { return value.first; };

            Node *copy() const {
                auto temp = Allocate<Node>(value.first, value.second, Red);
                temp->left = left;
                temp->right = right;
                temp->last = last;
            }
        };

        Node *root = nullptr, *val_begin = nullptr, *val_end = nullptr;

        Compare less = Less<Key>();

        Equal_ equal = Equal<Key>();

        Size size_ = 0;

        Node *L_alpha(Node *node) {
            Node *n_right = node->right;
            link(node, false, n_right->left);
            link(n_right, true, node);
            n_right->Red = node->Red;
            node->Red = true;
            return n_right;
        }

        Node *R_alpha(Node *node) {
            Node * n_left = node->left;
            link(node, true, n_left->right);
            link(n_left, false, node);
            n_left->Red = node->Red;
            node->Red = true;
            return n_left;
        }

        void insert_change_Color(Node *node) const {
            node->Red = true;
            node->left->Red = false;
            node->right->Red = false;
        }

        void erase_change_Color(Node *node) const {
            node->Red = false;
            node->left->Red = true;
            node->right->Red = true;
        }

        bool is_Red(Node *node) {
            if (!node) return false;
            else return node->Red;
        }

        void link(Node *parent, bool left, Node *child) {
            if (parent) {
                if (left) parent->left = child;
                else parent->right = child;
            }
            if (child) child->last = parent;
        }

        static Node *the_min(Node *node);

        static Node *the_max(Node *node);

        Node *get_from(Node *node, const Key &key) const;

        Node *insert(Node *node, Node *&target);

        Node *erase_left_adjustment(Node *node);

        Node *erase_right_adjustment(Node *node);

        Node *erase_min(Node *node);

        Node *balance(Node *node);

        Node *erase(Node *node, const Key &key);

    public:
        Pair<Iterator, bool> insert(const Key &key, const Val &value);

        Pair<Iterator, bool> insert(const Key &key, Val &&value);

        bool erase(const Key &key);

        Iterator find(const Key &key) const;

        Val &operator[](const Key &key);

        Size size() const { return size_; };

        Iterator begin() const { return Iterator(val_begin, const_cast<Self *>(this)); };

        Iterator end() const { return Iterator(nullptr, const_cast<Self *>(this)); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin, const_cast<Self *>(this))); };

        cIterator cend() const { return cIterator(Iterator(nullptr, const_cast<Self *>(this))); };

        rIterator rbegin() const { return rIterator(Iterator(val_end, const_cast<Self *>(this))); };

        rIterator rend() const { return rIterator(Iterator(nullptr, const_cast<Self *>(this))); };

        crIterator crbegin() const { return crIterator(Iterator(val_end, const_cast<Self *>(this))); };

        crIterator crend() const { return crIterator(Iterator(nullptr, const_cast<Self *>(this))); };

    };

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Val &Red_Black_Tree<Key, Val, Compare, Equal_>::operator[](const Key &key) {
        auto ptr = get_from(root, key);
        if (ptr) return ptr->value.second;
        return (*(insert(key, Val()).first)).second;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::the_min(Node *node) {
        while (node->left) node = node->left;
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::the_max(Node *node) {
        while (node->right) node = node->right;
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::get_from(Node *node, const Key &key) const {
        while (node && !equal(key, node->key())) {
            if (less(key, node->key())) node = node->left;
            else node = node->right;
        }
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::insert(Node *node, Node *&target) {
        if (!node) {
            ++size_;
            return target;
        }
        if (less(target->key(), node->key())) link(node, true, insert(node->left, target));
        else if (!equal(target->key(), node->key())) link(node, false, insert(node->right, target));
        else {
            Deallocate(target);
            target = node;
        }
        //当前节点左黑右红，左旋。
        if (!is_Red(node->left) && is_Red(node->right)) node = L_alpha(node);
        //当前的节点子节点出现左边红节点相连的情况，右旋。
        if (is_Red(node->left) && is_Red(node->left->left)) node = R_alpha(node);
        //当前节点的两个子节点为红色，染色，将红节点向上传递，要放到最后。
        if (is_Red(node->left) && is_Red(node->right)) insert_change_Color(node);
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    Pair<typename Red_Black_Tree<Key, Val, Compare, Equal>::Iterator, bool>
    Red_Black_Tree<Key, Val, Compare, Equal>::insert(const Key &key, const Val &value) {
        Size record = size_;
        //插入节点为红节点，可以强迫结构的变换。
        Node * temp = Allocate<Node>(key, value, true);
        if (!size_) {
            val_begin = temp;
            val_end = temp;
        } else if (less(key, val_begin->key())) {
            val_begin = temp;
        } else if (!less(key, val_end->key()) && !equal(key, val_end->key())) {
            val_end = temp;
        }
        root = insert(root, temp);
        root->Red = false;
        root->last = nullptr;
        return {Iterator(temp, this), size_ != record};
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    Pair<typename Red_Black_Tree<Key, Val, Compare, Equal>::Iterator, bool>
    Red_Black_Tree<Key, Val, Compare, Equal>::insert(const Key &key, Val &&value) {
        Size record = size_;
        //插入节点为红节点，可以强迫结构的变换。
        Node * temp = Allocate<Node>(key, move(value), true);
        if (!size_) {
            val_begin = temp;
            val_end = temp;
        } else if (less(key, val_begin->key())) {
            val_begin = temp;
        } else if (!less(key, val_end->key()) && !equal(key, val_end->key())) {
            val_end = temp;
        }
        root = insert(root, temp);
        root->Red = false;
        root->last = nullptr;
        return {Iterator(temp, this), size_ != record};
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::erase_left_adjustment(Node *node) {
        erase_change_Color(node);
        if (is_Red(node->right->left)) {
            link(node, false, R_alpha(node->right));
            node = L_alpha(node);
        }
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::erase_right_adjustment(Node *node) {
        erase_change_Color(node);
        if (is_Red(node->left->left)) {
            node = R_alpha(node);
        }
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::balance(Node *node) {
        if (is_Red(node->right))
            node = L_alpha(node);
        if (!is_Red(node->left) && is_Red(node->right)) node = L_alpha(node);
        if (is_Red(node->left) && is_Red(node->left->left)) node = R_alpha(node);
        if (is_Red(node->left) && is_Red(node->right)) insert_change_Color(node);
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::erase_min(Node *node) {
        if (!node->left) {
            Deallocate(node);
            --size_;
            return nullptr;
        }
        if (!is_Red(node->left) && !is_Red(node->left->left))
            node = erase_left_adjustment(node);
        link(node, true, erase_min(node->left));
        return balance(node);
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::erase(Node *node, const Key &key) {
        if (less(key, node->key())) {
            if (!is_Red(node->left) && !is_Red(node->left->left))
                node = erase_left_adjustment(node);
            link(node, true, erase(node->left, key));
        } else {
            if (is_Red(node->left))
                node = R_alpha(node);
            if (equal(key, node->key()) && !node->right) {
                Deallocate(node);
                --size_;
                return nullptr;
            }
            if (!is_Red(node->right) && !is_Red(node->right->left))
                node = erase_right_adjustment(node);
            if (equal(key, node->key())) {
                node->value = the_min(node->right)->value;
                link(node, false, erase_min(node->right));
            } else {
                link(node, false, erase(node->right, key));
            }
        }
        return balance(node);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool Red_Black_Tree<Key, Val, Compare, Equal_>::erase(const Key &key) {
        Size record = size_;
        if (equal(key, val_begin->key())) {
            if (!val_begin->right) val_begin = val_begin->last;
            else val_begin = val_begin->right;
        }
        if (equal(key, val_end->key())) {
            if (!val_end->left) val_end = val_end->last;
            else val_end = val_end->left;
        }
        if (!is_Red(root->left) && !is_Red(root->right))
            root->Red = true;
        root = erase(root, key);
        if (root) {
            root->Red = false;
            root->last = nullptr;
        }
        return record != size_;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Iterator
    Red_Black_Tree<Key, Val, Compare, Equal>::find(const Key &key) const {
        auto temp = root;
        while (temp && !equal(key, temp->key())) {
            if (less(key, temp->key())) temp = temp->left;
            else temp = temp->right;
        }
        return Iterator(temp, const_cast<Self *>(this));
    }

}

#endif //TINYSTL_RED_BLACK_TREE_HPP
