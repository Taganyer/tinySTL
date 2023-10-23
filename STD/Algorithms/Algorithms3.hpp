//
// Created by 86152 on 2023/10/19.
//

#ifndef TINYSTL_ALGORITHMS3_HPP
#define TINYSTL_ALGORITHMS3_HPP

#include "../Config.hpp"
#include "../Allocater.hpp"
#include "../Iterator.hpp"

namespace STD {

    template<typename Type, typename Output_iterator>
    void Fill(Output_iterator begin, const Output_iterator &end, const Type &target) {
        while (begin != end) {
            *begin = target;
            ++begin;
        }
    }

    template<typename Type, typename Output_iterator>
    void Fill_N(Output_iterator begin, Signed_Size size, const Type &target) {
        while (size >= 0) {
            *begin = target;
            ++begin, --size;
        }
    }

    template<typename Object, typename Output_iterator>
    void Generate(Output_iterator begin, const Output_iterator &end, Object gen) {
        while (begin != end) {
            *begin = gen();
            ++begin;
        }
    }

    template<typename Object, typename Output_iterator>
    void Generate(Output_iterator begin, Signed_Size size, Object gen) {
        while (size >= 0) {
            *begin = gen();
            ++begin, --size;
        }
    }

    template<typename Input_iterator, typename Output_iterator>
    void Copy(Input_iterator begin, const Input_iterator &end, Output_iterator dest) {
        while (begin != end) {
            *dest = *begin;
            ++dest, ++begin;
        }
    }

    template<typename Input_iterator, typename Output_iterator>
    void Copy_N(Input_iterator begin, Signed_Size size, Output_iterator dest) {
        while (size >= 0) {
            *dest = *begin;
            ++dest, ++begin, --size;
        }
    }

    template<typename Fun, typename Input_iterator, typename Output_iterator>
    void Copy_If(Input_iterator begin, const Input_iterator &end, Output_iterator dest, Fun fun) {
        while (begin != end) {
            if (fun(*begin)) {
                *dest = *begin;
                ++dest;
            }
            ++begin;
        }
    }

    template<typename Input_iterator, typename Output_iterator>
    void Move(Input_iterator begin, const Input_iterator &end, Output_iterator dest) {
        while (begin != end) {
            *dest = move(*begin);
            ++dest, ++begin;
        }
    }

    template<typename Fun, typename Input_iterator, typename Output_iterator>
    void Transform(Input_iterator begin, const Input_iterator &end, Output_iterator dest, Fun fun) {
        while (begin != end) {
            *dest = fun(*begin);
            ++dest, ++begin;
        }
    }

    template<typename Fun, typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Transform(Input_iterator1 begin1, const Input_iterator1 &end,
                   Input_iterator2 begin2, Output_iterator dest, Fun fun) {
        while (begin1 != end) {
            *dest = fun(*begin1, *begin2);
            ++dest, ++begin1, ++begin2;
        }
    }

    template<typename Type, typename Input_iterator, typename Output_iterator>
    void Replace_Copy(Input_iterator begin, const Input_iterator &end,
                      Output_iterator dest, const Type &old_val, const Type &new_val) {
        while (begin != end) {
            if (*begin == old_val) *dest = new_val;
            else *dest = *begin;
            ++dest, ++begin;
        }
    }

    template<typename Fun, typename Type, typename Input_iterator, typename Output_iterator>
    void Replace_Copy(Input_iterator begin, const Input_iterator &end,
                      Output_iterator dest, Fun fun, const Type &new_val) {
        while (begin != end) {
            if (fun(*begin)) *dest = new_val;
            else *dest = *begin;
            ++dest, ++begin;
        }
    }

    template<typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Merge(Input_iterator1 begin1, const Input_iterator1 &end1,
               Input_iterator2 begin2, const Input_iterator2 &end2, Output_iterator dest) {
        while (begin1 != end1 || begin2 != end2) {
            if (begin1 != end1 && *begin1 < *begin2 || begin2 == end2) {
                *dest = *begin1;
                ++begin1;
            } else {
                *dest = *begin2;
                ++begin2;
            }
            ++dest;
        }
    }

    template<typename Fun, typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Merge(Input_iterator1 begin1, const Input_iterator1 &end1,
               Input_iterator2 begin2, const Input_iterator2 &end2, Output_iterator dest, Fun fun) {
        while (begin1 != end1 || begin2 != end2) {
            if (begin1 != end1 && fun(*begin1, *begin2) || begin2 == end2) {
                *dest = *begin1;
                ++begin1;
            } else {
                *dest = *begin2;
                ++begin2;
            }
            ++dest;
        }
    }

    template<typename Input_iterator1, typename Input_iterator2>
    inline void Iter_Swap(Input_iterator1 iter1, Input_iterator2 iter2) {
        swap(*iter1, *iter2);
    }

    template<typename Input_iterator1, typename Input_iterator2>
    void Swap_Ranges(Input_iterator1 begin1, const Input_iterator1 &end1, Input_iterator2 begin2) {
        while (begin1 != end1) {
            swap(*begin1, *begin2);
            ++begin1, ++begin2;
        }
    }

    template<typename Type, typename Input_iterator>
    void Replace(Input_iterator begin, const Input_iterator &end,
                 const Type &old_val, const Type &new_val) {
        while (begin != end) {
            if (*begin == old_val) *begin = new_val;
            ++begin;
        }
    }

    template<typename Fun, typename Type, typename Input_iterator>
    void Replace_if(Input_iterator begin, const Input_iterator &end, const Type &new_val, Fun fun) {
        while (begin != end) {
            if (fun(*begin)) *begin = new_val;
            ++begin;
        }
    }

    template<typename Bidirectional_iterator1, typename Bidirectional_iterator2>
    void Copy_BackWard(const Bidirectional_iterator1 &begin1, Bidirectional_iterator1 end1,
                       Bidirectional_iterator2 dest) {
        while (end1 != begin1)
            *--dest = *--end1;
    }

    template<typename Bidirectional_iterator1, typename Bidirectional_iterator2>
    void Move_BackWard(const Bidirectional_iterator1 &begin1, Bidirectional_iterator1 end1,
                       Bidirectional_iterator2 dest) {
        while (end1 != begin1)
            *--dest = move(*--end1);
    }

    template<typename Type, typename Output_iterator>
    void Iota(Output_iterator begin, const Output_iterator &end, Type init) {
        if (begin == end) return;
        *begin = init;
        ++begin;
        while (begin != end) {
            *begin = ++init;
            ++begin;
        }
    }

}

#endif //TINYSTL_ALGORITHMS3_HPP
