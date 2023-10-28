//
// Created by 86152 on 2023/10/23.
//

#ifndef TINYSTL_ALGORITHMS4_HPP
#define TINYSTL_ALGORITHMS4_HPP

#include <initializer_list>

namespace STD {

    template<typename Arg>
    const Arg &Min(const Arg &left, const Arg &right) {
        if (left < right) return left;
        else return right;
    }

    template<typename Fun, typename Arg>
    const Arg &Min(const Arg &left, const Arg &right, Fun compare) {
        if (compare(left, right)) return left;
        else return right;
    }

    template<typename Arg>
    Arg Min(const std::initializer_list<Arg> &list) {
        auto target = list.begin(), begin = list.begin(), end = list.end();
        while (begin != end) {
            if (*begin < *target)
                target = begin;
            ++begin;
        }
        return *begin;
    }

    template<typename Fun, typename Arg>
    Arg Min(const std::initializer_list<Arg> &list, Fun compare) {
        auto target = list.begin(), begin = list.begin(), end = list.end();
        while (begin != end) {
            if (compare(*begin, *target))
                target = begin;
            ++begin;
        }
        return *begin;
    }

    template<typename Arg>
    const Arg &Max(const Arg &left, const Arg &right) {
        if (left < right) return right;
        else return left;
    }

    template<typename Fun, typename Arg>
    const Arg &Max(const Arg &left, const Arg &right, Fun compare) {
        if (compare(left, right)) return right;
        else return left;
    }

    template<typename Arg>
    Arg Max(const std::initializer_list<Arg> &list) {
        auto target = list.begin(), begin = list.begin(), end = list.end();
        while (begin != end) {
            if (*target < *begin)
                target = begin;
            ++begin;
        }
        return *begin;
    }

    template<typename Fun, typename Arg>
    Arg Max(const std::initializer_list<Arg> &list, Fun compare) {
        auto target = list.begin(), begin = list.begin(), end = list.end();
        while (begin != end) {
            if (compare(*target, *begin))
                target = begin;
            ++begin;
        }
        return *begin;
    }

    template<typename Arg>
    Pair<Arg, Arg> MinMax(const Arg &left, const Arg &right) {
        if (left < right) return {left, right};
        else return {right, left};
    }

    template<typename Fun, typename Arg>
    Pair<Arg, Arg> MinMax(const Arg &left, const Arg &right, Fun compare) {
        if (compare(left, right)) return {left, right};
        else return {right, left};
    }

    template<typename Arg>
    Pair<Arg, Arg> MinMax(const std::initializer_list<Arg> &list) {
        auto max = list.begin(), min = max, begin = list.begin(), end = list.end();
        while (begin != end) {
            if (*max < *begin) max = begin;
            if (*begin < *min) min = begin;
            ++begin;
        }
        return {*min, *max};
    }

    template<typename Fun, typename Arg>
    Pair<Arg, Arg> MinMax(const std::initializer_list<Arg> &list, Fun compare) {
        auto max = list.begin(), min = max, begin = list.begin(), end = list.end();
        while (begin != end) {
            if (compare(*max, *begin)) max = begin;
            if (compare(*begin, *min)) min = begin;
            ++begin;
        }
        return {*min, *max};
    }

    template<typename Input_iterator>
    Input_iterator Min_Element(Input_iterator begin, const Input_iterator &end) {
        Input_iterator target = begin;
        while (begin != end) {
            if (*begin < *target)
                target = begin;
            ++begin;
        }
        return target;
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Min_Element(Input_iterator begin, const Input_iterator &end, Fun compare) {
        Input_iterator target = begin;
        while (begin != end) {
            if (compare(*begin, *target))
                target = begin;
            ++begin;
        }
        return target;
    }

    template<typename Input_iterator>
    Input_iterator Max_Element(Input_iterator begin, const Input_iterator &end) {
        Input_iterator target = begin;
        while (begin != end) {
            if (*target < *begin)
                target = begin;
            ++begin;
        }
        return target;
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Max_Element(Input_iterator begin, const Input_iterator &end, Fun compare) {
        Input_iterator target = begin;
        while (begin != end) {
            if (compare(*target, *begin))
                target = begin;
            ++begin;
        }
        return target;
    }

    template<typename Input_iterator>
    Pair<Input_iterator, Input_iterator>
    MinMax_Element(Input_iterator begin, const Input_iterator &end) {
        Input_iterator min = begin, max = begin;
        while (begin != end) {
            if (*begin < *min) min = begin;
            if (*max < *begin) max = begin;
            ++begin;
        }
        return {min, max};
    }

    template<typename Fun, typename Input_iterator>
    Pair<Input_iterator, Input_iterator>
    MinMax_Element(Input_iterator begin, const Input_iterator &end, Fun compare) {
        Input_iterator min = begin, max = begin;
        while (begin != end) {
            if (compare(*begin, *min)) min = begin;
            if (compare(*max, *begin)) max = begin;
            ++begin;
        }
        return {min, max};
    }

    template<typename Type, typename Input_iterator>
    Input_iterator Find_Next(Input_iterator begin, const Input_iterator &end, const Type &value) {
        Input_iterator target = begin;
        while (begin != end) {
            if (value < *begin) {
                target = begin;
                break;
            }
            ++begin;
        }
        while (begin != end) {
            if (value < *begin && *begin < *target)
                target = begin;
            ++begin;
        }
        return target;
    }

    template<typename Fun, typename Type, typename Input_iterator>
    Input_iterator Find_Next(Input_iterator begin, const Input_iterator &end,
                             const Type &value, Fun compare) {
        Input_iterator target = begin;
        while (begin != end) {
            if (compare(value, *begin)) {
                target = begin;
                break;
            }
            ++begin;
        }
        while (begin != end) {
            if (compare(value, *begin) && compare(*begin, *target))
                target = begin;
            ++begin;
        }
        return target;
    }

    template<typename Type, typename Input_iterator>
    Input_iterator Find_Last(Input_iterator begin, const Input_iterator &end, const Type &value) {
        Input_iterator target = begin;
        while (begin != end) {
            if (*begin < value) {
                target = begin;
                break;
            }
            ++begin;
        }
        while (begin != end) {
            if (*begin < value && *target < *begin)
                target = begin;
            ++begin;
        }
        return target;
    }

    template<typename Fun, typename Type, typename Input_iterator>
    Input_iterator Find_Last(Input_iterator begin, const Input_iterator &end,
                             const Type &value, Fun compare) {
        Input_iterator target = begin;
        while (begin != end) {
            if (compare(*begin, value)) {
                target = begin;
                break;
            }
            ++begin;
        }
        while (begin != end) {
            if (compare(*begin, value) && compare(*target, *begin))
                target = begin;
            ++begin;
        }
        return target;
    }

    template<typename Input_iterator1, typename Input_iterator2>
    bool Lexicographical_Compare(Input_iterator1 begin1, const Input_iterator2 &end1,
                                 Input_iterator2 begin2, const Input_iterator2 &end2) {
        while (begin1 != end1 && begin2 != end2) {
            if (*begin1 != *begin2) return *begin1 < *begin2;
            ++begin1, ++begin2;
        }
        return begin1 == end1 && begin2 != end2;
    }

    template<typename Fun, typename Input_iterator1, typename Input_iterator2>
    bool Lexicographical_Compare(Input_iterator1 begin1, const Input_iterator2 &end1,
                                 Input_iterator2 begin2, const Input_iterator2 &end2, Fun compare) {
        while (begin1 != end1 && begin2 != end2) {
            if (*begin1 != *begin2) return compare(*begin1, *begin2);
            ++begin1, ++begin2;
        }
        return begin1 == end1 && begin2 != end2;
    }

    template<typename Type, typename Input_iterator1, typename Input_iterator2>
    Type Inner_Product(Input_iterator1 begin1, const Input_iterator1 &end1,
                       Input_iterator2 begin2, Type init) {
        while (begin1 != end1) {
            init = init + *begin1 * *begin2;
            ++begin1, ++begin2;
        }
        return init;
    }

    template<typename Fun1, typename Fun2, typename Type, typename Input_iterator1, typename Input_iterator2>
    Type Inner_Product(Input_iterator1 begin1, const Input_iterator1 &end1,
                    Input_iterator2 begin2, Type init, Fun1 add, Fun2 product) {
        while (begin1 != end1) {
            init = add(init, product(*begin1, *begin2));
            ++begin1, ++begin2;
        }
        return init;
    }

    template<typename Input_iterator, typename Output_iterator>
    void Partial_Sum(Input_iterator begin, const Input_iterator &end, Output_iterator dest) {
        if (begin == end) return;
        *dest = *begin;
        Output_iterator last = dest;
        ++dest, ++begin;
        while (begin != end) {
            *dest = *last + *begin;
            ++dest, ++last, ++begin;
        }
    }

    template<typename Fun, typename Input_iterator, typename Output_iterator>
    void Partial_Sum(Input_iterator begin, const Input_iterator &end, Output_iterator dest, Fun fun) {
        if (begin == end) return;
        *dest = *begin;
        Output_iterator last = dest;
        ++dest, ++begin;
        while (begin != end) {
            *dest = fun(*last, *begin);
            ++dest, ++last, ++begin;
        }
    }

    template<typename Input_iterator, typename Output_iterator>
    void Adjacent_Difference(Input_iterator begin, const Input_iterator &end, Output_iterator dest) {
        if (begin == end) return;
        *dest = *begin;
        Input_iterator last = begin;
        ++dest, ++begin;
        while (begin != end) {
            *dest = *begin - *last;
            ++dest, ++last, ++begin;
        }
    }

    template<typename Fun, typename Input_iterator, typename Output_iterator>
    void Adjacent_Difference(Input_iterator begin, const Input_iterator &end, Output_iterator dest, Fun fun) {
        if (begin == end) return;
        *dest = *begin;
        Input_iterator last = begin;
        ++dest, ++begin;
        while (begin != end) {
            *dest = fun(*begin, *last);
            ++dest, ++last, ++begin;
        }
    }

}

#endif //TINYSTL_ALGORITHMS4_HPP
