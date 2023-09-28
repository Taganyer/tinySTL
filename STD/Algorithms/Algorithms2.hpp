//
// Created by 86152 on 2023/9/23.
//

#ifndef ALGORITHMS2_HPP2
#define ALGORITHMS2_HPP2

#include "../Iter.hpp"
#include "../Function.hpp"

namespace STD {

    template<typename Arg, typename Compare = Less<Arg>>
    void sort(const Iter<Arg> &begin, const Iter<Arg> &end, Compare fun = Less<Arg>()) {
        auto iter = begin.deep_copy(), temp = begin.deep_copy(), target = begin.deep_copy();
        while (*iter != end) {
            while (++(*temp) != end) {
                if (fun(**temp, **target))
                    *target = *temp;
            }
            swap(**iter, **target);
            ++(*iter);
            temp = iter->deep_copy();
            target = iter->deep_copy();
        }
    };

    template<typename Arg>
    void sort(const Bidirectional_Iter<Arg> &begin, const Bidirectional_Iter<Arg> &end);

    template<typename Arg>
    void sort(const Random_Iter<Arg> &begin, const Random_Iter<Arg> &end);


} // namespace STD

#endif
