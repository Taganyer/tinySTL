//
// Created by 86152 on 2023/9/23.
//

#ifndef ALGORITHMS2_HPP2
#define ALGORITHMS2_HPP2

#include "../Iterator.hpp"
#include "../Function.hpp"
#include "Container_algorithms.hpp"

namespace STD {

    template<typename Input_iterator>
    bool Is_Sorted(Input_iterator begin, const Input_iterator &end) {
        if (begin == end) return true;
        Input_iterator last = begin;
        if (begin != end && *last == *begin) last = begin++;
        if (begin == end) return true;
        if (*last < *++begin) {
            last = begin++;
            while (begin != end) {
                if (*begin < *last) return false;
                last = begin++;
            }
        } else {
            last = begin++;
            while (begin != end) {
                if (*begin > *last) return false;
                last = begin++;
            }
        }
        return true;
    }

    template<typename Fun, typename Input_iterator>
    bool Is_Sorted(Input_iterator begin, const Input_iterator &end, Fun fun) {
        if (begin == end) return true;
        Input_iterator last = begin++;
        while (begin != end) {
            if (fun(*begin, *last)) return false;
            last = begin++;
        }
        return true;
    }

    template<typename Input_iterator>
    Input_iterator Is_Sorted_Until(Input_iterator begin, const Input_iterator &end) {
        if (begin == end) return end;
        Input_iterator last = begin;
        if (begin != end && *last == *begin) last = begin++;
        if (begin == end) return end;
        if (*last < *++begin) {
            last = begin++;
            while (begin != end) {
                if (*begin < *last) return begin;
                last = begin++;
            }
        } else {
            last = begin++;
            while (begin != end) {
                if (*begin > *last) return begin;
                last = begin++;
            }
        }
        return end;
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Is_Sorted_Until(Input_iterator begin, const Input_iterator &end, Fun fun) {
        if (begin == end) return end;
        Input_iterator last = begin++;
        while (begin != end) {
            if (fun(*begin, *last)) return begin;
            last = begin++;
        }
        return end;
    }

    namespace Detail {
        template<typename Bidirectional_iterator, typename Compare>
        void Selection_Sort(const Bidirectional_iterator &begin, const Bidirectional_iterator &end, Compare fun) {
            if (begin == end) return;
            Bidirectional_iterator target = begin, last = target++;
            while (target != end) {
                if (fun(*target, *last)) {
                    typename Iterator_traits<Bidirectional_iterator>::Value_type val = move(*target);
                    Bidirectional_iterator temp = target;
                    *temp = move(*last);
                    --last, --temp;
                    while (temp != begin && fun(val, *last)) {
                        *temp = move(*last);
                        --temp, --last;
                    }
                    *temp = move(val);
                }
                last = target;
                ++target;
            }
        }

        //冒泡排序
        template<typename Input_iterator, typename Compare>
        void Bubble_Sort(Input_iterator begin, const Input_iterator &end, Compare fun) {
            Input_iterator iter = begin, temp = begin, target = begin;
            while (iter != end) {
                while (++temp != end) {
                    if (fun(*temp, *target))
                        target = temp;
                }
                swap(*iter, *target);
                target = temp = ++iter;
            }
        }

        //快速排序
        template<typename Bidirectional_iterator, typename Compare>
        void Quick_Sort(const Bidirectional_iterator &begin, const Bidirectional_iterator &end, Compare fun) {
            if (Is_Sorted(begin, end, fun)) return;
            Bidirectional_iterator left = begin, right = end, target = --right;
            while (left != right) {
                while (left != right && fun(*left, *target)) ++left;
                while (right != left && !fun(*right, *target)) --right;
                swap(*left, *right);
            }
            if (fun(*target, *left)) swap(*left, *target);
            Quick_Sort(begin, left, fun);
            Quick_Sort(++left, end, fun);
        }

        template<typename Random_Iterator, typename Compare>
        void Quick_Sort(const Random_Iterator &begin, const Random_Iterator &end,
                        Compare fun, Random_iterator_tag) {
            if (Is_Sorted(begin, end, fun)) return;
            Random_Iterator left = begin, right = end, target = --right;
            while (left != right) {
                while (left != right && fun(*left, *target)) ++left;
                while (right != left && !fun(*right, *target)) --right;
                swap(*left, *right);
            }
            if (fun(*target, *left)) swap(*left, *target);
            if (left - begin > 20) Quick_Sort(begin, left, fun);
            else Selection_Sort(begin, left, fun);
            if (end - ++left > 20) Quick_Sort(left, end, fun);
            else Selection_Sort(left, end, fun);
        }

        template<typename Arg, typename Input_iterator, typename Compare>
        void Stable_Quick_Sort_Helper(Vector<Arg> &arr, Input_iterator begin,
                                      const Input_iterator &end, Compare fun) {
            Vector<Arg> less, great, equal;
            equal.push_back(move(arr[0]));
            for (Size i = 1; i < arr.size(); ++i) {
                if (fun(arr[i], equal[0])) {
                    if (fun(equal[0], arr[i])) equal.push_back(move(arr[i]));
                    else less.push_back(move(arr[i]));
                } else if (!fun(equal[0], arr[i])) equal.push_back(move(arr[i]));
                else great.push_back(move(arr[i]));
            }
            if (less.size() < 2) {
                for (auto &t: less) {
                    *begin = move(t);
                    ++begin;
                }
            } else {
                Input_iterator temp = Advance(begin, less.size());
                Stable_Quick_Sort_Helper(less, begin, temp, fun);
                begin = temp;
            }
            for (auto &t: equal) {
                *begin = move(t);
                ++begin;
            }
            if (great.size() < 2) {
                for (auto &t: great) {
                    *begin = move(t);
                    ++begin;
                }
            } else {
                Stable_Quick_Sort_Helper(great, begin, end, fun);
            }
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
            for (Signed_Size i = size / 2 - 1; i >= 0; --i)
                create_max_heap(i, size);
            for (Size i = size; i > 0; --i) {
                swap(begin[0], begin[i - 1]);
                create_max_heap(0, i - 1);
            }
        }

        template<typename Input_iterator, typename Compare>
        void Sort_Helper(const Input_iterator &begin, const Input_iterator &end,
                         Compare fun, Random_iterator_tag) {
            if (end - begin <= 50) {
                Selection_Sort(begin, end, fun);
            } else {
                Quick_Sort(begin, end, fun, Random_iterator_tag());
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

    template<typename Input_iterator, typename Compare>
    void Stable_Sort(Input_iterator begin, const Input_iterator &end, Compare fun) {
        if (begin == end) return;
        Vector<typename Iterator_traits<Input_iterator>::Value_type> arr;
        Input_iterator temp = begin;
        while (begin != end) {
            arr.push_back(move(*begin));
            ++begin;
        }
        Detail::Stable_Quick_Sort_Helper(arr, temp, end, fun);
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
        Vector<typename Iterator_traits<Input_iterator>::Value_Type> store_t, store_f;
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

        template<typename Fun, typename Input_iterator>
        void Partition_Sort_Helper(Input_iterator begin, Input_iterator mid,
                                   const Input_iterator &end, Fun fun, Input_iterator_tag) {
            Input_iterator iter = begin, temp = begin, target = begin;
            while (iter != mid) {
                while (++temp != end) {
                    if (fun(*temp, *target))
                        target = temp;
                }
                swap(*iter, *target);
                target = temp = ++iter;
            }
        }

        template<typename Fun, typename Bidirectional_iterator>
        void Partition_Sort_Helper(Bidirectional_iterator begin, Bidirectional_iterator mid,
                                   const Bidirectional_iterator &end, Fun fun, Bidirectional_iterator_tag) {
            if (mid == end) Quick_Sort(begin, end, fun);
            if (Is_Sorted(begin, end, fun) || begin == mid) return;
            Bidirectional_iterator left = begin, right = end, target = --right;
            bool l_flag = false;
            while (left != right) {
                while (left != right && fun(*left, *target)) {
                    if (left == mid) l_flag = true;
                    ++left;
                }
                while (right != left && !fun(*right, *target)) --right;
                swap(*left, *right);
            }
            if (fun(*target, *left)) swap(*left, *target);
            if (l_flag) Partition_Quick_Sort(begin, mid, left, fun);
            else {
                Quick_Sort(begin, left, fun);
                Partition_Quick_Sort(++left, mid, end, fun);
            }
        }

        template<typename Fun, typename Random_iterator>
        void Partition_Sort_Helper(Random_iterator begin, Random_iterator mid,
                                   const Random_iterator &end, Fun fun, Random_iterator_tag) {
            if (mid == end) Quick_Sort(begin, end, fun, Random_iterator_tag());
            if (Is_Sorted(begin, end, fun) || begin == mid) return;
            Random_iterator left = begin, right = end, target = --right;
            while (left != right) {
                while (left != right && fun(*left, *target)) ++left;
                while (right != left && !fun(*right, *target)) --right;
                swap(*left, *right);
            }
            if (fun(*target, *left)) swap(*left, *target);
            if (mid - left >= 0) Partition_Quick_Sort(begin, mid, left, fun);
            else {
                Quick_Sort(begin, left, fun);
                Partition_Quick_Sort(++left, mid, end, fun);
            }
        }
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Partition(Input_iterator begin, const Input_iterator &end, Fun fun) {
        return Detail::Partition_Helper(begin, end, fun, Iterator_category(begin));
    }

    template<typename Input_iterator>
    void Partition_Sort(Input_iterator begin, Input_iterator mid, const Input_iterator &end) {
        Detail::Partition_Sort_Helper(begin, mid, end,
                                      Less<typename Iterator_traits<Input_iterator>::Value_type>(),
                                      Iterator_category(begin));
    }

    template<typename Fun, typename Input_iterator>
    void Partition_Sort(Input_iterator begin, Input_iterator mid, const Input_iterator &end, Fun fun) {
        Detail::Partition_Sort_Helper(begin, mid, end, fun, Iterator_category(begin));
    }

    namespace Detail {
        template<typename Type, typename Input_iterator>
        Input_iterator Remove_Helper(Input_iterator begin, const Input_iterator &end,
                                     const Type &value, Input_iterator_tag) {
            Input_iterator temp = end;
            while (begin != end) {
                while (begin != end && *begin != value) ++begin;
                if (begin == end) break;
                if (temp == end) temp = begin;
                while (temp != end && *temp == value) ++temp;
                if (temp != end) {
                    swap(*begin, *temp);
                    if (++temp == end) return ++begin;
                } else return begin;
                ++begin;
            }
            return end;
        }

        template<typename Type, typename Bidirectional_iterator>
        Bidirectional_iterator Remove_Helper(Bidirectional_iterator begin, Bidirectional_iterator end,
                                             const Type &value, Bidirectional_iterator_tag) {
            if (begin == end) return end;
            while (begin != end) {
                while (begin != end && *begin != value) ++begin;
                while (begin != end && *--end == value);
                if (begin != end) swap(*begin, *end);
            }
            return begin;
        }

        template<typename Fun, typename Input_iterator>
        Input_iterator Remove_If_Helper(Input_iterator begin, const Input_iterator &end,
                                        const Fun &fun, Input_iterator_tag) {
            Input_iterator temp = end;
            while (begin != end) {
                while (begin != end && !fun(*begin)) ++begin;
                if (begin == end) break;
                if (temp == end) temp = begin;
                while (temp != end && fun(*temp)) ++temp;
                if (temp != end) {
                    swap(*begin, *temp);
                    if (++temp == end) return ++begin;
                } else return begin;
                ++begin;
            }
            return end;
        }

        template<typename Fun, typename Bidirectional_iterator>
        Bidirectional_iterator Remove_If_Helper(Bidirectional_iterator begin, Bidirectional_iterator end,
                                                const Fun &fun, Bidirectional_iterator_tag) {
            if (begin == end) return end;
            while (begin != end) {
                while (begin != end && !fun(*begin)) ++begin;
                while (begin != end && fun(*--end));
                if (begin != end) swap(*begin, *end);
            }
            return begin;
        }
    }

    template<typename Type, typename Input_iterator>
    Input_iterator Remove(Input_iterator begin, const Input_iterator &end, const Type &value) {
        return Detail::Remove_Helper(begin, end, value, Iterator_category(begin));
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Remove_If(Input_iterator begin, const Input_iterator &end, const Fun &fun) {
        return Detail::Remove_If_Helper(begin, end, fun, Iterator_category(begin));
    }

    template<typename Type, typename Input_iterator, typename Output_iterator>
    void Remove_Copy(Input_iterator begin, const Input_iterator &end,
                     Output_iterator dest, const Type &value) {
        while (begin != end) {
            if (*begin != value) {
                *dest = *begin;
                ++dest;
            }
            ++begin;
        }
    }

    template<typename Fun, typename Input_iterator, typename Output_iterator>
    void Remove_If_Copy(Input_iterator begin, const Input_iterator &end,
                        Output_iterator dest, const Fun &fun) {
        while (begin != end) {
            if (!fun(*begin)) {
                *dest = *begin;
                ++dest;
            }
            ++begin;
        }
    }

    template<typename Input_iterator>
    bool Is_Unique(Input_iterator begin, const Input_iterator &end) {
        if (begin == end) return true;
        Input_iterator last = begin;
        while (++begin != end) {
            if (*begin == *last)
                return false;
            ++last;
        }
        return true;
    }

    template<typename Fun, typename Input_iterator>
    bool Is_Unique(Input_iterator begin, const Input_iterator &end, Fun equal) {
        if (begin == end) return true;
        Input_iterator last = begin;
        while (++begin != end) {
            if (equal(*begin, *last))
                return false;
            ++last;
        }
        return true;
    }

    template<typename Input_iterator>
    Input_iterator Unique(Input_iterator begin, const Input_iterator &end) {
        if (begin == end) return end;
        Input_iterator last = begin;
        while (++begin != end) {
            if (!(*begin == *last) && ++last != begin)
                *last = *begin;
        }
        return last;
    }

    template<typename Fun, typename Input_iterator>
    Input_iterator Unique(Input_iterator begin, const Input_iterator &end, Fun equal) {
        if (begin == end) return end;
        Input_iterator last = begin;
        while (++begin != end) {
            if (!equal(*begin, *last) && ++last != begin)
                *last = *begin;
        }
        return last;
    }

    template<typename Input_iterator, typename Output_iterator>
    void Unique_Copy(Input_iterator begin, const Input_iterator &end, Output_iterator dest) {
        if (begin == end) return;
        Input_iterator last = begin;
        while (++begin != end) {
            while (begin != end && *last == *begin) ++begin;
            *dest = *last;
            last = begin;
        }
        *dest = *last;
    }

    template<typename Fun, typename Input_iterator, typename Output_iterator>
    void Unique_Copy(Input_iterator begin, const Input_iterator &end, Fun equal, Output_iterator dest) {
        if (begin == end) return;
        Input_iterator last = begin;
        while (++begin != end) {
            while (begin != end && equal(*last, *begin)) ++begin;
            *dest = *last;
            last = begin;
        }
        *dest = *last;
    }

    template<typename Input_iterator>
    void Rotate(Input_iterator begin, Input_iterator mid, const Input_iterator &end) {
        List<typename Iterator_traits<Input_iterator>::Value_type> store;
        Input_iterator temp = begin;
        while (temp != mid) {
            store.push_back(move(*temp));
            ++temp;
        }
        while (mid != end) {
            *begin = move(*mid);
            ++begin, ++mid;
        }
        for (auto &t: store) {
            *begin = move(t);
            ++begin;
        }
    }

    template<typename Input_iterator, typename Output_iterator>
    void Rotate_Copy(Input_iterator begin, Input_iterator mid,
                     const Input_iterator &end, Output_iterator dest) {
        Input_iterator record = mid;
        while (mid != end) {
            *dest = *mid;
            ++dest, ++mid;
        }
        while (begin != record) {
            *dest = *begin;
            ++dest, ++begin;
        }
    }

    template<typename Bidirectional_iterator>
    void Reverse(Bidirectional_iterator begin, Bidirectional_iterator end) {
        while (begin != end) {
            if (--end != begin) swap(*begin, *end);
            else break;
            ++begin;
        }
    }

    template<typename Bidirectional_iterator, typename Output_iterator>
    void Reverse_Copy(Bidirectional_iterator begin, Bidirectional_iterator end, Output_iterator dest) {
        while (begin != end) {
            *dest = *--end;
            ++dest;
        }
    }

    template<typename Random_iterator>
    void Shuffle(Random_iterator begin, Random_iterator end) {
        Size length = end - begin, M = length / 5, k;
        if (length < 2) return;
        if (M == 0) M = 1;
        for (Size i = M; i < length; ++i) {
            srand((unsigned) time(NULL));
            k = rand() % (i + 1);
            if (k < M)
                swap(begin[k], begin[i]);
        }
    }

    template<typename Input_iterator, typename Random_iterator>
    void Shuffle_Copy(Input_iterator begin, const Input_iterator &end, Random_iterator dest) {
        Size length = 0;
        while (begin != end) {
            *dest = *begin;
            ++dest, ++begin, ++length;
        }
        Size M = length / 5, k;
        if (length < 2) return;
        if (M == 0) M = 1;
        for (Size i = M; i < length; ++i) {
            srand((unsigned) time(NULL));
            k = rand() % (i + 1);
            if (k < M)
                swap(begin[k], begin[i]);
        }
    }


} // namespace STD

#endif
