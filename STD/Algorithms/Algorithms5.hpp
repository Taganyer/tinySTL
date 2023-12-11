//
// Created by 86152 on 2023/10/23.
//

#ifndef TINYSTL_ALGORITHMS5_HPP
#define TINYSTL_ALGORITHMS5_HPP

namespace STD {

    template<typename Input_iterator1, typename Input_iterator2>
    bool Includes(Input_iterator1 begin1, const Input_iterator1 &end1,
                  Input_iterator2 begin2, const Input_iterator2 &end2) {
        Input_iterator2 last = begin2;
        while (begin1 != end1 && begin2 != end2) {
            while (begin1 != end1 && *begin1 != *last) ++begin1;
            while (begin2 != end2 && *last == *begin2) ++begin2;
            last = begin2;
        }
        return begin2 == end2 && begin1 != end1;
    }

    template<typename Fun, typename Input_iterator1, typename Input_iterator2>
    bool Includes(Input_iterator1 begin1, const Input_iterator1 &end1,
                  Input_iterator2 begin2, const Input_iterator2 &end2, Fun compare) {
        Input_iterator2 last = begin2;
        while (begin1 != end1 && begin2 != end2) {
            while (begin1 != end1 && !compare(*begin1, *last)) ++begin1;
            while (begin2 != end2 && compare(*last, *begin2)) ++begin2;
            last = begin2;
        }
        return begin2 == end2 && begin1 != end1;
    }

    template<typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Set_Union(Input_iterator1 begin1, const Input_iterator1 &end1,
                   Input_iterator2 begin2, const Input_iterator2 &end2, Output_iterator dest) {
        while (begin1 != end1 || begin2 != end2) {
            if (begin2 == end2 || begin1 != end1 && *begin1 < *begin2) {
                *dest = *begin1;
                Input_iterator1 last1 = begin1;
                while (++begin1 != end1 && *last1 == *begin1);
                while (begin2 != end2 && *last1 == *begin2) ++begin2;
            } else {
                *dest = *begin2;
                Input_iterator2 last2 = begin2;
                while (++begin2 != end2 && *last2 == *begin2);
                while (begin1 != end1 && *last2 == *begin1) ++begin1;
            }
            ++dest;
        }
    }

    template<typename Fun1, typename Fun2, typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Set_Union(Input_iterator1 begin1, const Input_iterator1 &end1,
                   Input_iterator2 begin2, const Input_iterator2 &end2,
                   Output_iterator dest, Fun1 equal, Fun2 compare) {
        while (begin1 != end1 || begin2 != end2) {
            if (begin2 == end2 || begin1 != end1 && compare(*begin1, *begin2)) {
                *dest = *begin1;
                Input_iterator1 last1 = begin1;
                while (++begin1 != end1 && equal(*last1, *begin1));
                while (begin2 != end2 && equal(*last1, *begin2)) ++begin2;
            } else {
                *dest = *begin2;
                Input_iterator2 last2 = begin2;
                while (++begin2 != end2 && equal(*last2, *begin2));
                while (begin1 != end1 && equal(*last2, *begin1)) ++begin1;
            }
            ++dest;
        }
    }

    template<typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Set_Intersection(Input_iterator1 begin1, const Input_iterator1 &end1,
                          Input_iterator2 begin2, const Input_iterator2 &end2, Output_iterator dest) {
        while (begin1 != end1 && begin2 != end2) {
            while (begin1 != end1 && *begin1 < *begin2) ++begin1;
            if (begin1 == end1) return;
            while (begin2 != end2 && *begin2 < *begin1) ++begin2;
            if (begin2 == end2) return;
            *dest = *begin1;
            ++dest, ++begin1, ++begin2;
        }
    }

    template<typename Fun, typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Set_Intersection(Input_iterator1 begin1, const Input_iterator1 &end1,
                          Input_iterator2 begin2, const Input_iterator2 &end2,
                          Output_iterator dest, Fun compare) {
        while (begin1 != end1 && begin2 != end2) {
            while (begin1 != end1 && compare(*begin1, *begin2)) ++begin1;
            if (begin1 == end1) return;
            while (begin2 != end2 && compare(*begin2, *begin1)) ++begin2;
            if (begin2 == end2) return;
            *dest = *begin1;
            ++dest, ++begin1, ++begin2;
        }
    }

    template<typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Set_Difference(Input_iterator1 begin1, const Input_iterator1 &end1,
                        Input_iterator2 begin2, const Input_iterator2 &end2, Output_iterator dest) {
        while (begin1 != end1) {
            while (begin1 != end1 && *begin1 < *begin2) {
                *dest = *begin1;
                ++dest, ++begin1;
            }
            while (begin2 != end2 && !(*begin1 < *begin2)) ++begin2;
            if (begin2 == end2) {
                while (begin1 != end1) {
                    *dest = *begin1;
                    ++dest, ++begin1;
                }
            }
        }
    }

    template<typename Fun, typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Set_Difference(Input_iterator1 begin1, const Input_iterator1 &end1,
                        Input_iterator2 begin2, const Input_iterator2 &end2,
                        Output_iterator dest, Fun compare) {
        while (begin1 != end1) {
            while (begin1 != end1 && compare(*begin1, *begin2)) {
                *dest = *begin1;
                ++dest, ++begin1;
            }
            while (begin2 != end2 && !compare(*begin1, *begin2)) ++begin2;
            if (begin2 == end2) {
                while (begin1 != end1) {
                    *dest = *begin1;
                    ++dest, ++begin1;
                }
            }
        }
    }

    template<typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Set_Symmetric_Difference(Input_iterator1 begin1, const Input_iterator1 &end1,
                                  Input_iterator2 begin2, const Input_iterator2 &end2, Output_iterator dest) {
        while (begin1 != end1 || begin2 != end2) {
            if (begin2 == end2 || begin1 != end1 && *begin1 < *begin2) {
                *dest = *begin1;
                ++dest, ++begin1;
            } else if (begin1 == end1 || begin2 != end2 && *begin2 < *begin1) {
                *dest = *begin2;
                ++dest, ++begin2;
            } else {
                Input_iterator1 record = begin1;
                while (begin1 != end1 && *begin1 == *record) ++begin1;
                while (begin2 != end2 && *begin2 == *record) ++begin2;
            }
        }
    }

    template<typename Fun1, typename Fun2, typename Input_iterator1, typename Input_iterator2, typename Output_iterator>
    void Set_Symmetric_Difference(Input_iterator1 begin1, const Input_iterator1 &end1,
                                  Input_iterator2 begin2, const Input_iterator2 &end2,
                                  Output_iterator dest, Fun1 equal, Fun2 compare) {
        while (begin1 != end1 || begin2 != end2) {
            if (begin2 == end2 || begin1 != end1 && compare(*begin1, *begin2)) {
                *dest = *begin1;
                ++dest, ++begin1;
            } else if (begin1 == end1 || begin2 != end2 && compare(*begin2, *begin1)) {
                *dest = *begin2;
                ++dest, ++begin2;
            } else {
                Input_iterator1 record = begin1;
                while (begin1 != end1 && equal(*begin1, *record)) ++begin1;
                while (begin2 != end2 && equal(*begin2, *record)) ++begin2;
            }
        }
    }

    template<typename Input_iterator1, typename Input_iterator2>
    bool Is_Permutation(Input_iterator1 begin1, const Input_iterator1 &end1, Input_iterator2 begin2) {
        Input_iterator2 end2 = Advance(begin1, Distance(begin1, end1));
        while (begin1 != end1) {
            if (*begin1 == *begin2) break;
            ++begin1, ++begin2;
        }
        while (begin1 != end1) {
            Size t1 = Count(begin2, end2, *begin1);
            Size t2 = Count(begin1, end1, *begin1);
            if (t1 < t2 || t1 == 0) return false;
        }
        return true;
    }

    template<typename Fun, typename Input_iterator1, typename Input_iterator2>
    bool Is_Permutation(Input_iterator1 begin1, const Input_iterator1 &end1,
                        Input_iterator2 begin2, Fun fun) {
        Input_iterator2 end2 = Advance(begin1, Distance(begin1, end1));
        while (begin1 != end1) {
            if (fun(*begin1, *begin2)) break;
            ++begin1, ++begin2;
        }
        while (begin1 != end1) {
            Size t1 = Count_If(begin2, end2, *begin1, fun);
            Size t2 = Count_If(begin1, end1, *begin1, fun);
            if (t1 < t2 || t1 == 0) return false;
        }
        return true;
    }

    template<typename Bidirectional_iterator>
    void Next_Permutation(Bidirectional_iterator begin, const Bidirectional_iterator &end) {
        if (begin == end) return;
        Bidirectional_iterator record = end, last = --end;
        if (begin == end) return;
        --last;
        while (end != begin && *end < *last) --end, --last;
        if (end != record) {
            swap(*last, *Find_Next(end, record));
            ++last, ++end;
        }
        while (end != record) {
            Bidirectional_iterator temp = Min_Element(last, record);
            if (last != temp) swap(*last, *temp);
            ++last, ++end;
        }
    }

    template<typename Fun, typename Bidirectional_iterator>
    void Next_Permutation(Bidirectional_iterator begin, const Bidirectional_iterator &end, Fun compare) {
        if (begin == end) return;
        Bidirectional_iterator record = end, last = --end;
        if (begin == end) return;
        --last;
        while (end != begin && compare(*end, *last)) --end, --last;
        if (end != record) {
            swap(*last, *Find_Next(end, record, compare));
            ++last, ++end;
        }
        while (end != record) {
            Bidirectional_iterator temp = Min_Element(last, record, compare);
            if (last != temp) swap(*last, *temp);
            ++last, ++end;
        }
    }

    template<typename Bidirectional_iterator>
    void Prev_Permutation(Bidirectional_iterator begin, const Bidirectional_iterator &end) {
        if (begin == end) return;
        Bidirectional_iterator record = end, last = --end;
        if (begin == end) return;
        --last;
        while (last != begin && *last < *end) --end, --last;
        if (end != record) {
            swap(*last, *Find_Last(end, record));
            ++last, ++end;
        }
        while (end != record) {
            Bidirectional_iterator temp = Max_Element(last, record);
            if (last != temp) swap(*last, *temp);
            ++last, ++end;
        }
    }

    template<typename Fun, typename Bidirectional_iterator>
    void Prev_Permutation(Bidirectional_iterator begin, const Bidirectional_iterator &end, Fun compare) {
        if (begin == end) return;
        Bidirectional_iterator record = end, last = --end;
        if (begin == end) return;
        --last;
        while (end != begin && compare(*end, *last)) --end, --last;
        if (end != record) {
            swap(*last, *Find_Last(end, record, compare));
            ++last, ++end;
        }
        while (end != record) {
            Bidirectional_iterator temp = Min_Element(last, record, compare);
            if (last != temp) swap(*last, *temp);
            ++last, ++end;
        }
    }

}

#endif //TINYSTL_ALGORITHMS5_HPP
