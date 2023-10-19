//
// Created by 86152 on 2023/9/23.
//

#ifndef ALGORITHMS2_HPP2
#define ALGORITHMS2_HPP2

#include "../Iterator.hpp"
#include "../Function.hpp"

namespace STD {

    //冒泡排序
    template<typename Input_iterator, typename Compare>
    void Bubble_Sort(Input_iterator begin, const Input_iterator &end, Compare fun) {
        auto iter = begin, temp = begin, target = begin;
        while (iter != end) {
            while (++temp != end) {
                if (fun(*temp, *target))
                    target = temp;
            }
            swap(*iter, *target);
            ++iter;
            target = temp = iter;
        }
    }

    //快速排序
    template<typename Bidirectional_iterator, typename Compare>
    void Quick_Sort(const Bidirectional_iterator &begin, const Bidirectional_iterator &end, Compare fun) {
        if (begin == end) return;
        Bidirectional_iterator left = begin, right = end - 1, target = end - 1;
        while (left != right) {
            while (left != right && fun(*left, *target)) ++left;
            while (right != left && !fun(*right, *target)) --right;
            swap(*left, *right);
        }
        if (fun(*target, *left)) swap(*left, *target);
        Quick_Sort(begin, left, fun);
        Quick_Sort(left + 1, end, fun);
    }

    //堆排序
    template<typename Random_iterator, typename Compare>
    void Heap_Sort(const Random_iterator &begin, const Random_iterator &end, Compare fun) {
        if (end - begin < 2) return;
        Size size = end - begin;
        auto create_max_heap = [&begin, &fun](Size start, Size last) {
            Size parent = start, child = (parent << 1) + 1;
            while (child < last) {
                if (child + 1 < last && fun(begin[child], begin[child + 1]))
                    ++child;
                if (!fun(begin[child], begin[parent])) {
                    swap(begin[parent], begin[child]);
                    parent = child;
                    child = (parent << 1) + 1;
                } else return;
            }
        };

        for (Step i = (size >> 1) - 1; i >= 0; --i) {
            create_max_heap(i, size);
        }
        for (Size i = size; i > 0; --i) {
            swap(begin[0], begin[i - 1]);
            create_max_heap(0, i - 1);
        }
    }

    namespace Detail {
        template<typename Input_iterator, typename Compare>
        void Sort_Helper(const Input_iterator &begin, const Input_iterator &end,
                         Compare fun, Random_iterator_tag) {
            if (end - begin <= 30) {
                Bubble_Sort(begin, end, fun);
            } else if (end - begin < 100000) {
                Quick_Sort(begin, end, fun);
            } else {
                Heap_Sort(begin, end, fun);
            }
        }

        template<typename Input_iterator, typename Compare>
        void Sort_Helper(const Input_iterator &begin, const Input_iterator &end,
                         Compare fun, Bidirectional_iterator_tag) {
            Quick_Sort(begin, end, fun);
        }

        template<typename Input_iterator, typename Compare>
        void Sort_Helper(const Input_iterator &begin, const Input_iterator &end,
                         Compare fun, Forward_iterator_tag) {
            Bubble_Sort(begin, end, fun);
        }
    }

    template<typename Input_iterator,
            typename Compare = Less<typename Iterator_traits<Input_iterator>::Value_type>>
    void Sort(const Input_iterator &begin, const Input_iterator &end,
              Compare fun = Less<typename Iterator_traits<Input_iterator>::Value_type>()) {
        Detail::Sort_Helper(begin, end, fun, Iterator_category(begin));
    }


} // namespace STD

#endif
