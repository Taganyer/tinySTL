//
// Created by 86152 on 2023/9/23.
//

#ifndef ALGORITHMS2_HPP2
#define ALGORITHMS2_HPP2

#include "../Iter.hpp"

namespace STD {

template <typename Arg>
void sort(const cIter<Arg> &begin, const cIter<Arg> &right);

template <typename Arg>
void sort(const Iter<Arg> &begin, const Iter<Arg> &right);

template <typename Arg>
void sort(const cBidirectional_Iter<Arg> &begin,
          const cBidirectional_Iter<Arg> &right);

template <typename Arg>
void sort(const Bidirectional_Iter<Arg> &begin,
          const Bidirectional_Iter<Arg> &right);

template <typename Arg>
void sort(const cRandom_Iter<Arg> &begin, const cRandom_Iter<Arg> &right);

template <typename Arg>
void sort(const Random_Iter<Arg> &begin, const Random_Iter<Arg> &right);




} // namespace STD

#endif
