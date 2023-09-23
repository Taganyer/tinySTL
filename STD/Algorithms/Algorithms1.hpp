#ifndef TINYSTL_ALGORITHMS_HPP1
#define TINYSTL_ALGORITHMS_HPP1

#include "../Allocater.hpp"

namespace STD {

Size calculateLength(const char *target);

const char *Boyer_Moore(const char *pattern, Size pattern_len,
                        const char *target, Size target_length);

const char *Boyer_Moore(const char *pattern, const char *target,
                        Size target_length);

const char *Boyer_Moore(const char *pattern, const char *target);

const char *rBoyer_Moore(const char *pattern, Size pattern_len,
                         const char *target, Size target_length);

const char *rBoyer_Moore(const char *pattern, const char *target,
                         Size target_length);

const char *rBoyer_Moore(const char *pattern, const char *target);

const char *Knuth_Morris_Pratt(const char *pattern, Size pattern_len,
                               const char *target, Size target_length);

const char *Knuth_Morris_Pratt(const char *pattern, const char *target,
                               Size target_length);

const char *Knuth_Morris_Pratt(const char *pattern, const char *target);

const char *rKnuth_Morris_Pratt(const char *pattern, Size pattern_len,
                                const char *target, Size target_length);

const char *rKnuth_Morris_Pratt(const char *pattern, const char *target,
                                Size target_length);

const char *rKnuth_Morris_Pratt(const char *pattern, const char *target);

} // namespace STD

#endif // TINYSTL_ALGORITHMS_HPP