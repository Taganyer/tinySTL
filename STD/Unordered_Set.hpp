//
// Created by 86152 on 2023/10/7.
//

#ifndef TINYSTL_UNORDERED_SET_HPP
#define TINYSTL_UNORDERED_SET_HPP

#include "Detail/Hashtable.hpp"

namespace STD {

    template<typename Key, typename Hash_Code = Hash<Key>, typename Equal_ = Equal<Key>>
    class Unordered_Set : public Detail::Hashtable<const Key, Hash_Code, Equal_> {
    public:
        using Basic = Detail::Hashtable<const Key, Hash_Code, Equal_>;

        using Key_Type = Key;

        using Value_Type = Key;

        using Iterator = typename Basic::Iterator;

        using cIterator = typename Basic::cIterator;

        using rIterator = typename Basic::rIterator;

        using crIterator = typename Basic::crIterator;

        using Detail::Hashtable<const Key, Hash_Code, Equal_>::Hashtable;

    };

}

#endif //TINYSTL_UNORDERED_SET_HPP
