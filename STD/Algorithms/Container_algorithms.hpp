//
// Created by 86152 on 2023/9/26.
//

#ifndef TINYSTL_CONTAINER_ALGORITHMS_HPP
#define TINYSTL_CONTAINER_ALGORITHMS_HPP

#include <initializer_list>
#include "../Detail/Iter.hpp"
#include "../Type_traits.hpp"

namespace STD {

//----------------------------------------------------------------------------------------------------------------------

    namespace Detail {
        template<typename Input_iterator>
        typename Iterator_traits<Input_iterator>::Difference_type
        Distance_Helper(const Input_iterator &begin, const Input_iterator &end, Input_iterator_tag) {
            typename Iterator_traits<Input_iterator>::Difference_type count = 0;
            Input_iterator temp = begin;
            while (temp != end) {
                ++temp, ++count;
            }
            return count;
        }

        template<typename Random_iterator>
        typename Iterator_traits<Random_iterator>::Difference_type
        Distance_Helper(const Random_iterator &begin, const Random_iterator &end, Random_iterator_tag) {
            return end - begin;
        }
    }

    template<typename Input_iterator>
    typename Iterator_traits<Input_iterator>::Difference_type
    Distance(const Input_iterator &begin, const Input_iterator &end) {
        return Detail::Distance_Helper(begin, end, Iterator_category(begin));
    }

//----------------------------------------------------------------------------------------------------------------------

    namespace Detail {
        template<typename Output_iterator>
        Output_iterator Advance_Helper(Output_iterator target,
                                       Size size, Input_iterator_tag) {
            while (size) {
                ++target;
                --size;
            }
            return target;
        }

        template<typename Output_iterator>
        Output_iterator Advance_Helper(Output_iterator target,
                                       Size size, Random_iterator_tag) {
            return target += size;
        }

        template<typename Output_iterator>
        Output_iterator Backwards_Helper(Output_iterator target,
                                         Size size, Bidirectional_iterator_tag) {
            while (size) {
                --target;
                --size;
            }
            return target;
        }

        template<typename Output_iterator>
        Output_iterator Backwards_Helper(Output_iterator target,
                                         Size size, Random_iterator_tag) {
            return target -= size;
        }
    }

    template<typename Output_iterator>
    Output_iterator Backwards(Output_iterator target, Size size) {
        return Detail::Backwards_Helper(target, size, Iterator_category(target));
    }

    template<typename Output_iterator>
    Output_iterator Advance(Output_iterator target, Size size) {
        return Detail::Advance_Helper(target, size, Iterator_category(target));
    }

//----------------------------------------------------------------------------------------------------------------------

    namespace Detail {
        template<typename Input_iterator>
        Size Get_Size_Helper(Size size, const Input_iterator &target, True_type) {
            return size;
        }

        template<typename Input_iterator>
        Size Get_Size_Helper(Input_iterator begin, const Input_iterator &end, False_type) {
            return Distance(begin, end);
        }

        template<typename Output_iterator, typename Input_iterator>
        void Fill_With_Helper(Output_iterator pos, const Input_iterator &size,
                              const Input_iterator &target, True_type) {
            for (int i = 0; i < size; ++i) {
                *pos = target;
                ++pos;
            }
        }

        template<typename Output_iterator, typename Input_iterator>
        void Fill_With_Helper(Output_iterator pos, Input_iterator begin,
                              const Input_iterator &end, False_type) {
            while (begin != end) {
                *pos = *begin;
                ++pos, ++begin;
            }
        }

        template<typename Output_iterator, typename Input_iterator>
        void rFill_With_Helper(Output_iterator pos, const Input_iterator &size,
                               const Input_iterator &target, True_type) {
            for (int i = 0; i < size; ++i) {
                *pos = target;
                --pos;
            }
        }

        template<typename Output_iterator, typename Input_iterator>
        void rFill_With_Helper(Output_iterator pos, Input_iterator begin,
                               const Input_iterator &end, False_type) {
            while (begin != end) {
                *pos = *begin;
                --pos, ++begin;
            }
        }

        template<typename Input_iterator>
        Size Get_Size(Input_iterator begin, const Input_iterator &end) {
            return Get_Size_Helper(begin, end, Is_integral<Input_iterator>());
        }
    }

    template<typename Output_iterator, typename Arg>
    void Fill_With(Output_iterator pos, Size size, Arg target) {
        for (int i = 0; i < size; ++i) {
            *pos = target;
            ++pos;
        }
    }

    template<typename Output_iterator, typename Arg>
    void Fill_With(Output_iterator pos, const std::initializer_list<Arg> &list) {
        for (auto &t: list) {
            *pos = t;
            ++pos;
        }
    }

    template<typename Output_iterator, typename Input_iterator>
    void Fill_With(Output_iterator pos, const Input_iterator &begin,
                   const Input_iterator &end) {
        Detail::Fill_With_Helper(pos, begin, end, Is_integral<Input_iterator>());
    }

    template<typename Output_iterator, typename Arg>
    void rFill_With(Output_iterator pos, Size size, Arg target) {
        for (int i = 0; i < size; ++i) {
            *pos = target;
            --pos;
        }
    }

    template<typename Output_iterator, typename Arg>
    void rFill_With(Output_iterator pos, const std::initializer_list<Arg> &list) {
        for (auto &t: list) {
            *pos = t;
            --pos;
        }
    }

    template<typename Output_iterator, typename Input_iterator>
    void rFill_With(Output_iterator pos, Input_iterator begin,
                    const Input_iterator &end) {
        Detail::rFill_With_Helper(pos, begin, end, Is_integral<Input_iterator>());
    }

//----------------------------------------------------------------------------------------------------------------------

    void MemSet(void *target, int val, Size size);

}

#endif //TINYSTL_CONTAINER_ALGORITHMS_HPP
