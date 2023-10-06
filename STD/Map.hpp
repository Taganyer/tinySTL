//
// Created by 86152 on 2023/10/5.
//

#ifndef TINYSTL_MAP_HPP
#define TINYSTL_MAP_HPP

#include "Detail/Red_Black_Tree.hpp"

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
    class Map : public Red_Black_Tree<Pair<const Key, Val>, Detail::Map_compare_Helper<Key, Val, Compare>,
            Detail::Map_equal_Helper<Key, Val, Equal_>> {
    public:
        using Cmp = Detail::Map_compare_Helper<Key, Val, Compare>;

        using Eql = Detail::Map_equal_Helper<Key, Val, Equal_>;

        using Basic = Red_Black_Tree<Pair<const Key, Val>, Cmp, Eql>;

        using Iterator = typename Basic::Iterator;

        using cIterator = typename Basic::cIterator;

        using rIterator = typename Basic::rIterator;

        using crIterator = typename Basic::crIterator;

        explicit Map(Compare compare = Compare(), Equal_ equal = Equal_()) :
                Basic(Cmp(compare), Eql(equal)), less(compare), equal(equal) {};

        template<typename Key_Input, typename Val_Input>
        Map(Key_Input key_begin, const Key_Input &key_end, Val_Input val_begin,
            Compare compare = Compare(), Equal_ equal = Equal_());

        ~Map() = default;

        Pair<Iterator, bool> insert(const Key &key, const Val &val);

        Pair<Iterator, bool> insert(const Key &key, Val &&val);

        Pair<Iterator, bool> insert(Key &&key, const Val &val);

        Pair<Iterator, bool> insert(Key &&key, Val &&val);

        bool erase(const Key &key);

        Val &operator[](const Key &key) const;

        Iterator find(const Key &key) const;

        using Basic::size;

        using Basic::empty;

        using Basic::swap;

        using Basic::clear;

        using Basic::operator=;

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
    Val &Map<Key, Val, Compare, Equal_>::operator[](const Key &key) const {
        Node *ptr = get_from(root, key);
        if (ptr) return ptr->value.second;
        else return (*Basic::insert({key, Val()}).first).second;
    }

    template<typename Key, typename Val, typename Compare, typename Equal_>
    typename Map<Key, Val, Compare, Equal_>::Iterator
    Map<Key, Val, Compare, Equal_>::find(const Key &key) const {
        Node *ptr = get_from(root, key);
        return Basic::get_Iterator(ptr);
    }

}

#endif //TINYSTL_MAP_HPP
