//
// Created by 86152 on 2023/9/26.
//

#ifndef TINYSTL_CONTAINER_ALGORITHMS_HPP
#define TINYSTL_CONTAINER_ALGORITHMS_HPP

#include <initializer_list>
#include "../../Detail/Iter.hpp"
#include "../../Type_traits.hpp"

namespace STD {

//----------------------------------------------------------------------------------------------------------------------
    template<typename Input_iterator>
    inline typename Iterator_traits<Input_iterator>::Difference_type
    _distance(const Input_iterator &begin, const Input_iterator &end, Input_iterator_tag) {
        typename Iterator_traits<Input_iterator>::Difference_type count = 0;
        Input_iterator temp = begin;
        while (temp != end) {
            ++temp, ++count;
        }
        return count;
    }

    template<typename Random_iterator>
    inline typename Iterator_traits<Random_iterator>::Difference_type
    _distance(const Random_iterator &begin, const Random_iterator &end, Random_iterator_tag) {
        return end - begin;
    }

    template<typename Input_iterator>
    inline typename Iterator_traits<Input_iterator>::Difference_type
    calculate_Length(const Input_iterator &begin, const Input_iterator &end) {
        return _distance(begin, end, Iterator_category(begin));
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Output_iterator>
    Output_iterator _advance(Output_iterator target,
                             Size size, Input_iterator_tag) {
        while (size) {
            ++target;
            --size;
        }
        return target;
    }

    template<typename Output_iterator>
    Output_iterator _advance(Output_iterator target,
                             Size size, Random_iterator_tag) {
        return target += size;
    }

    template<typename Output_iterator>
    Output_iterator advance(Output_iterator target, Size size) {
        return _advance(target, size, Iterator_category(target));
    }

    template<typename Output_iterator>
    Output_iterator _backwards(Output_iterator target,
                               Size size, Bidirectional_iterator_tag) {
        while (size) {
            --target;
            --size;
        }
        return target;
    }

    template<typename Output_iterator>
    Output_iterator _backwards(Output_iterator target,
                             Size size, Random_iterator_tag) {
        return target -= size;
    }

    template<typename Output_iterator>
    Output_iterator backwards(Output_iterator target, Size size) {
        return _backwards(target, size, Iterator_category(target));
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Output_iterator, typename Arg>
    void fill_with(Output_iterator pos, Arg target, Size size) {
        for (int i = 0; i < size; ++i) {
            *pos = target;
            ++pos;
        }
    }

    template<typename Output_iterator, typename Arg>
    void fill_with(Output_iterator pos, const std::initializer_list<Arg> &list) {
        for (auto &t: list) {
            *pos = t;
            ++pos;
        }
    }

    template<typename Output_iterator, typename Input_iterator>
    void fill_with_helper(Output_iterator pos, const Input_iterator &target,
                          const Input_iterator &size, True_type) {
        for (int i = 0; i < size; ++i) {
            *pos = target;
            ++pos;
        }
    }

    template<typename Output_iterator, typename Input_iterator>
    void fill_with_helper(Output_iterator pos, Input_iterator begin,
                          const Input_iterator &end, False_type) {
        while (begin != end) {
            *pos = *begin;
            ++pos, ++begin;
        }
    }

    template<typename Output_iterator, typename Input_iterator>
    void fill_with(Output_iterator pos, const Input_iterator &begin,
                   const Input_iterator &end) {
        fill_with_helper(pos, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Output_iterator, typename Arg>
    void rfill_with(Output_iterator pos, const std::initializer_list<Arg> &list) {
        for (auto &t: list) {
            *pos = t;
            --pos;
        }
    }

    template<typename Output_iterator, typename Input_iterator>
    void rfill_with(Output_iterator pos, Input_iterator begin,
                    const Input_iterator &end) {
        while (begin != end) {
            *pos = *begin;
            --pos, ++begin;
        }
    }

//----------------------------------------------------------------------------------------------------------------------

}

#endif //TINYSTL_CONTAINER_ALGORITHMS_HPP
