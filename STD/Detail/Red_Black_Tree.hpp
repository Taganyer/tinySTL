//
// Created by 86152 on 2023/10/1.
//

#ifndef TINYSTL_RED_BLACK_TREE_HPP
#define TINYSTL_RED_BLACK_TREE_HPP

#include "../Iterator.hpp"
#include "../Allocater.hpp"
#include "../Function.hpp"

namespace STD {

    namespace Detail {

        template <typename Key, typename Compare, typename Equal_>
        class Red_Black_Tree;

        /* 该迭代器end()和rend()和cend()和crend()指向的位置相同，都无法被解引用。这使得该迭代器
         * 的行为更像一个环形迭代器，带来隐患的同时，也可以进行一些特殊的操作。使用时要多加注意。
         */

        template <typename Key, typename Compare, typename Equal_>
        struct Red_Black_Tree_Iterator : public Iterator<Bidirectional_iterator_tag, Key> {
        public:
            using Basic = Iterator<Bidirectional_iterator_tag, Key>;

            using Self = Red_Black_Tree_Iterator<Key, Compare, Equal_>;

            using Container = Red_Black_Tree<Key, Compare, Equal_>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            using Node = typename Container::Node;

            friend class Red_Black_Tree<Key, Compare, Equal_>;

            friend class rIterator<Self, Bidirectional_iterator_tag>;

            friend class cIterator<Self, Bidirectional_iterator_tag>;

            friend class crIterator<Self, Bidirectional_iterator_tag>;

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

            Self& operator++();

            Self operator++(int);

            Self& operator--();

            Self operator--(int);

            friend bool operator==(const Self& left, const Self& right) {
                return left.target == right.target && left.container == right.container;
            };

            friend bool operator!=(const Self& left, const Self& right) {
                return left.target != right.target || left.container != right.container;
            };
        };

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree_Iterator<Key, Compare, Equal_>::Self
        & Red_Black_Tree_Iterator<Key, Compare, Equal_>::operator++() {
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

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree_Iterator<Key, Compare, Equal_>::Self
        Red_Black_Tree_Iterator<Key, Compare, Equal_>::operator++(int) {
            Self temp = Self(target, container);
            ++(*this);
            return temp;
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree_Iterator<Key, Compare, Equal_>::Self
        & Red_Black_Tree_Iterator<Key, Compare, Equal_>::operator--() {
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

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree_Iterator<Key, Compare, Equal_>::Self
        Red_Black_Tree_Iterator<Key, Compare, Equal_>::operator--(int) {
            Self temp = Self(target, container);
            --(*this);
            return temp;
        }

//----------------------------------------------------------------------------------------------------------------------

        template <typename Key, typename Compare = Less<Key>, typename Equal_ = Equal<Key>>
        class Red_Black_Tree {
        public:
            friend class Red_Black_Tree_Iterator<Key, Compare, Equal_>;

            using Iterator = Red_Black_Tree_Iterator<Key, Compare, Equal_>;

            using cIterator = STD::cIterator<Iterator, Bidirectional_iterator_tag>;

            using rIterator = STD::rIterator<Iterator, Bidirectional_iterator_tag>;

            using crIterator = STD::crIterator<Iterator, Bidirectional_iterator_tag>;

            using Self = Red_Black_Tree<Key, Compare, Equal_>;

        protected:
            struct Node {
                Key value;

                Node *left = nullptr, *right = nullptr, *last = nullptr;

                bool Red = true;

                template <typename... Args>
                constexpr explicit Node(Args&&... args) : value(forward<Args>(args)...) {};

            };

            Node *root = nullptr;

            Size size_ = 0;

            // 判断是否为红色节点。
            static bool is_Red(Node *node) {
                if (!node) return false;
                else return node->Red;
            };

            // 将target_val位置的值换成val的值。
            void swap_value(Node *val, Node *target_val) {
                link(val, true, target_val->left);
                link(val, false, target_val->right);
                if (target_val->last)
                    link(target_val->last, target_val->last->left == target_val, val);
                else val->last = nullptr;
                val->Red = target_val->Red;
                if (root == target_val) root = val;
            }

            // 内置版本的插入函数。
            Node* insert(Node *node, Node *target, Node *& judge);

            // 内置递归版本的删除函数。
            Node* erase(Node *node, const Key& key, Node *& val, Node *& target_val);

            // 找到该节点下的最小值。
            static Node* the_min(Node *node);

            // 找到该节点下的最大值。
            static Node* the_max(Node *node);

            // 把target节点下的分支复制到parent的下面。
            static Node* copy(Node *target, Node *parent);

            // 得到某个节点的迭代器，主要是为了子类服务。。
            Iterator get_Iterator(Node *temp) const { return Iterator(temp, const_cast<Self *>(this)); };

        private:
            Compare less;

            Equal_ equal;

            // link操作是为了更新子节点的last，并将子节点连接到父节点上。
            static void link(Node *parent, bool left, Node *child) {
                if (parent) {
                    if (left) parent->left = child;
                    else parent->right = child;
                }
                if (child) child->last = parent;
            };

            // 左旋
            Node* L_alpha(Node *node) {
                Node *n_right = node->right;
                link(node, false, n_right->left);
                link(n_right, true, node);
                n_right->Red = node->Red;
                node->Red = true;
                return n_right;
            };

            // 右旋
            Node* R_alpha(Node *node) {
                Node *n_left = node->left;
                link(node, true, n_left->right);
                link(n_left, false, node);
                n_left->Red = node->Red;
                node->Red = true;
                return n_left;
            };

            // 插入时的反转颜色，也就是将临时的4节点分裂出两个2节点。
            static void insert_change_Color(Node *node) {
                node->Red = true;
                node->left->Red = false;
                node->right->Red = false;
            };

            // 删除时的反转颜色，也就是创建一个临时4节点。
            static void erase_change_Color(Node *node) {
                node->Red = false;
                node->left->Red = true;
                node->right->Red = true;
            };

            // 用作建立临时4节点。
            Node* erase_left_adjustment(Node *node);

            // 用作建立临时4节点。
            Node* erase_right_adjustment(Node *node);

            // 删除该节点下的最小值。
            Node* erase_min(Node *node, Node *& val);

            // 该函数的作用是在树被破坏时，重新平衡树。
            Node* balance(Node *node);

            // 从该节点下获取某个值。
            Node* get_from(Node *node, const Key& key) const;

            // 将该节点的全部分支删除。
            void erase_from(Node *node);

        public:
            explicit Red_Black_Tree(Compare compare = Compare(), Equal_ equal = Equal_())
                : less(compare), equal(equal) {};

            Red_Black_Tree(const std::initializer_list<Key>& list, Compare compare = Compare(),
                           Equal_ equal = Equal_());

            template <typename Input_Key>
            Red_Black_Tree(Input_Key k_begin, const Input_Key& k_end, Compare compare = Compare(),
                           Equal_ equal = Equal_());

            Red_Black_Tree(const Self& other);

            Red_Black_Tree(Self&& other) noexcept;

            virtual ~Red_Black_Tree();

            void clear();

            template <typename... Input_Key>
            Pair<Iterator, bool> emplace(Input_Key&&... args);

            Pair<Iterator, bool> insert(const Key& key);

            bool erase(const Key& key);

            void erase(const Iterator& key);

            void erase(const cIterator& key);

            void erase(const rIterator& key);

            void erase(const crIterator& key);

            Iterator find(const Key& key);

            cIterator find(const Key& key) const;

            Iterator at(const Key& key);

            cIterator at(const Key& key) const;

            Key& operator[](const Key& key);

            Size count(const Key& key) const;

            bool contains(const Key& key) const;

            Pair<Iterator, Iterator> equal_range(const Key& key);

            Iterator lower_bound(const Key& key);

            Iterator upper_bound(const Key& key);

            Pair<cIterator, cIterator> equal_range(const Key& key) const;

            cIterator lower_bound(const Key& key) const;

            cIterator upper_bound(const Key& key) const;

            Size size() const { return size_; };

            bool empty() const { return !size_; };

            void swap(Self& other) noexcept {
                Size temp1 = size_;
                Node *temp2 = root;
                root = other.root;
                size_ = other.size_;
                other.size_ = temp1;
                other.root = temp2;
                STD::swap(less, other.less);
                STD::swap(equal, other.equal);
            }

            // 这两个函数不会复制Compare和Equal_对象。
            Self& operator=(const Self& other);

            Self& operator=(Self&& other) noexcept;

            Iterator begin() const { return Iterator(the_min(root), const_cast<Self *>(this)); };

            Iterator end() const { return Iterator(nullptr, const_cast<Self *>(this)); };

            cIterator cbegin() const { return cIterator(Iterator(the_min(root), const_cast<Self *>(this))); };

            cIterator cend() const { return cIterator(Iterator(nullptr, const_cast<Self *>(this))); };

            rIterator rbegin() const { return rIterator(Iterator(the_max(root), const_cast<Self *>(this))); };

            rIterator rend() const { return rIterator(Iterator(nullptr, const_cast<Self *>(this))); };

            crIterator crbegin() const { return crIterator(Iterator(the_max(root), const_cast<Self *>(this))); };

            crIterator crend() const { return crIterator(Iterator(nullptr, const_cast<Self *>(this))); };

            friend bool operator==(const Self& left, const Self& right) {
                if (left.size_ != right.size_) return false;
                auto l = left.begin(), r = right.begin();
                Size size = left.size_;
                while (size) {
                    if (*l != *r) return false;
                    --size;
                }
                return true;
            }

            friend bool operator!=(const Self& left, const Self& right) {
                return !(left == right);
            }

            friend bool operator<(const Self& left, const Self& right) {
                auto l = left.begin(), r = right.begin(), l_end = left.end(), r_end = right.end();
                while (l != l_end && r != r_end) {
                    if (*l != *r) return *l < *r;
                    ++l, ++r;
                }
                return l == l_end && r != r_end;
            }

            friend bool operator<=(const Self& left, const Self& right) {
                return !(left > right);
            }

            friend bool operator>(const Self& left, const Self& right) {
                auto l = left.begin(), r = right.begin(), l_end = left.end(), r_end = right.end();
                while (l != l_end && r != r_end) {
                    if (*l != *r) return *l > *r;
                    ++l, ++r;
                }
                return l != l_end && r == r_end;
            }

            friend bool operator>=(const Self& left, const Self& right) {
                return !(left < right);
            }

            friend inline void swap(Self& left, Self& right) noexcept {
                left.swap(right);
            }

        };

//----------------------------------------------------------------------------------------------------------------------

        template <typename Key, typename Compare, typename Equal>
        typename Red_Black_Tree<Key, Compare, Equal>::Node*
        Red_Black_Tree<Key, Compare, Equal>::the_min(Node *node) {
            if (!node) return nullptr;
            while (node->left) node = node->left;
            return node;
        }

        template <typename Key, typename Compare, typename Equal>
        typename Red_Black_Tree<Key, Compare, Equal>::Node*
        Red_Black_Tree<Key, Compare, Equal>::the_max(Node *node) {
            if (!node) return nullptr;
            while (node->right) node = node->right;
            return node;
        }

        template <typename Key, typename Compare, typename Equal_>
        void Red_Black_Tree<Key, Compare, Equal_>::erase_from(Node *node) {
            if (!node) return;
            erase_from(node->left);
            erase_from(node->right);
            Deallocate(node);
            --size_;
        }

        template <typename Key, typename Compare, typename Equal>
        typename Red_Black_Tree<Key, Compare, Equal>::Node*
        Red_Black_Tree<Key, Compare, Equal>::get_from(Node *node, const Key& key) const {
            while (node && !equal(key, node->value)) {
                if (less(key, node->value)) node = node->left;
                else node = node->right;
            }
            return node;
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Node*
        Red_Black_Tree<Key, Compare, Equal_>::copy(Node *target, Node *parent) {
            if (!target) return nullptr;
            Node *temp = Allocate<Node>(target->value);
            temp->Red = target->Red;
            temp->left = copy(target->left, temp);
            temp->right = copy(target->right, temp);
            temp->last = parent;
            return temp;
        }

        // 该树为左倾红黑树。
        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Node*
        Red_Black_Tree<Key, Compare, Equal_>::insert(Node *node, Node *target, Node *& judge) {
            if (!node) { // 说明找到了合适的位置插入，将目标向上传递，终止递归。
                ++size_;
                return target;
            }
            if (less(target->value, node->value)) // 目标值小于当前节点。
                link(node, true, insert(node->left, target, judge));
            else if (!equal(target->value, node->value)) // 目标值大于当前节点。
                link(node, false, insert(node->right, target, judge));
            else { // 目标值等于当前节点，给本节点重新赋值。
                judge = node;
                return node;
            }
            // 当前节点出现红色右倾节点，违反左倾策略，左旋之后，两个子节点均为黑色，父节点为红色。
            if (!is_Red(node->left) && is_Red(node->right)) node = L_alpha(node);
            // 当前的节点子节点出现左边红节点相连的情况，此时node一定是黑色的，右旋建立一个临时4节点。
            if (is_Red(node->left) && is_Red(node->left->left)) node = R_alpha(node);
            // 当前节点出现临时4节点，将它分裂。
            if (is_Red(node->left) && is_Red(node->right)) insert_change_Color(node);
            return node;
        }

        template <typename Key, typename Compare, typename Equal>
        typename Red_Black_Tree<Key, Compare, Equal>::Node*
        Red_Black_Tree<Key, Compare, Equal>::erase_left_adjustment(Node *node) {
            erase_change_Color(node);
            if (is_Red(node->right->left)) { // 上面建立了临时4节点，这里出现了红节点相连的状况，经过下面的操作可重新建立临时4节点。
                link(node, false, R_alpha(node->right));
                node = L_alpha(node);
            }
            return node;
        }

        template <typename Key, typename Compare, typename Equal>
        typename Red_Black_Tree<Key, Compare, Equal>::Node*
        Red_Black_Tree<Key, Compare, Equal>::erase_right_adjustment(Node *node) {
            erase_change_Color(node);
            if (is_Red(node->left->left)) { // 上面建立了临时4节点，这里出现了红节点相连的状况，经过下面的操作可重新建立临时4节点。
                node = R_alpha(node);
            }
            return node;
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Node*
        Red_Black_Tree<Key, Compare, Equal_>::balance(Node *node) {
            if (is_Red(node->right))  // 当前节点出现红色右倾节点，违反左倾策略，左旋之后，两个子节点均为黑色，父节点为红色。
                node = L_alpha(node);
            if (!is_Red(node->left) && is_Red(node->right)) node = L_alpha(node);
            if (is_Red(node->left) && is_Red(node->left->left)) node = R_alpha(node);
            if (is_Red(node->left) && is_Red(node->right)) insert_change_Color(node);
            return node;
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Node*
        Red_Black_Tree<Key, Compare, Equal_>::erase_min(Node *node, Node *& val) {
            if (!node->left) { // 找到最小值，直接删除。
                val = node;
                --size_;
                return nullptr;
            }
            if (!is_Red(node->left) && !is_Red(node->left->left)) // 两个子节点都是2节点，建立临时4节点。
                node = erase_left_adjustment(node);
            link(node, true, erase_min(node->left, val)); // 不满足条件，继续递归。
            return balance(node);
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Node*
        Red_Black_Tree<Key, Compare, Equal_>::erase(Node *node, const Key& key, Node *& val, Node *& target_val) {
            if (less(key, node->value)) { // 当前节点小于目标值。
                if (!is_Red(node->left) && !is_Red(node->left->left)) // 两个子节点都是2节点，建立临时4节点。
                    node = erase_left_adjustment(node);
                link(node, true, erase(node->left, key, val, target_val));
            } else { // 当前节点大于或等于目标值。
                if (is_Red(node->left)) // 左节点为非2节点，右旋建立一个3节点。
                    node = R_alpha(node);
                if (equal(key, node->value) && !node->right) { // 要删除的节点为底部节点，直接删除。
                    Deallocate(node);
                    --size_;
                    return nullptr;
                }
                if (!is_Red(node->right) && !is_Red(node->right->left)) // 两个子节点都是2节点，建立临时4节点。
                    node = erase_right_adjustment(node);
                if (equal(key, node->value)) { // 要删除的节点有子节点，置换其右边节点的最小值，转而删除被置换的那个底部节点。
                    target_val = node;
                    link(node, false, erase_min(node->right, val));
                } else { // 当前节点不是目标值，继续递归。
                    link(node, false, erase(node->right, key, val, target_val));
                }
            }
            return balance(node);
        }

//----------------------------------------------------------------------------------------------------------------------

        template <typename Key, typename Compare, typename Equal_>
        Red_Black_Tree<Key, Compare, Equal_>::Red_Black_Tree
        (const std::initializer_list<Key>& list, Compare compare, Equal_ equal)
            : less(compare), equal(equal) {
            for (const Key& t : list) {
                insert(t);
            }
        }

        template <typename Key, typename Compare, typename Equal_>
        template <typename Input_Key>
        Red_Black_Tree<Key, Compare, Equal_>::Red_Black_Tree
        (Input_Key k_begin, const Input_Key& k_end, Compare compare, Equal_ equal) :
            less(compare), equal(equal) {
            while (k_begin != k_end) {
                insert(*k_begin);
                ++k_begin;
            }
        }

        template <typename Key, typename Compare, typename Equal_>
        Red_Black_Tree<Key, Compare, Equal_>::Red_Black_Tree(const Self& other) :
            size_(other.size_), less(other.less), equal(other.equal) {
            root = copy(other.root, nullptr);
        }

        template <typename Key, typename Compare, typename Equal_>
        Red_Black_Tree<Key, Compare, Equal_>::Red_Black_Tree(Self&& other) noexcept
            : root(other.root), size_(other.size_), less(other.less), equal(other.equal) {
            other.size_ = 0;
            other.root = nullptr;
        }

        template <typename Key, typename Compare, typename Equal_>
        Red_Black_Tree<Key, Compare, Equal_>::~Red_Black_Tree() {
            clear();
        }

        template <typename Key, typename Compare, typename Equal_>
        void Red_Black_Tree<Key, Compare, Equal_>::clear() {
            erase_from(root);
        }

        template <typename Key, typename Compare, typename Equal_>
        template <typename... Input_Key>
        Pair<typename Red_Black_Tree<Key, Compare, Equal_>::Iterator, bool>
        Red_Black_Tree<Key, Compare, Equal_>::emplace(Input_Key&&... args) {
            Size record = size_;
            //插入节点为红节点，可以强迫结构的变换。
            Node *temp = Allocate<Node>(forward<args>(args)...), *judge = nullptr;
            root = insert(root, temp, judge);
            root->Red = false;
            root->last = nullptr;
            if (judge) {
                swap_value(temp, judge);
                Deallocate(judge);
            }
            return { Iterator(temp, this), size_ != record };
        }

        template <typename Key, typename Compare, typename Equal_>
        Pair<typename Red_Black_Tree<Key, Compare, Equal_>::Iterator, bool>
        Red_Black_Tree<Key, Compare, Equal_>::insert(const Key& key) {
            Size record = size_;
            //插入节点为红节点，可以强迫结构的变换。
            Node *temp = Allocate<Node>(key), *judge = nullptr;
            root = insert(root, temp, judge);
            root->Red = false;
            root->last = nullptr;
            if (judge) {
                swap_value(temp, judge);
                Deallocate(judge);
            }
            return { Iterator(temp, this), size_ != record };
        }

        template <typename Key, typename Compare, typename Equal_>
        bool Red_Black_Tree<Key, Compare, Equal_>::erase(const Key& key) {
            Size record = size_;
            if (!is_Red(root->left) && !is_Red(root->right))
                root->Red = true;
            Node *val = nullptr, *target_val = nullptr;
            root = erase(root, key, val, target_val);
            if (root) {
                root->Red = false;
                root->last = nullptr;
            }
            if (target_val) {
                swap_value(val, target_val);
                Deallocate(target_val);
            }
            return record != size_;
        }

        template <typename Key, typename Compare, typename Equal_>
        void Red_Black_Tree<Key, Compare, Equal_>::erase(const Iterator& key) {
            erase(key.target->value);
        }

        template <typename Key, typename Compare, typename Equal_>
        void Red_Black_Tree<Key, Compare, Equal_>::erase(const cIterator& key) {
            erase(key.target.target->value);
        }

        template <typename Key, typename Compare, typename Equal_>
        void Red_Black_Tree<Key, Compare, Equal_>::erase(const rIterator& key) {
            erase(key.target.target->value);
        }

        template <typename Key, typename Compare, typename Equal_>
        void Red_Black_Tree<Key, Compare, Equal_>::erase(const crIterator& key) {
            erase(key.target.target->value);
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Iterator
        Red_Black_Tree<Key, Compare, Equal_>::find(const Key& key) {
            return Iterator(get_from(root, key), this);
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::cIterator
        Red_Black_Tree<Key, Compare, Equal_>::find(const Key& key) const {
            return cIterator(Iterator(get_from(root, key), const_cast<Self *>(this)));
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Iterator
        Red_Black_Tree<Key, Compare, Equal_>::at(const Key& key) {
            auto temp = get_from(root, key);
            if (!temp)
                throw outOfRange("You passed an out-of-range basic in at()");
            return Iterator(temp, this);
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::cIterator
        Red_Black_Tree<Key, Compare, Equal_>::at(const Key& key) const {
            auto temp = get_from(root, key);
            if (!temp)
                throw outOfRange("You passed an out-of-range basic in at()");
            return cIterator(Iterator(temp, const_cast<Self *>(this)));
        }

        template <typename Key, typename Compare, typename Equal_>
        Key& Red_Black_Tree<Key, Compare, Equal_>::operator[](const Key& key) {
            auto ptr = get_from(root, key);
            if (ptr) return ptr->value;
            return *insert(key).first;
        }

        template <typename Key, typename Compare, typename Equal_>
        Size Red_Black_Tree<Key, Compare, Equal_>::count(const Key& key) const {
            return get_from(root, key);
        }

        template <typename Key, typename Compare, typename Equal_>
        bool Red_Black_Tree<Key, Compare, Equal_>::contains(const Key& key) const {
            return get_from(root, key);
        }

        template <typename Key, typename Compare, typename Equal_>
        Pair<typename Red_Black_Tree<Key, Compare, Equal_>::Iterator,
             typename Red_Black_Tree<Key, Compare, Equal_>::Iterator>
        Red_Black_Tree<Key, Compare, Equal_>::equal_range(const Key& key) {
            return { lower_bound(key), upper_bound(key) };
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Iterator
        Red_Black_Tree<Key, Compare, Equal_>::lower_bound(const Key& key) {
            Node *temp = root;
            while (temp) {
                if (less(key, temp->value)) {
                    if (temp->left) temp = temp->left;
                    else return Iterator(temp, this);
                } else if (equal(temp->value, key)) {
                    return Iterator(temp, this);
                } else {
                    temp = temp->right;
                }
            }
            return end();
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Iterator
        Red_Black_Tree<Key, Compare, Equal_>::upper_bound(const Key& key) {
            Node *temp = root;
            while (temp) {
                if (less(key, temp->value)) {
                    if (temp->left) temp = temp->left;
                    else return Iterator(temp, this);
                } else if (equal(temp->value, key)) {
                    return ++Iterator(temp, this);
                } else {
                    temp = temp->right;
                }
            }
            return end();
        }

        template <typename Key, typename Compare, typename Equal_>
        Pair<typename Red_Black_Tree<Key, Compare, Equal_>::cIterator,
             typename Red_Black_Tree<Key, Compare, Equal_>::cIterator>
        Red_Black_Tree<Key, Compare, Equal_>::equal_range(const Key& key) const {
            return { lower_bound(key), upper_bound(key) };
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::cIterator
        Red_Black_Tree<Key, Compare, Equal_>::lower_bound(const Key& key) const {
            return cIterator(const_cast<Self *>(this)->lower_bound(key));
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::cIterator
        Red_Black_Tree<Key, Compare, Equal_>::upper_bound(const Key& key) const {
            return cIterator(const_cast<Self *>(this)->upper_bound(key));
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Self
        & Red_Black_Tree<Key, Compare, Equal_>::operator=(const Self& other) {
            if (this == &other) return *this;
            clear();
            size_ = other.size_;
            root = copy(other.root, nullptr);
            return *this;
        }

        template <typename Key, typename Compare, typename Equal_>
        typename Red_Black_Tree<Key, Compare, Equal_>::Self
        & Red_Black_Tree<Key, Compare, Equal_>::operator=(Self&& other) noexcept {
            if (this == &other) return *this;
            size_ = other.size_;
            root = other.root;
            other.size_ = 0;
            other.root = nullptr;
            return *this;
        }
    }
}

#endif //TINYSTL_RED_BLACK_TREE_HPP
