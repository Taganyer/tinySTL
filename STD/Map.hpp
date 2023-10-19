//
// Created by 86152 on 2023/10/5.
//

#ifndef TINYSTL_MAP_HPP
#define TINYSTL_MAP_HPP

#include "Detail/Red_Black_Tree.hpp"
#include "Detail/MultiRed_Black_Tree.hpp"

namespace STD {

    namespace Detail {

        template<typename Key, typename Val, typename Compare>
        struct Map_compare_Helper {
            Compare compare;

            explicit Map_compare_Helper(Compare compare = Compare()) : compare(compare) {};

            bool operator()(const Pair<const Key, Val> &left,
                            const Pair<const Key, Val> &right) const {
                return compare(left.first, right.first);
            }
        };

        template<typename Key, typename Val, typename Equal_>
        struct Map_equal_Helper {
            Equal_ equal;

            explicit Map_equal_Helper(Equal_ equal = Equal_()) : equal(equal) {};

            bool operator()(const Pair<const Key, Val> &left,
                            const Pair<const Key, Val> &right) const {
                return equal(left.first, right.first);
            }
        };

    }


    template<typename Key, typename Val, typename Compare = Less<Key>, typename Equal_ = Equal<Key>>
    class Map : public Detail::Red_Black_Tree<Pair<const Key, Val>, Detail::Map_compare_Helper<Key, Val, Compare>,
            Detail::Map_equal_Helper<Key, Val, Equal_>> {
    public:
        using Cmp = Detail::Map_compare_Helper<Key, Val, Compare>;

        using Eql = Detail::Map_equal_Helper<Key, Val, Equal_>;

        using Map_Type = Pair<const Key, Val>;

        using Key_Type = Key;

        using Value_Type = Val;

        using Basic = Detail::Red_Black_Tree<Map_Type, Cmp, Eql>;

        using Self = Map<Key, Val, Compare, Equal_>;

        using Iterator = typename Basic::Iterator;

        using cIterator = typename Basic::cIterator;

        using rIterator = typename Basic::rIterator;

        using crIterator = typename Basic::crIterator;

        explicit Map(Compare compare = Compare(), Equal_ equal = Equal_()) :
                Basic(Cmp(compare), Eql(equal)), less(compare), equal(equal) {};

        Map(const std::initializer_list<Map_Type> &list, Compare compare = Compare(),
            Equal_ equal = Equal_());

        template<typename Key_Input, typename Val_Input>
        Map(Key_Input key_begin, const Key_Input &key_end, Val_Input val_begin,
            Compare compare = Compare(), Equal_ equal = Equal_());

        Map(const Self &other) : Basic(other), less(other.less), equal(other.equal) {};

        Map(Self &&other) noexcept: Basic(move(other)), less(other.less), equal(other.equal) {};

        ~Map() override = default;

        using Basic::insert;

        Pair<Iterator, bool> insert(const Key &key, const Val &val);

        Pair<Iterator, bool> insert(const Key &key, Val &&val);

        Pair<Iterator, bool> insert(Key &&key, const Val &val);

        Pair<Iterator, bool> insert(Key &&key, Val &&val);

        using Basic::erase;

        bool erase(const Key &key);

        Iterator find(const Key &key);

        cIterator find(const Key &key) const;

        Iterator at(const Key &key);

        cIterator at(const Key &key) const;

        Val &operator[](const Key &key);

        Size count(const Key &key) const;

        bool contains(const Key &key) const;

        Pair<Iterator, Iterator> equal_range(const Key &key);

        Iterator lower_bound(const Key &key);

        Iterator upper_bound(const Key &key);

        Pair<cIterator, cIterator> equal_range(const Key &key) const;

        cIterator lower_bound(const Key &key) const;

        cIterator upper_bound(const Key &key) const;

        Self &operator=(const Self &other) {
            Basic::operator=(other);
            return *this;
        };

        Self &operator=(Self &&other) noexcept {
            Basic::operator=(move(other));
            return *this;
        };

        void swap(Self &other) noexcept {
            Basic::swap(other);
        };

        friend void swap(Self &left, Self &right) noexcept {
            left.Basic::swap(right);
        };

    private:
        using Node = typename Basic::Node;

        using Basic::root;

        using Basic::size_;

        Compare less;

        Equal_ equal;

        Node *get_from(Node *node, const Key &key) const;

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::Node
    *Map<Key, Val, Compare, Equal_>::get_from(Node *node, const Key &key) const {
        while (node && !equal(key, node->value.first)) {
            if (less(key, node->value.first)) node = node->left;
            else node = node->right;
        }
        return node;
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Map<Key, Val, Compare, Equal_>::Map
            (const std::initializer_list<Map_Type> &list, Compare compare, Equal_ equal) :
            Basic(list, Cmp(compare), Eql(equal)), less(compare), equal(equal) {}

    template<typename Key, typename Val, typename Compare, typename Equal_>
    template<typename Key_Input, typename Val_Input>
    Map<Key, Val, Compare, Equal_>::Map(Key_Input key_begin, const Key_Input &key_end,
                                        Val_Input val_begin, Compare compare, Equal_ equal) :
            Basic(Cmp(compare), Eql(equal)), less(compare), equal(equal) {
        while (key_begin != key_end) {
            insert(*key_begin, *val_begin);
            ++key_begin, ++val_begin;
        }
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename Map<Key, Val, Compare, Equal_>::Iterator, bool>
    Map<Key, Val, Compare, Equal_>::insert(const Key &key, const Val &val) {
        Size record = size_;
        Node *temp = Allocate<Node>(key, val), *judge = nullptr;
        root = Basic::insert(root, temp, judge);
        root->Red = false;
        root->last = nullptr;
        if (judge) {
            Basic::swap_value(temp, judge);
            Deallocate(judge);
        }
        return {Basic::get_Iterator(temp), size_ != record};
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename Map<Key, Val, Compare, Equal_>::Iterator, bool>
    Map<Key, Val, Compare, Equal_>::insert(const Key &key, Val &&val) {
        Size record = size_;
        Node *temp = Allocate<Node>(key, move(val)), *judge = nullptr;
        root = Basic::insert(root, temp, judge);
        root->Red = false;
        root->last = nullptr;
        if (judge) {
            Basic::swap_value(temp, judge);
            Deallocate(judge);
        }
        return {Basic::get_Iterator(temp), size_ != record};
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename Map<Key, Val, Compare, Equal_>::Iterator, bool>
    Map<Key, Val, Compare, Equal_>::insert(Key &&key, const Val &val) {
        Size record = size_;
        Node *temp = Allocate<Node>(move(key), val), *judge = nullptr;
        root = Basic::insert(root, temp, judge);
        root->Red = false;
        root->last = nullptr;
        if (judge) {
            Basic::swap_value(temp, judge);
            Deallocate(judge);
        }
        return {Basic::get_Iterator(temp), size_ != record};
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename Map<Key, Val, Compare, Equal_>::Iterator, bool>
    Map<Key, Val, Compare, Equal_>::insert(Key &&key, Val &&val) {
        Size record = size_;
        Node *temp = Allocate<Node>(move(key), move(val)), *judge = nullptr;
        root = Basic::insert(root, temp, judge);
        root->Red = false;
        root->last = nullptr;
        if (judge) {
            Basic::swap_value(temp, judge);
            Deallocate(judge);
        }
        return {Basic::get_Iterator(temp), size_ != record};
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool Map<Key, Val, Compare, Equal_>::erase(const Key &key) {
        Size record = size_;
        if (!Basic::is_Red(root->left) && !Basic::is_Red(root->right))
            root->Red = true;
        Node *val = nullptr, *target_val = nullptr;
        root = Basic::erase(root, key, val, target_val);
        if (root) {
            root->Red = false;
            root->last = nullptr;
        }
        if (target_val) {
            Basic::swap_value(val, target_val);
            Deallocate(target_val);
        }
        return record != size_;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::Iterator
    Map<Key, Val, Compare, Equal_>::find(const Key &key) {
        return Basic::get_Iterator(get_from(root, key));
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::cIterator
    Map<Key, Val, Compare, Equal_>::find(const Key &key) const {
        return cIterator(Basic::get_Iterator(get_from(root, key)));
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::Iterator
    Map<Key, Val, Compare, Equal_>::at(const Key &key) {
        Node *ptr = get_from(root, key);
        if (!ptr)
            throw outOfRange("You passed an out-of-range basic in Map::at()");
        return Basic::get_Iterator(ptr);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::cIterator
    Map<Key, Val, Compare, Equal_>::at(const Key &key) const {
        Node *ptr = get_from(root, key);
        if (!ptr)
            throw outOfRange("You passed an out-of-range basic in Map::at()");
        return cIterator(Basic::get_Iterator(ptr));
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Val &Map<Key, Val, Compare, Equal_>::operator[](const Key &key) {
        Node *ptr = get_from(root, key);
        if (ptr) return ptr->value.second;
        else return (*Basic::insert({key, Val()}).first).second;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Size Map<Key, Val, Compare, Equal_>::count(const Key &key) const {
        return contains(key);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool Map<Key, Val, Compare, Equal_>::contains(const Key &key) const {
        return get_from(root, key);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename Map<Key, Val, Compare, Equal_>::Iterator,
            typename Map<Key, Val, Compare, Equal_>::Iterator>
    Map<Key, Val, Compare, Equal_>::equal_range(const Key &key) {
        return {lower_bound(key), upper_bound(key)};
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::Iterator
    Map<Key, Val, Compare, Equal_>::lower_bound(const Key &key) {
        Node *temp = root;
        while (temp) {
            if (less(key, temp->value.first)) {
                if (temp->left) temp = temp->left;
                else return Iterator(temp, this);
            } else if (equal(temp->value.first, key)) {
                return Iterator(temp, this);
            } else {
                temp = temp->right;
            }
        }
        return Basic::end();
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::Iterator
    Map<Key, Val, Compare, Equal_>::upper_bound(const Key &key) {
        Node *temp = root;
        while (temp) {
            if (less(key, temp->value.first)) {
                if (temp->left) temp = temp->left;
                else return Iterator(temp, this);
            } else if (equal(temp->value.first, key)) {
                return ++Iterator(temp, this);
            } else {
                temp = temp->right;
            }
        }
        return Basic::end();
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename Map<Key, Val, Compare, Equal_>::cIterator,
            typename Map<Key, Val, Compare, Equal_>::cIterator>
    Map<Key, Val, Compare, Equal_>::equal_range(const Key &key) const {
        return {lower_bound(key), upper_bound(key)};
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::cIterator
    Map<Key, Val, Compare, Equal_>::lower_bound(const Key &key) const {
        return cIterator(const_cast<Self *>(this)->lower_bound(key));
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::cIterator
    Map<Key, Val, Compare, Equal_>::upper_bound(const Key &key) const {
        return cIterator(const_cast<Self *>(this)->upper_bound(key));
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Compare = Less<Key>, typename Equal_ = Equal<Key>>
    class MultiMap : public Detail::MultiRed_Black_Tree<Pair<const Key, Val>,
            Detail::Map_compare_Helper<Key, Val, Compare>,
            Detail::Map_equal_Helper<Key, Val, Equal_>> {
    public:
        using Cmp = Detail::Map_compare_Helper<Key, Val, Compare>;

        using Eql = Detail::Map_equal_Helper<Key, Val, Equal_>;

        using Map_Type = Pair<const Key, Val>;

        using Key_Type = Key;

        using Value_Type = Val;

        using Basic = Detail::MultiRed_Black_Tree<Map_Type, Cmp, Eql>;

        using Self = MultiMap<Key, Val, Compare, Equal_>;

        using Iterator = typename Basic::Iterator;

        using cIterator = typename Basic::cIterator;

        using rIterator = typename Basic::rIterator;

        using crIterator = typename Basic::crIterator;

        explicit MultiMap(Compare compare = Compare(), Equal_ equal = Equal_()) :
                Basic(Cmp(compare), Eql(equal)), less(compare), equal(equal) {};

        MultiMap(const std::initializer_list<Map_Type> &list, Compare compare = Compare(),
                 Equal_ equal = Equal_());

        template<typename Key_Input, typename Val_Input>
        MultiMap(Key_Input key_begin, const Key_Input &key_end, Val_Input val_begin,
                 Compare compare = Compare(), Equal_ equal = Equal_());

        MultiMap(const Self &other) : Basic(other), less(other.less), equal(other.equal) {};

        MultiMap(Self &&other) noexcept: Basic(move(other)), less(other.less), equal(other.equal) {};

        ~MultiMap() override = default;

        using Basic::insert;

        Iterator insert(const Key &key, const Val &val);

        Iterator insert(const Key &key, Val &&val);

        Iterator insert(Key &&key, const Val &val);

        Iterator insert(Key &&key, Val &&val);

        using Basic::erase;

        bool erase(const Key &key);

        Iterator find(const Key &key);

        cIterator find(const Key &key) const;

        Size count(const Key &key) const;

        bool contains(const Key &key) const;

        Pair<Iterator, Iterator> equal_range(const Key &key);

        Iterator lower_bound(const Key &key);

        Iterator upper_bound(const Key &key);

        Pair<cIterator, cIterator> equal_range(const Key &key) const;

        cIterator lower_bound(const Key &key) const;

        cIterator upper_bound(const Key &key) const;

        Self &operator=(const Self &other) {
            Basic::operator=(other);
            return *this;
        };

        Self &operator=(Self &&other) noexcept {
            Basic::operator=(move(other));
            return *this;
        };

        void swap(Self &other) noexcept {
            Basic::swap(other);
        };

        friend void swap(Self &left, Self &right) noexcept {
            left.Basic::swap(right);
        };

    private:
        using Node = typename Basic::Node;

        using Joint = typename Basic::Joint;

        using Basic::root;

        using Basic::size_;

        Compare less;

        Equal_ equal;

        Node *get_from(Node *node, const Key &key) const;

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::Node
    *MultiMap<Key, Val, Compare, Equal_>::get_from(Node *node, const Key &key) const {
        while (node && !equal(key, node->list->value.first)) {
            if (less(key, node->list->value.first)) node = node->left;
            else node = node->right;
        }
        return node;
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Compare, typename Equal_>
    MultiMap<Key, Val, Compare, Equal_>::MultiMap
            (const std::initializer_list<Map_Type> &list, Compare compare, Equal_ equal) :
            Basic(list, Cmp(compare), Eql(equal)), less(compare), equal(equal) {}

    template<typename Key, typename Val, typename Compare, typename Equal_>
    template<typename Key_Input, typename Val_Input>
    MultiMap<Key, Val, Compare, Equal_>::MultiMap(Key_Input key_begin, const Key_Input &key_end,
                                                  Val_Input val_begin, Compare compare, Equal_ equal) :
            Basic(Cmp(compare), Eql(equal)), less(compare), equal(equal) {
        while (key_begin != key_end) {
            insert(*key_begin, *val_begin);
            ++key_begin, ++val_begin;
        }
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::Iterator
    MultiMap<Key, Val, Compare, Equal_>::insert(const Key &key, const Val &val) {
        Node *temp = Allocate<Node>(key, val);
        Joint *record = temp->list;
        root = Basic::insert(root, temp);
        root->Red = false;
        root->last = nullptr;
        return Basic::get_Iterator(record, temp);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::Iterator
    MultiMap<Key, Val, Compare, Equal_>::insert(const Key &key, Val &&val) {
        Node *temp = Allocate<Node>(key, move(val));
        Joint *record = temp->list;
        root = Basic::insert(root, temp);
        root->Red = false;
        root->last = nullptr;
        return Basic::get_Iterator(record, temp);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::Iterator
    MultiMap<Key, Val, Compare, Equal_>::insert(Key &&key, const Val &val) {
        Node *temp = Allocate<Node>(move(key), val);
        Joint *record = temp->list;
        root = Basic::insert(root, temp);
        root->Red = false;
        root->last = nullptr;
        return Basic::get_Iterator(record, temp);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::Iterator
    MultiMap<Key, Val, Compare, Equal_>::insert(Key &&key, Val &&val) {
        Node *temp = Allocate<Node>(move(key), move(val));
        Joint *record = temp->list;
        root = Basic::insert(root, temp);
        root->Red = false;
        root->last = nullptr;
        return Basic::get_Iterator(record, temp);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool MultiMap<Key, Val, Compare, Equal_>::erase(const Key &key) {
        Size record = size_;
        if (!Basic::is_Red(root->left) && !Basic::is_Red(root->right))
            root->Red = true;
        Node *val = nullptr, *target_val = nullptr;
        root = Basic::erase(root, {key, Val()}, val, target_val);
        if (root) {
            root->Red = false;
            root->last = nullptr;
        }
        if (target_val) {
            Basic::swap_value(val, target_val);
            Deallocate(target_val);
        }
        return record != size_;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::Iterator
    MultiMap<Key, Val, Compare, Equal_>::find(const Key &key) {
        auto temp = get_from(root, key);
        return Basic::get_Iterator(temp ? temp->list : nullptr, temp);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::cIterator
    MultiMap<Key, Val, Compare, Equal_>::find(const Key &key) const {
        auto temp = get_from(root, key);
        return cIterator(Basic::get_Iterator(temp ? temp->list : nullptr, temp));
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Size MultiMap<Key, Val, Compare, Equal_>::count(const Key &key) const {
        Node *temp = get_from(root, key);
        return temp ? temp->size() : 0;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    bool MultiMap<Key, Val, Compare, Equal_>::contains(const Key &key) const {
        return get_from(root, key);
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename MultiMap<Key, Val, Compare, Equal_>::Iterator,
            typename MultiMap<Key, Val, Compare, Equal_>::Iterator>
    MultiMap<Key, Val, Compare, Equal_>::equal_range(const Key &key) {
        return {lower_bound(key), upper_bound(key)};
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::Iterator
    MultiMap<Key, Val, Compare, Equal_>::lower_bound(const Key &key) {
        Node *temp = root;
        while (temp) {
            if (less(key, temp->list->value.first)) {
                if (temp->left) temp = temp->left;
                else return Basic::get_Iterator(temp->list, temp);
            } else if (equal(key, temp->list->value.first)) {
                return Basic::get_Iterator(temp->list, temp);
            } else {
                temp = temp->right;
            }
        }
        return Basic::end();
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::Iterator
    MultiMap<Key, Val, Compare, Equal_>::upper_bound(const Key &key) {
        Node *temp = root;
        while (temp) {
            if (less(key, temp->list->value.first)) {
                if (temp->left) temp = temp->left;
                else return Basic::get_Iterator(temp->list, temp);
            } else if (equal(key, temp->list->value.first)) {
                if (temp->right) temp = Basic::the_min(temp->right);
                else {
                    while (temp->last && temp->last->right == temp)
                        temp = temp->last;
                    temp = temp->last;
                    return Basic::get_Iterator(temp ? temp->list : nullptr, temp);
                }
            } else {
                temp = temp->right;
            }
        }
        return Basic::end();
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    Pair<typename MultiMap<Key, Val, Compare, Equal_>::cIterator,
            typename MultiMap<Key, Val, Compare, Equal_>::cIterator>
    MultiMap<Key, Val, Compare, Equal_>::equal_range(const Key &key) const {
        return {lower_bound(key), upper_bound(key)};
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::cIterator
    MultiMap<Key, Val, Compare, Equal_>::lower_bound(const Key &key) const {
        return cIterator(const_cast<Self *>(this)->lower_bound(key));
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename MultiMap<Key, Val, Compare, Equal_>::cIterator
    MultiMap<Key, Val, Compare, Equal_>::upper_bound(const Key &key) const {
        return cIterator(const_cast<Self *>(this)->upper_bound(key));
    }

}

#endif //TINYSTL_MAP_HPP
