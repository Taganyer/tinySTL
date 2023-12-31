#ifndef TINYSTL_ALGORITHMS_HPP1
#define TINYSTL_ALGORITHMS_HPP1

namespace STD {

    template<typename Type, typename Input_Iterator>
    Input_Iterator Find(Input_Iterator begin, const Input_Iterator &end, const Type &val) {
        while (begin != end)
            if (*begin == val) break;
            else ++begin;
        return begin;
    }

    template<typename Fun, typename Input_Iterator>
    Input_Iterator Find_If(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        while (begin != end)
            if (fun(*begin)) break;
            else ++begin;
        return begin;
    }

    template<typename Fun, typename Input_Iterator>
    Input_Iterator Find_If_Not(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        while (begin != end)
            if (!fun(*begin)) break;
            else ++begin;
        return begin;
    }

    template<typename Type, typename Input_Iterator>
    Size Count(Input_Iterator begin, const Input_Iterator &end, const Type &val) {
        Size count = 0;
        while (begin != end) {
            if (*begin == val) ++count;
            ++begin;
        }
        return count;
    }

    template<typename Fun, typename Input_Iterator>
    Size Count_If(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        Size count = 0;
        while (begin != end) {
            if (fun(*begin)) ++count;
            ++begin;
        }
        return count;
    }

    template<typename Fun, typename Input_Iterator>
    Size Count_If_Not(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        Size count = 0;
        while (begin != end) {
            if (!fun(*begin)) ++count;
            ++begin;
        }
        return count;
    }

    template<typename Fun, typename Input_Iterator>
    bool All_Of(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        while (begin != end) {
            if (!fun(*begin)) return false;
            ++begin;
        }
        return true;
    }

    template<typename Fun, typename Input_Iterator>
    bool Any_Of(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        while (begin != end) {
            if (fun(*begin)) return true;
            ++begin;
        }
        return false;
    }

    template<typename Fun, typename Input_Iterator>
    bool None_Of(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        while (begin != end) {
            if (fun(*begin)) return false;
            ++begin;
        }
        return true;
    }

    template<typename Type, typename Input_Iterator>
    Input_Iterator Adjacent_Find(Input_Iterator begin, const Input_Iterator &end) {
        Input_Iterator temp = begin++;
        while (begin != end) {
            if (*temp == *begin) return temp;
            temp = begin++;
        }
        return begin;
    }

    template<typename Fun, typename Input_Iterator>
    Input_Iterator Adjacent_Find(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        Input_Iterator temp = begin++;
        while (begin != end) {
            if (fun(*temp, *begin)) return temp;
            temp = begin++;
        }
        return begin;
    }

    template<typename Input_Iterator>
    Input_Iterator Search_N(Input_Iterator begin, const Input_Iterator &end, Signed_Size size) {
        if (size <= 0) return end;
        Signed_Size count = 0;
        Input_Iterator temp = begin;
        while (temp != end) {
            if (*temp == *begin) ++count;
            else {
                if (count == size) return begin;
                begin = temp;
                count = 1;
            }
            ++temp;
        }
        if (count == size) return begin;
        else return temp;
    }

    template<typename Type, typename Input_Iterator>
    Input_Iterator Search_N(Input_Iterator begin, const Input_Iterator &end, Signed_Size size, const Type &val) {
        if (size <= 0) return end;
        Input_Iterator temp = begin;
        Signed_Size count = 0;
        while (temp != end) {
            if (*temp == val) {
                if (!count) begin = temp;
                ++count;
            } else {
                if (count == size) return begin;
                else count = 0;
            }
            ++temp;
        }
        if (count == size) return begin;
        else return temp;
    }

    template<typename Fun, typename Type, typename Input_Iterator>
    Input_Iterator
    Search_N(Input_Iterator begin, const Input_Iterator &end, Signed_Size size, const Type &val, Fun fun) {
        if (size <= 0) return end;
        Input_Iterator temp = begin;
        Signed_Size count = 0;
        while (temp != end) {
            if (fun(*temp, val)) {
                if (!count) begin = temp;
                ++count;
            } else {
                if (count == size) return begin;
                else count = 0;
            }
            ++temp;
        }
        if (count == size) return begin;
        else return temp;
    }

    template<typename Input_Iterator1, typename Input_Iterator2>
    Input_Iterator1 Find_First_Of(Input_Iterator1 begin1, const Input_Iterator1 &end1,
                                  Input_Iterator2 begin2, const Input_Iterator2 &end2) {
        if (begin2 == end2) return end1;
        while (begin1 != end1) {
            if (*begin1 == *begin2) {
                Input_Iterator1 temp1 = begin1;
                Input_Iterator2 temp2 = begin2;
                while (temp1 != end1 && temp2 != end2) {
                    if (*temp1 != *temp2) break;
                    ++temp1, ++temp2;
                }
                if (temp2 == end2) return begin1;
            }
            ++begin1;
        }
        return begin1;
    }

    template<typename Input_Iterator1, typename Input_Iterator2, typename Fun>
    Input_Iterator1 Find_First_Of(Input_Iterator1 begin1, const Input_Iterator1 &end1,
                                  Input_Iterator2 begin2, const Input_Iterator2 &end2, Fun fun) {
        if (begin2 == end2) return end1;
        while (begin1 != end1) {
            if (fun(*begin1, *begin2)) {
                Input_Iterator1 temp1 = begin1;
                Input_Iterator2 temp2 = begin2;
                while (temp1 != end1 && temp2 != end2) {
                    if (!fun(*temp1, *temp2)) break;
                    ++temp1, ++temp2;
                }
                if (temp2 == end2) return begin1;
            }
            ++begin1;
        }
        return begin1;
    }

    template<typename Backward_Iterator1, typename Backward_Iterator2>
    Backward_Iterator1 Find_End(const Backward_Iterator1 &begin1, Backward_Iterator1 end1,
                                const Backward_Iterator2 &begin2, Backward_Iterator2 end2) {
        if (begin2 == end2) return end1;
        Backward_Iterator1 end = end1;
        --end1, --end2;
        while (true) {
            if (*end1 == *end2) {
                Backward_Iterator1 temp1 = end1;
                Backward_Iterator2 temp2 = end2;
                while (true) {
                    if (*temp1 != *temp2 || temp1 == begin1 || temp2 == begin2) break;
                    --temp1, --temp2;
                }
                if (temp2 == begin2) return temp1;
            }
            if (end1 == begin1) break;
            --end1;
        }
        return end;
    }

    template<typename Backward_Iterator1, typename Backward_Iterator2, typename Fun>
    Backward_Iterator1 Find_End(Backward_Iterator1 begin1, const Backward_Iterator1 &end1,
                                Backward_Iterator2 begin2, const Backward_Iterator2 &end2, Fun fun) {
        if (begin2 == end2) return end1;
        Backward_Iterator1 end = end1;
        --end1, --end2;
        while (true) {
            if (fun(*end1, *end2)) {
                Backward_Iterator1 temp1 = end1;
                Backward_Iterator2 temp2 = end2;
                while (true) {
                    if (!fun(*temp1, *temp2) || temp1 == begin1 || temp2 == begin2) break;
                    --temp1, --temp2;
                }
                if (temp2 == begin2) return temp1;
            }
            if (end1 == begin1) break;
            --end1;
        }
        return end;
    }

    template<typename Input_Iterator1, typename Input_Iterator2>
    Input_Iterator1 Search(Input_Iterator1 begin1, const Input_Iterator1 &end1,
                           Input_Iterator2 begin2, const Input_Iterator2 &end2) {
        return Find_First_Of(begin1, end1, begin2, end2);
    }

    template<typename Input_Iterator1, typename Input_Iterator2, typename Fun>
    Input_Iterator1 Search(Input_Iterator1 begin1, const Input_Iterator1 &end1,
                           Input_Iterator2 begin2, const Input_Iterator2 &end2, Fun fun) {
        return Find_First_Of(begin1, end1, begin2, end2, fun);
    }

    template<typename Fun, typename Input_Iterator>
    void For_Each(Input_Iterator begin, const Input_Iterator &end, Fun fun) {
        while (begin != end) {
            fun(*begin);
            ++begin;
        }
    }

    template<typename Input_Iterator1, typename Input_Iterator2>
    Pair<Input_Iterator1, Input_Iterator2>
    Mismatch(Input_Iterator1 begin1, const Input_Iterator1 &end1,
             Input_Iterator2 begin2, const Input_Iterator2 &end2) {
        while (begin1 != end1 && begin2 != end2) {
            if (*begin1 != *begin2) break;
            ++begin1, ++begin2;
        }
        return {begin1, begin2};
    }

    template<typename Input_Iterator1, typename Input_Iterator2, typename Fun>
    Pair<Input_Iterator1, Input_Iterator2>
    Mismatch(Input_Iterator1 begin1, const Input_Iterator1 &end1,
             Input_Iterator2 begin2, const Input_Iterator2 &end2, Fun fun) {
        while (begin1 != end1 && begin2 != end2) {
            if (fun(*begin1, *begin2)) break;
            ++begin1, ++begin2;
        }
        return {begin1, begin2};
    }

    template<typename Input_Iterator1, typename Input_Iterator2>
    bool Equal_(Input_Iterator1 begin1, const Input_Iterator1 &end1,
                Input_Iterator2 begin2, const Input_Iterator2 &end2) {
        while (begin1 != end1 && begin2 != end2) {
            if (*begin1 != *begin2) return false;
            ++begin1, ++begin2;
        }
        return begin1 == end1 && begin2 == end2;
    }

    template<typename Fun, typename Input_Iterator1, typename Input_Iterator2>
    bool Equal_(Input_Iterator1 begin1, const Input_Iterator1 &end1,
                Input_Iterator2 begin2, const Input_Iterator2 &end2, Fun fun) {
        while (begin1 != end1 && begin2 != end2) {
            if (fun(*begin1, *begin2)) return false;
            ++begin1, ++begin2;
        }
        return begin1 == end1 && begin2 == end2;
    }

    template<typename Arg, typename Random_iterator>
    Random_iterator
    Binary_Search(Random_iterator begin, Random_iterator end, const Arg &target) {
        --end;
        while (begin <= end) {
            Random_iterator mid = begin + (end - begin) / 2;
            if (target == *mid) return mid;
            else if (target < *mid) end = --mid;
            else begin = ++mid;
        }
        return begin;
    }

    template<typename Fun, typename Arg, typename Random_iterator>
    Random_iterator
    Binary_Search(Random_iterator begin, Random_iterator end, const Arg &target, Fun less) {
        --end;
        while (begin <= end) {
            Random_iterator mid = begin + (end - begin) / 2;
            if (!less(target, *mid) && !less(*mid, target)) return mid;
            else if (less(target, *mid)) end = --mid;
            else begin = ++mid;
        }
        return begin;
    }

    template<typename Arg, typename Input_iterator>
    Input_iterator Lower_Bound(Input_iterator begin, const Input_iterator &end, const Arg &value) {
        while (begin != end) {
            if (*begin >= value) return begin;
            ++begin;
        }
        return begin;
    }

    template<typename Fun, typename Arg, typename Input_iterator>
    Input_iterator Lower_Bound(Input_iterator begin, const Input_iterator &end, const Arg &value, Fun compare) {
        while (begin != end) {
            if (!compare(*begin, value)) return begin;
            ++begin;
        }
        return begin;
    }

    template<typename Arg, typename Input_iterator>
    Input_iterator Upper_Bound(Input_iterator begin, const Input_iterator &end, const Arg &value) {
        while (begin != end) {
            if (*begin > value) return begin;
            ++begin;
        }
        return begin;
    }

    template<typename Fun, typename Arg, typename Input_iterator>
    Input_iterator Upper_Bound(Input_iterator begin, const Input_iterator &end, const Arg &value, Fun compare) {
        while (begin != end) {
            if (compare(value, *begin)) return begin;
            ++begin;
        }
        return begin;
    }

    template<typename Arg, typename Input_iterator>
    Pair<Input_iterator, Input_iterator>
    Equal_Range(Input_iterator begin, const Input_iterator &end, const Arg &value) {
        return {Lower_Bound(begin, end, value), Upper_Bound(begin, end, value)};
    }

    template<typename Fun, typename Arg, typename Input_iterator>
    Pair<Input_iterator, Input_iterator>
    Equal_Range(Input_iterator begin, const Input_iterator &end, const Arg &value, Fun compare) {
        return {Lower_Bound(begin, end, value, compare), Upper_Bound(begin, end, value, compare)};
    }

} // namespace STD

#endif // TINYSTL_ALGORITHMS_HPP1