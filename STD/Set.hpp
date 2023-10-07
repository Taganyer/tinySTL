//
// Created by 86152 on 2023/10/5.
//

#ifndef TINYSTL_SET_HPP
#define TINYSTL_SET_HPP

#include "Detail/Red_Black_Tree.hpp"

namespace STD {

    template<typename Key, typename Compare = Less<Key>, typename Equal_ = Equal<Key>>
    class Set : public Detail::Red_Black_Tree<const Key, Compare, Equal_> {
    public:
        using Basic = Detail::Red_Black_Tree<const Key, Compare, Equal_>;

        using Key_Type = Key;

        using Value_Type = Key;

        using Iterator = typename Basic::Iterator;

        using cIterator = typename Basic::cIterator;

        using rIterator = typename Basic::rIterator;

        using crIterator = typename Basic::crIterator;

        using Detail::Red_Black_Tree<const Key, Compare, Equal_>::Red_Black_Tree;

    };
}

#endif //TINYSTL_SET_HPP
