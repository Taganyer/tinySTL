//
// Created by 86152 on 2023/10/5.
//

#ifndef TINYSTL_SET_HPP
#define TINYSTL_SET_HPP

#include "Detail/Red_Black_Tree.hpp"

namespace STD {

    template<typename Key, typename Compare = Less<Key>, typename Equal_ = Equal<Key>>
    class Set : public Red_Black_Tree<const Key, Compare, Equal_> {
    public:
        using Basic = Red_Black_Tree<const Key, Compare, Equal_>;

        using Iterator = typename Basic::Iterator;

        using cIterator = typename Basic::cIterator;

        using rIterator = typename Basic::rIterator;

        using crIterator = typename Basic::crIterator;

        using Self = Set<Key, Compare, Equal_>;

        explicit Set(Compare compare = Compare(), Equal_ equal = Equal_()) :
                Basic(compare, equal) {};

        template<typename Key_Input, typename Val_Input>
        Set(Key_Input key_begin, const Key_Input &key_end, Val_Input val_begin,
            Compare compare = Compare(), Equal_ equal = Equal_()) :
                Basic(key_begin, key_end, val_begin, compare, equal) {};

        ~Set() = default;

    };
}

#endif //TINYSTL_SET_HPP
