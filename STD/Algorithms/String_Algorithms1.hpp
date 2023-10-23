//
// Created by 86152 on 2023/10/23.
//

#ifndef TINYSTL_STRING_ALGORITHMS1_HPP
#define TINYSTL_STRING_ALGORITHMS1_HPP

#include "../Allocater.hpp"
#include "Container_algorithms.hpp"

namespace STD {

    Size Get_Length(const char *target);

    const char *Boyer_Moore(const char *pattern, Size pattern_len, const char *target, Size target_length);

    const char *Boyer_Moore(const char *pattern, const char *target, Size target_length);

    const char *Boyer_Moore(const char *pattern, const char *target);

    const char *rBoyer_Moore(const char *pattern, Size pattern_len, const char *target, Size target_length);

    const char *rBoyer_Moore(const char *pattern, const char *target, Size target_length);

    const char *rBoyer_Moore(const char *pattern, const char *target);

    const char *Knuth_Morris_Pratt(const char *pattern, Size pattern_len, const char *target, Size target_length);

    const char *Knuth_Morris_Pratt(const char *pattern, const char *target, Size target_length);

    const char *Knuth_Morris_Pratt(const char *pattern, const char *target);

    const char *rKnuth_Morris_Pratt(const char *pattern, Size pattern_len, const char *target, Size target_length);

    const char *rKnuth_Morris_Pratt(const char *pattern, const char *target, Size target_length);

    const char *rKnuth_Morris_Pratt(const char *pattern, const char *target);

}

#endif //TINYSTL_STRING_ALGORITHMS1_HPP
