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

        /* 该迭代器end()和rend()和cend()和crend()指向的位置相同，都无法被解引用。这使得该迭代器
         * 的行为更像一个环形迭代器，带来隐患的同时，也可以进行一些特殊的操作。使用时要多加注意。
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

        };

        Node *root = nullptr, *val_begin = nullptr, *val_end = nullptr;

        Compare less = Less<Key>();

        Equal_ equal = Equal<Key>();

        Size size_ = 0;

        // 判断是否为红色节点。
        bool is_Red(Node *node) const {
            if (!node) return false;
            else return node->Red;
        };

        // link操作是为了更新子节点的last，并将子节点连接到父节点上。
        void link(Node *parent, bool left, Node *child) const {
            if (parent) {
                if (left) parent->left = child;
                else parent->right = child;
            }
            if (child) child->last = parent;
        };

        // 左旋
        Node *L_alpha(Node *node) {
            Node *n_right = node->right;
            link(node, false, n_right->left);
            link(n_right, true, node);
            n_right->Red = node->Red;
            node->Red = true;
            return n_right;
        };

        // 右旋
        Node *R_alpha(Node *node) {
            Node *n_left = node->left;
            link(node, true, n_left->right);
            link(n_left, false, node);
            n_left->Red = node->Red;
            node->Red = true;
            return n_left;
        };

        // 插入时的反转颜色，也就是将临时的4节点分裂出两个2节点。
        void insert_change_Color(Node *node) const {
            node->Red = true;
            node->left->Red = false;
            node->right->Red = false;
        };

        // 删除时的反转颜色，也就是创建一个临时4节点。
        void erase_change_Color(Node *node) const {
            node->Red = false;
            node->left->Red = true;
            node->right->Red = true;
        };

        // 找到该节点下的最小值。
        static Node *the_min(Node *node);

        // 找到该节点下的最大值。
        static Node *the_max(Node *node);

        // 内置版本的插入函数。
        Node *insert(Node *node, Node *&target);

        // 用作建立临时4节点。
        Node *erase_left_adjustment(Node *node);

        // 用作建立临时4节点。
        Node *erase_right_adjustment(Node *node);

        // 删除该节点下的最小值。
        Node *erase_min(Node *node);

        // 该函数的作用是在树被破坏时，重新平衡树。
        Node *balance(Node *node);

        // 内置递归版本的删除函数。
        Node *erase(Node *node, const Key &key);

        // 从该节点下获取某个值。
        Node *get_from(Node *node, const Key &key) const;

        // 将该节点的全部分支删除。
        void erase_from(Node *node);

        // 把target节点下的分支复制到parent的下面。
        Node *copy(Node *target, Node *parent);

    public:
        Red_Black_Tree() = default;

        Red_Black_Tree(const Self &other);

        Red_Black_Tree(Self &&other) noexcept;

        template<typename Input_Key, typename Input_Val>
        Red_Black_Tree(Input_Key k_begin, const Input_Key &k_end,
                       Input_Val v_begin);

        ~Red_Black_Tree();

        void clear();

        Pair<Iterator, bool> insert(const Key &key, const Val &value);

        Pair<Iterator, bool> insert(const Key &key, Val &&value);

        bool erase(const Key &key);

        Iterator find(const Key &key) const;

        Val &operator[](const Key &key);

        Size size() const { return size_; };

        bool empty() const { return !size_; };

        void swap(Self &other) noexcept {
            Size temp1 = size_;
            Node *temp2 = val_begin, *temp3 = val_end;
            size_ = other.size_;
            val_begin = other.val_begin;
            val_end = other.val_end;
            other.size_ = temp1;
            other.val_begin = temp2;
            other.val_end = temp3;
            STD::swap(less, other.less);
            STD::swap(equal, other.equal);
        }

        Self &operator=(const Self &other);

        Self &operator=(Self &&other) noexcept;

        Iterator begin() const { return Iterator(val_begin, const_cast<Self *>(this)); };

        Iterator end() const { return Iterator(nullptr, const_cast<Self *>(this)); };

        cIterator cbegin() const { return cIterator(Iterator(val_begin, const_cast<Self *>(this))); };

        cIterator cend() const { return cIterator(Iterator(nullptr, const_cast<Self *>(this))); };

        rIterator rbegin() const { return rIterator(Iterator(val_end, const_cast<Self *>(this))); };

        rIterator rend() const { return rIterator(Iterator(nullptr, const_cast<Self *>(this))); };

        crIterator crbegin() const { return crIterator(Iterator(val_end, const_cast<Self *>(this))); };

        crIterator crend() const { return crIterator(Iterator(nullptr, const_cast<Self *>(this))); };

        friend bool operator==(const Self &left, const Self &right);

        friend bool operator!=(const Self &left, const Self &right);

        friend bool operator<(const Self &left, const Self &right);

        friend bool operator<=(const Self &left, const Self &right);

        friend bool operator>(const Self &left, const Self &right);

        friend bool operator>=(const Self &left, const Self &right);

        friend inline void swap(Self &left, Self &right) noexcept;

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::the_min(Node *node) {
        if (!node) return nullptr;
        while (node->left) node = node->left;
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::the_max(Node *node) {
        if (!node) return nullptr;
        while (node->right) node = node->right;
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    void Red_Black_Tree<Key, Val, Compare, Equal_>::erase_from(Node *node) {
        if (!node) return;
        erase_from(node->left);
        erase_from(node->right);
        Deallocate(node);
        --size_;
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

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Red_Black_Tree<Key, Val, Compare, Equal_>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal_>::copy(Node *target, Node *parent) {
        if (!target) return nullptr;
        Node *temp = Allocate<Node>(target->value.first, target->value.second, target->Red);
        temp->left = copy(target->left, temp);
        temp->right = copy(target->right, temp);
        temp->last = parent;
        return temp;
    }

    // 该树为左倾红黑树。
    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Red_Black_Tree<Key, Val, Compare, Equal_>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal_>::insert(Node *node, Node *&target) {
        if (!node) { // 说明找到了合适的位置插入，将目标向上传递，终止递归。
            ++size_;
            return target;
        }
        if (less(target->key(), node->key())) // 目标值小于当前节点。
            link(node, true, insert(node->left, target));
        else if (!equal(target->key(), node->key())) // 目标值大于当前节点。
            link(node, false, insert(node->right, target));
        else { // 目标值等于当前节点，给本节点重新赋值。
            node->value = move(target->value);
            Deallocate(target);
            target = node;
        }
        // 当前节点出现红色右倾节点，违反左倾策略，左旋之后，两个子节点均为黑色，父节点为红色。
        if (!is_Red(node->left) && is_Red(node->right)) node = L_alpha(node);
        // 当前的节点子节点出现左边红节点相连的情况，此时node一定是黑色的，右旋建立一个临时4节点。
        if (is_Red(node->left) && is_Red(node->left->left)) node = R_alpha(node);
        // 当前节点出现临时4节点，将它分裂。
        if (is_Red(node->left) && is_Red(node->right)) insert_change_Color(node);
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::erase_left_adjustment(Node *node) {
        erase_change_Color(node);
        if (is_Red(node->right->left)) { // 上面建立了临时4节点，这里出现了红节点相连的状况，经过下面的操作可重新建立临时4节点。
            link(node, false, R_alpha(node->right));
            node = L_alpha(node);
        }
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal>
    typename Red_Black_Tree<Key, Val, Compare, Equal>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal>::erase_right_adjustment(Node *node) {
        erase_change_Color(node);
        if (is_Red(node->left->left)) { // 上面建立了临时4节点，这里出现了红节点相连的状况，经过下面的操作可重新建立临时4节点。
            node = R_alpha(node);
        }
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Red_Black_Tree<Key, Val, Compare, Equal_>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal_>::balance(Node *node) {
        if (is_Red(node->right))  // 当前节点出现红色右倾节点，违反左倾策略，左旋之后，两个子节点均为黑色，父节点为红色。
            node = L_alpha(node);
        if (!is_Red(node->left) && is_Red(node->right)) node = L_alpha(node);
        if (is_Red(node->left) && is_Red(node->left->left)) node = R_alpha(node);
        if (is_Red(node->left) && is_Red(node->right)) insert_change_Color(node);
        return node;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Red_Black_Tree<Key, Val, Compare, Equal_>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal_>::erase_min(Node *node) {
        if (!node->left) { // 找到最小值，直接删除。
            Deallocate(node);
            --size_;
            return nullptr;
        }
        if (!is_Red(node->left) && !is_Red(node->left->left)) // 两个子节点都是2节点，建立临时4节点。
            node = erase_left_adjustment(node);
        link(node, true, erase_min(node->left)); // 不满足条件，继续递归。
        return balance(node);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Red_Black_Tree<Key, Val, Compare, Equal_>::Node *
    Red_Black_Tree<Key, Val, Compare, Equal_>::erase(Node *node, const Key &key) {
        if (less(key, node->key())) { // 当前节点小于目标值。
            if (!is_Red(node->left) && !is_Red(node->left->left)) // 两个子节点都是2节点，建立临时4节点。
                node = erase_left_adjustment(node);
            link(node, true, erase(node->left, key));
        } else { // 当前节点大于或等于目标值。
            if (is_Red(node->left)) // 左节点为非2节点，右旋建立一个3节点。
                node = R_alpha(node);
            if (equal(key, node->key()) && !node->right) { // 要删除的节点为底部节点，直接删除。
                Deallocate(node);
                --size_;
                return nullptr;
            }
            if (!is_Red(node->right) && !is_Red(node->right->left)) // 两个子节点都是2节点，建立临时4节点。
                node = erase_right_adjustment(node);
            if (equal(key, node->key())) { // 要删除的节点有子节点，置换其右边节点的最小值，转而删除被置换的那个底部节点。
                STD::swap(node->value, the_min(node->right)->value);
                link(node, false, erase_min(node->right));
            } else { // 当前节点不是目标值，继续递归。
                link(node, false, erase(node->right, key));
            }
        }
        return balance(node);
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Red_Black_Tree<Key, Val, Compare, Equal_>::Red_Black_Tree(const Self &other) :
            size_(other.size_), less(other.less), equal(other.equal) {
        root = copy(other.root);
        val_begin = the_min(root);
        val_end = the_max(root);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Red_Black_Tree<Key, Val, Compare, Equal_>::Red_Black_Tree(Self &&other) noexcept
            : size_(other.size_), root(other.root), val_begin(other.val_begin),
              val_end(other.val_end), less(other.less), equal(other.equal) {
        other.size_ = 0;
        other.root = other.val_end = other.val_begin = nullptr;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    template<typename Input_Key, typename Input_Val>
    Red_Black_Tree<Key, Val, Compare, Equal_>::Red_Black_Tree(Input_Key k_begin, const Input_Key &k_end,
                                                              Input_Val v_begin) {
        while (k_begin != k_end) {
            insert(*k_begin, *v_begin);
            ++k_begin, ++v_begin;
        }
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Red_Black_Tree<Key, Val, Compare, Equal_>::~Red_Black_Tree() {
        clear();
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    void Red_Black_Tree<Key, Val, Compare, Equal_>::clear() {
        erase_from(root);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename Red_Black_Tree<Key, Val, Compare, Equal_>::Iterator, bool>
    Red_Black_Tree<Key, Val, Compare, Equal_>::insert(const Key &key, const Val &value) {
        Size record = size_;
        //插入节点为红节点，可以强迫结构的变换。
        Node *temp = Allocate<Node>(key, value, true);
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

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename Red_Black_Tree<Key, Val, Compare, Equal_>::Iterator, bool>
    Red_Black_Tree<Key, Val, Compare, Equal_>::insert(const Key &key, Val &&value) {
        Size record = size_;
        //插入节点为红节点，可以强迫结构的变换。
        Node *temp = Allocate<Node>(key, move(value), true);
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

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Red_Black_Tree<Key, Val, Compare, Equal_>::Iterator
    Red_Black_Tree<Key, Val, Compare, Equal_>::find(const Key &key) const {
        auto temp = root;
        while (temp && !equal(key, temp->key())) {
            if (less(key, temp->key())) temp = temp->left;
            else temp = temp->right;
        }
        return Iterator(temp, const_cast<Self *>(this));
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Val &Red_Black_Tree<Key, Val, Compare, Equal_>::operator[](const Key &key) {
        auto ptr = get_from(root, key);
        if (ptr) return ptr->value.second;
        return (*(insert(key, Val()).first)).second;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self
    &Red_Black_Tree<Key, Val, Compare, Equal_>::operator=(const Self &other) {
        if (this == &other) return *this;
        clear();
        less = other.less;
        equal = other.equal;
        root = copy(other.root);
        val_begin = the_min(root);
        val_end = the_max(root);
        return *this;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self
    &Red_Black_Tree<Key, Val, Compare, Equal_>::operator=(Self &&other) noexcept {
        if (this == &other) return *this;
        size_ = other.size_;
        root = other.root;
        val_begin = other.val_begin;
        val_end = other.val_end;
        less = other.less;
        equal = other.equal;
        other.size_ = 0;
        other.root = other.val_begin = other.val_end = nullptr;
        return *this;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool operator==(const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &left,
                    const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &right) {
        if (left.size_ != right.size_) return false;
        auto l = left.begin(), r = right.begin();
        Size size = l.size_;
        while (size) {
            if (*l != *r) return false;
            --size;
        }
        return true;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool operator!=(const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &left,
                    const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &right) {
        return !(left == right);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool operator<(const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &left,
                   const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &right) {
        auto l = left.begin(), r = right.begin(), l_end = left.end(), r_end = right.end();
        while (l != l_end && r != r_end) {
            if (*l != *r) return *l < *r;
            ++l, ++r;
        }
        return l == l_end && r != r_end;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool operator<=(const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &left,
                    const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &right) {
        return !(left > right);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool operator>(const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &left,
                   const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &right) {
        auto l = left.begin(), r = right.begin(), l_end = left.end(), r_end = right.end();
        while (l != l_end && r != r_end) {
            if (*l != *r) return *l > *r;
            ++l, ++r;
        }
        return l != l_end && r == r_end;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool operator>=(const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &left,
                    const typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &right) {
        return !(left < right);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    void swap(typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &left,
              typename Red_Black_Tree<Key, Val, Compare, Equal_>::Self &right) noexcept {
        left.swap(right);
    }

}

#endif //TINYSTL_RED_BLACK_TREE_HPP
