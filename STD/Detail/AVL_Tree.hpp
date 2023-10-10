//
// Created by 86152 on 2023/10/4.
//

#ifndef TINYSTL_AVL_TREE_HPP
#define TINYSTL_AVL_TREE_HPP

#include "../Iterator.hpp"
#include "../Allocater.hpp"
#include "../Function.hpp"

namespace STD {

    namespace Detail {

        template<typename Key, typename Compare, typename Equal_>
        class AVL_Tree;

        /* 该迭代器end()和rend()和cend()和crend()指向的位置相同，都无法被解引用。这使得该迭代器
         * 的行为更像一个环形迭代器，带来隐患的同时，也可以进行一些特殊的操作。使用时要多加注意。
         */

        template<typename Key, typename Compare, typename Equal_>
        struct AVL_Tree_Iterator : public Iterator<Bidirectional_iterator_tag, Key> {
        public:
            using Basic = Iterator<Bidirectional_iterator_tag, Key>;

            using Self = AVL_Tree_Iterator<Key, Compare, Equal_>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            using Container = AVL_Tree<Key, Compare, Equal_>;

            using Node = typename Container::Node;

            friend class AVL_Tree<Key, Compare, Equal_>;

            friend class rIterator<Self, Random_iterator_tag>;

            friend class cIterator<Self, Random_iterator_tag>;

            friend class crIterator<Self, Random_iterator_tag>;

        private:
            Node *target;

            Container *container;

            explicit AVL_Tree_Iterator(Node *target, Container *container) :
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

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree_Iterator<Key, Compare, Equal_>::Self
        &AVL_Tree_Iterator<Key, Compare, Equal_>::operator++() {
            if (!target) {
                target = Container::the_min(container->root);
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

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree_Iterator<Key, Compare, Equal_>::Self
        AVL_Tree_Iterator<Key, Compare, Equal_>::operator++(int) {
            Self temp = Self(target, container);
            ++(*this);
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree_Iterator<Key, Compare, Equal_>::Self
        &AVL_Tree_Iterator<Key, Compare, Equal_>::operator--() {
            if (!target) {
                target = Container::the_max(container->root);
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

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree_Iterator<Key, Compare, Equal_>::Self
        AVL_Tree_Iterator<Key, Compare, Equal_>::operator--(int) {
            Self temp = Self(target, container);
            --(*this);
        }

//----------------------------------------------------------------------------------------------------------------------

        template<typename Key, typename Compare = Less<Key>, typename Equal_ = Equal<Key>>
        class AVL_Tree {
        public:
            friend class Detail::AVL_Tree_Iterator<Key, Compare, Equal_>;

            using Iterator = Detail::AVL_Tree_Iterator<Key, Compare, Equal_>;

            using cIterator = STD::cIterator<Iterator, Bidirectional_iterator_tag>;

            using rIterator = STD::rIterator<Iterator, Bidirectional_iterator_tag>;

            using crIterator = STD::crIterator<Iterator, Bidirectional_iterator_tag>;

            using Self = AVL_Tree<Key, Compare, Equal_>;

        protected:
            struct Node {
                Key value;

                Node *left = nullptr, *right = nullptr, *last = nullptr;

                int height = 1;

                template<typename ...Args>
                explicit Node(Args &&...args) : value(forward<Args>(args)...) {};

            };

            Node *root = nullptr;

            Compare less;

            Equal_ equal;

            Size size_ = 0;

            // 将target_val位置的值换成val的值。
            void swap_value(Node *val, Node *target_val) {
                link(val, true, target_val->left);
                link(val, false, target_val->right);
                if (target_val->last)
                    link(target_val->last, target_val->last->left == target_val, val);
                else val->last = nullptr;
                val->height = target_val->height;
                if (root == target_val) root = val;
            }

            // 内置版本的插入函数。
            Node *insert(Node *node, Node *target, Node *&judge);

            // 内置递归版本的删除函数。
            Node *erase(Node *node, const Key &key, Node *&val, Node *&target_val);

            // 找到该节点下的最小值。
            static Node *the_min(Node *node);

            // 找到该节点下的最大值。
            static Node *the_max(Node *node);

            // 把target节点下的分支复制到parent的下面。
            static Node *copy(Node *target, Node *parent);

            // 得到某个节点的迭代器，主要是为了子类服务。。
            Iterator get_Iterator(Node *temp) const { return Iterator(temp, const_cast<Self *>(this)); };

        private:
            // 计算传入节点的左节点与右节点之差
            int height_difference(Node *node) const {
                if (!node) return 0;
                int left = node->left ? node->left->height : 0;
                int right = node->right ? node->right->height : 0;
                return left - right;
            };

            // 重新计算传入节点的高度
            void calculate_height(Node *node) const {
                int left = node->left ? node->left->height : 0;
                int right = node->right ? node->right->height : 0;
                node->height = left > right ? left + 1 : right + 1;
            }

            // link操作是为了更新子节点的last，并将子节点连接到父节点上，同时会重新计算父节点的高度，可以减少许多麻烦。
            void link(Node *parent, bool left, Node *child) const {
                if (parent) {
                    if (left) parent->left = child;
                    else parent->right = child;
                    calculate_height(parent);
                }
                if (child) child->last = parent;
            };

            // 左旋
            Node *L_alpha(Node *node) {
                Node *n_right = node->right;
                link(node, false, n_right->left);
                link(n_right, true, node);
                return n_right;
            }

            // 右旋
            Node *R_alpha(Node *node) {
                Node *n_left = node->left;
                link(node, true, n_left->right);
                link(n_left, false, node);
                return n_left;
            }

            // 该函数的作用是在树被破坏时，重新平衡树。
            Node *balance(Node *node);

            // 从该节点开始寻找目标值。
            Node *get_from(Node *node, const Key &key) const;

            // 删除该节点下的最大值。
            Node *erase_max(Node *node, Node *&val);

            // 将该节点的全部分支删除。
            void erase_from(Node *node);

        public:
            AVL_Tree() = default;

            AVL_Tree(const Compare &compare) : less(compare) {};

            AVL_Tree(const Compare &compare, const Equal_ &equal) : less(compare), equal(equal) {};

            AVL_Tree(const Self &other);

            AVL_Tree(Self &&other) noexcept;

            template<typename Input_Key>
            AVL_Tree(Input_Key k_begin, const Input_Key &k_end);

            ~AVL_Tree();

            void clear();

            Pair<Iterator, bool> insert(const Key &key);

            bool erase(const Key &key);

            Iterator find(const Key &key);

            cIterator find(const Key &key) const;

            Iterator at(const Key &key);

            cIterator at(const Key &key) const;

            Key &operator[](const Key &key);

            Size count(const Key &key) const;

            bool contains(const Key &key) const;

            Pair<Iterator, Iterator> equal_range(const Key &key);

            Iterator lower_bound(const Key &key);

            Iterator upper_bound(const Key &key);

            Pair<cIterator, cIterator> equal_range(const Key &key) const;

            cIterator lower_bound(const Key &key) const;

            cIterator upper_bound(const Key &key) const;

            Size size() const { return size_; };

            bool empty() const { return !size_; };

            void swap(Self &other) noexcept {
                Size temp1 = size_;
                Node *temp2 = root;
                size_ = other.size_;
                root = other.root;
                other.size_ = temp1;
                other.root = temp2;
                STD::swap(less, other.less);
                STD::swap(equal, other.equal);
            }

            // 这两个函数不会复制Compare和Equal_对象。
            Self &operator=(const Self &other);

            Self &operator=(Self &&other) noexcept;

            Iterator begin() const { return get_Iterator(the_min(root)); };

            Iterator end() const { return get_Iterator(nullptr); };

            cIterator cbegin() const { return cIterator(get_Iterator(the_min(root))); };

            cIterator cend() const { return cIterator(end()); };

            rIterator rbegin() const { return rIterator(get_Iterator(the_max(root))); };

            rIterator rend() const { return rIterator(end()); };

            crIterator crbegin() const { return crIterator(get_Iterator(the_max(root))); };

            crIterator crend() const { return crIterator(end()); };

            friend bool operator==(const Self &left, const Self &right) {
                if (left.size_ != right.size_) return false;
                auto l = left.begin(), r = right.begin();
                Size size = left.size_;
                while (size) {
                    if (*l != *r) return false;
                    --size;
                }
                return true;
            }

            friend bool operator!=(const Self &left, const Self &right) {
                return !(left == right);
            }

            friend bool operator<(const Self &left, const Self &right) {
                auto l = left.begin(), r = right.begin(), l_end = left.end(), r_end = right.end();
                while (l != l_end && r != r_end) {
                    if (*l != *r) return *l < *r;
                    ++l, ++r;
                }
                return l == l_end && r != r_end;
            }

            friend bool operator<=(const Self &left, const Self &right) {
                return !(left > right);
            }

            friend bool operator>(const Self &left, const Self &right) {
                auto l = left.begin(), r = right.begin(), l_end = left.end(), r_end = right.end();
                while (l != l_end && r != r_end) {
                    if (*l != *r) return *l > *r;
                    ++l, ++r;
                }
                return l != l_end && r == r_end;
            }

            friend bool operator>=(const Self &left, const Self &right) {
                return !(left < right);
            }

            friend inline void swap(Self &left, Self &right) noexcept {
                left.swap(right);
            }

        };

//----------------------------------------------------------------------------------------------------------------------

        template<typename Key, typename Compare, typename Equal>
        typename AVL_Tree<Key, Compare, Equal>::Node *
        AVL_Tree<Key, Compare, Equal>::the_min(Node *node) {
            if (!node) return nullptr;
            while (node->left) node = node->left;
            return node;
        }

        template<typename Key, typename Compare, typename Equal>
        typename AVL_Tree<Key, Compare, Equal>::Node *
        AVL_Tree<Key, Compare, Equal>::the_max(Node *node) {
            if (!node) return nullptr;
            while (node->right) node = node->right;
            return node;
        }

        template<typename Key, typename Compare, typename Equal_>
        void AVL_Tree<Key, Compare, Equal_>::erase_from(Node *node) {
            if (!node) return;
            erase_from(node->left);
            erase_from(node->right);
            Deallocate(node);
            --size_;
        }

        template<typename Key, typename Compare, typename Equal>
        typename AVL_Tree<Key, Compare, Equal>::Node *
        AVL_Tree<Key, Compare, Equal>::get_from(Node *node, const Key &key) const {
            while (node && !equal(key, node->value)) {
                if (less(key, node->key())) node = node->left;
                else node = node->right;
            }
            return node;
        }

        template<typename Key, typename Compare, typename Equal>
        typename AVL_Tree<Key, Compare, Equal>::Node *
        AVL_Tree<Key, Compare, Equal>::copy(Node *target, Node *parent) {
            if (!target) return nullptr;
            Node *temp = Allocate<Node>(target->value);
            temp->height = target->height;
            temp->left = copy(target->left, temp);
            temp->right = copy(target->right, temp);
            temp->last = parent;
            return temp;
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Node
        *AVL_Tree<Key, Compare, Equal_>::balance(Node *node) {
            int difference = height_difference(node); // 得到左右的节点高度差。
            //一共有六种情况（插入会出现四种，删除会出现六种），这里合并为四种。
            if (difference > 1) { // 本节点的左边太长
                if (height_difference(node->left) >= 0) {
                    node = R_alpha(node);
                } else { // 本节点的左节点的右边比较长
                    link(node, true, L_alpha(node->left));
                    node = R_alpha(node);
                }
            } else if (difference < -1) { // 本节点的右边太长
                if (height_difference(node->right) <= 0) {
                    node = L_alpha(node);
                } else { // 本节点的右节点的左边比较长
                    link(node, false, R_alpha(node->right));
                    node = L_alpha(node);
                }
            }
            return node;
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Node
        *AVL_Tree<Key, Compare, Equal_>::insert(Node *node, Node *target, Node *&judge) {
            if (!node) { // 说明找到了合适的位置插入，将目标向上传递，终止递归。
                ++size_;
                return target;
            }
            if (less(target->value, node->value)) // 目标小于本节点，向左边寻找。
                link(node, true, insert(node->left, target, judge));
            else if (!equal(target->value, node->value)) // 目标大于本节点，向右边寻找。
                link(node, false, insert(node->right, target, judge));
            else { // 找到了相等值，给本节点重新赋值。
                judge = node;
                return node;
            }
            return balance(node);
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Node
        *AVL_Tree<Key, Compare, Equal_>::erase_max(Node *node, Node *&val) {
            if (node->right) { // 当没有找到最大值时，继续递归寻找。
                link(node, false, erase_max(node->right, val));
            } else if (node->left) { // 找到最大值，但其还有左节点，我们将其值换上来，删掉左节点。
                val = node;
                --size_;
                return node->left;
            } else { // 找到最大值，并且它没有任何子节点，可直接删除。
                val = node;
                --size_;
                return nullptr;
            }
            return balance(node);
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Node
        *AVL_Tree<Key, Compare, Equal_>::erase(Node *node, const Key &key, Node *&val, Node *&target_val) {
            if (!node) return nullptr; // 说明树中没有该目标，终止递归。
            if (less(key, node->value)) // 目标值小于当前值，向左继续寻找。
                link(node, true, erase(node->left, key, val, target_val));
            else if (!equal(key, node->value)) // 目标值大于当前值，向右继续寻找。
                link(node, false, erase(node->right, key, val, target_val));
            else { // 找到目标值
                if (node->left) { // 有子节点比较麻烦，我们选择置换上来当前节点左边的最大值，转而删除被置换的那个节点。
                    target_val = node;
                    link(node, true, erase_max(node->left, val));
                } else { // 这里合并了两种情况（有右节点和无节点），直接删除该节点，将该节点的右节点向上传递。
                    auto temp = node->right;
                    Deallocate(node);
                    --size_;
                    return temp;
                }
            }
            return balance(node);
        }

//----------------------------------------------------------------------------------------------------------------------

        template<typename Key, typename Compare, typename Equal_>
        AVL_Tree<Key, Compare, Equal_>::AVL_Tree(const Self &other) :
                size_(other.size_), less(other.less), equal(other.equal) {
            root = copy(other.root);
        }

        template<typename Key, typename Compare, typename Equal_>
        AVL_Tree<Key, Compare, Equal_>::AVL_Tree(Self &&other) noexcept
                : size_(other.size_), root(other.root), less(other.less), equal(other.equal) {
            other.size_ = 0;
            other.root = nullptr;
        }

        template<typename Key, typename Compare, typename Equal_>
        template<typename Input_Key>
        AVL_Tree<Key, Compare, Equal_>::AVL_Tree(Input_Key k_begin, const Input_Key &k_end) {
            while (k_begin != k_end) {
                insert(*k_begin);
                ++k_begin;
            }
        }

        template<typename Key, typename Compare, typename Equal_>
        AVL_Tree<Key, Compare, Equal_>::~AVL_Tree() {
            clear();
        }

        template<typename Key, typename Compare, typename Equal_>
        void AVL_Tree<Key, Compare, Equal_>::clear() {
            erase_from(root);
        }

        template<typename Key, typename Compare, typename Equal_>
        Pair<typename AVL_Tree<Key, Compare, Equal_>::Iterator, bool>
        AVL_Tree<Key, Compare, Equal_>::insert(const Key &key) {
            Size record = size_;
            Node *temp = Allocate<Node>(key), *judge = nullptr;
            root = insert(root, temp, judge);
            root->last = nullptr;
            if (judge) {
                swap_value(temp, judge);
                Deallocate(judge);
            }
            return {Iterator(temp, this), size_ != record};
        }

        template<typename Key, typename Compare, typename Equal_>
        bool AVL_Tree<Key, Compare, Equal_>::erase(const Key &key) {
            Size record = size_;
            Node *val = nullptr, *target_val = nullptr;
            root = erase(root, key, val, target_val);
            if (root) root->last = nullptr;
            if (target_val) {
                swap_value(val, target_val);
                Deallocate(target_val);
            }
            return record != size_;
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Iterator
        AVL_Tree<Key, Compare, Equal_>::find(const Key &key) {
            return Iterator(get_from(root, key), this);
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::cIterator
        AVL_Tree<Key, Compare, Equal_>::find(const Key &key) const {
            return cIterator(get_Iterator(get_from(root, key)));
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Iterator
        AVL_Tree<Key, Compare, Equal_>::at(const Key &key) {
            auto temp = get_from(root, key);
            if (!temp)
                throw outOfRange("You passed an out-of-range basic in at()");
            return Iterator(temp, this);
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::cIterator
        AVL_Tree<Key, Compare, Equal_>::at(const Key &key) const {
            auto temp = get_from(root, key);
            if (!temp)
                throw outOfRange("You passed an out-of-range basic in at()");
            return cIterator(Iterator(temp, const_cast<Self *>(this)));
        }

        template<typename Key, typename Compare, typename Equal_>
        Key &AVL_Tree<Key, Compare, Equal_>::operator[](const Key &key) {
            auto ptr = get_from(root, key);
            if (ptr) return ptr->basic;
            return *insert(key).first;
        }

        template<typename Key, typename Compare, typename Equal_>
        Size AVL_Tree<Key, Compare, Equal_>::count(const Key &key) const {
            return get_from(root, key);
        }

        template<typename Key, typename Compare, typename Equal_>
        bool AVL_Tree<Key, Compare, Equal_>::contains(const Key &key) const {
            return get_from(root, key);
        }

        template<typename Key, typename Compare, typename Equal_>
        Pair<typename AVL_Tree<Key, Compare, Equal_>::Iterator,
                typename AVL_Tree<Key, Compare, Equal_>::Iterator>
        AVL_Tree<Key, Compare, Equal_>::equal_range(const Key &key) {
            return {lower_bound(key), upper_bound(key)};
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Iterator
        AVL_Tree<Key, Compare, Equal_>::lower_bound(const Key &key) {
            Node *temp = root;
            while (temp) {
                if (less(key, temp->basic)) {
                    if (temp->left) temp = temp->left;
                    else return Iterator(temp, this);
                } else if (equal(temp->basic, key)) {
                    return Iterator(temp, this);
                } else {
                    temp = temp->right;
                }
            }
            return end();
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Iterator
        AVL_Tree<Key, Compare, Equal_>::upper_bound(const Key &key) {
            Node *temp = root;
            while (temp) {
                if (less(key, temp->basic)) {
                    if (temp->left) temp = temp->left;
                    else return Iterator(temp, this);
                } else if (equal(temp->basic, key)) {
                    return ++Iterator(temp, this);
                } else {
                    temp = temp->right;
                }
            }
            return end();
        }

        template<typename Key, typename Compare, typename Equal_>
        Pair<typename AVL_Tree<Key, Compare, Equal_>::cIterator,
                typename AVL_Tree<Key, Compare, Equal_>::cIterator>
        AVL_Tree<Key, Compare, Equal_>::equal_range(const Key &key) const {
            return {lower_bound(key), upper_bound(key)};
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::cIterator
        AVL_Tree<Key, Compare, Equal_>::lower_bound(const Key &key) const {
            cIterator(const_cast<Self *>(this)->lower_bound(key));
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::cIterator
        AVL_Tree<Key, Compare, Equal_>::upper_bound(const Key &key) const {
            return cIterator(const_cast<Self *>(this)->upper_bound(key));
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Self
        &AVL_Tree<Key, Compare, Equal_>::operator=(const Self &other) {
            if (this == &other) return *this;
            clear();
            size_ = other.size_;
            root = copy(other.root, nullptr);
            return *this;
        }

        template<typename Key, typename Compare, typename Equal_>
        typename AVL_Tree<Key, Compare, Equal_>::Self
        &AVL_Tree<Key, Compare, Equal_>::operator=(Self &&other) noexcept {
            if (this == &other) return *this;
            size_ = other.size_;
            root = other.root;
            other.size_ = 0;
            other.root = nullptr;
            return *this;
        }
    }
}

#endif //TINYSTL_AVL_TREE_HPP
