//
// Created by 86152 on 2023/9/23.
//

#ifndef ALGORITHMS2_HPP2
#define ALGORITHMS2_HPP2

#include "../Iterator.hpp"
#include "../Function.hpp"
#include "../Vector.hpp"

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

        for (Signed_Size i = (size >> 1) - 1; i >= 0; --i) {
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

    template<typename Fun, typename Input_iterator>
    bool Is_Partitioned(Input_iterator begin, Input_iterator end, Fun fun) {
        bool less = false, great = false;
        while (begin != end) {
            bool t = fun(*begin);
            if (less && great && t || !less && great && t) return false;
            if (t) less = true;
            else great = true;
            ++begin;
        }
    }

    template<typename Fun, typename Input_iterator, typename Output_iterator1, typename Output_iterator2>
    void Partition_Copy(Input_iterator begin, const Input_iterator &end,
                        Output_iterator1 dest1, Output_iterator2 dest2, Fun fun) {
        while (begin != end) {
            if (fun(*begin)) {
                *dest1 = *begin;
                ++dest1;
            } else {
                *dest2 = *begin;
                ++dest2;
            }
            ++begin;
        }
    }

    template<typename Fun, typename Input_iterator, typename Output_iterator1, typename Output_iterator2>
    void Partition_Move(Input_iterator begin, const Input_iterator &end,
                        Output_iterator1 dest1, Output_iterator2 dest2, Fun fun) {
        while (begin != end) {
            if (fun(*begin)) {
                *dest1 = move(*begin);
                ++dest1;
            } else {
                *dest2 = move(*begin);
                ++dest2;
            }
            ++begin;
        }
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Partition_Point(Input_iterator begin, const Input_iterator &end, Fun fun) {
        while (begin != end) {
            if (!fun(*begin)) break;
            ++begin;
        }
        return begin;
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Stable_Partition(Input_iterator begin, const Input_iterator &end, Fun fun) {
        Vector<typename Input_iterator::Value_Type> store_t, store_f;
        while (begin != end && fun(*begin)) ++begin;
        Input_iterator start = begin, temp = start;
        while (begin != end) {
            if (fun(*begin)) store_t.push_back(move(*begin));
            else store_f.push_back(move(*begin));
            ++begin;
        }
        for (auto &t: store_t) {
            *start = move(t);
            ++start;
        }
        for (auto &t: store_f) {
            *start = move(t);
            ++start;
        }
        return temp;
    }

    namespace Detail {
        template<typename Fun, typename Input_iterator>
        Input_iterator Partition_Helper(Input_iterator begin, const Input_iterator &end,
                                        Fun fun, Input_iterator_tag) {
            return Stable_Partition(begin, end, fun);
        }

        template<typename Fun, typename Input_iterator>
        Input_iterator Partition_Helper(Input_iterator begin, const Input_iterator &end,
                                        Fun fun, Bidirectional_iterator_tag) {
            while (begin != end && fun(*begin)) ++begin;
            if (begin == end) return begin;
            Input_iterator store = begin;
            while (!fun(*--end));
            while (begin != end) {
                swap(*begin, *end);
                while (!fun(*--end));
                while (begin != end && fun(*begin)) ++begin;
            }
            return store;
        }
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Partition(Input_iterator begin, const Input_iterator &end, Fun fun) {
        return Detail::Partition_Helper(begin, end, fun, Iterator_category(begin));
    }


} // namespace STD

#endif
