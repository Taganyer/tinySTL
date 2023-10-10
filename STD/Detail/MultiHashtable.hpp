//
// Created by 86152 on 2023/10/10.
//

#ifndef TINYSTL_MULTIHASHTABLE_HPP
#define TINYSTL_MULTIHASHTABLE_HPP

#include "../Function.hpp"

namespace STD {

    namespace Detail {

        template<typename Key, typename Hash_Code, typename Equal_>
        class MultiHashtable;

        template<typename Key, typename Hash_Code, typename Equal_>
        struct MultiHash_table_Iterator : public Iterator<Bidirectional_iterator_tag, Key> {
        public:

        };

    }

}

#endif //TINYSTL_MULTIHASHTABLE_HPP
