//
// Created by 86152 on 2023/9/30.
//

#ifndef TINYSTL_HASHCODE_HPP
#define TINYSTL_HASHCODE_HPP

#include "../Type_traits.hpp"
#include "../Config.hpp"
#include "../String.hpp"

namespace STD {

    template<typename Arg, typename Range>
    struct Hash_Helper {
        Size operator()(const Arg &target) const {
            return target;
        }
    };

    template<typename Arg>
    struct Hash_Helper<Arg, True_type> {
        Size operator()(const Arg &target) const {
            return target;
        }
    };

    template<typename Arg>
    struct Hash : Hash_Helper<Arg, Is_integral<Arg>> {
        using Hash_Helper<Arg, Is_integral<Arg>>::operator();
    };

    template<>
    struct Hash<String> {
        Size operator()(const String &target) const {
            int h = 0;
            Size len = target.size();
            if (len > 0) {
                for (int i = 0; i < len; ++i) {
                    h = 31 * h + target[i];
                }
            }
            return h;
        };
    };

}

#endif //TINYSTL_HASHCODE_HPP
