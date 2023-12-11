//
// Created by 86152 on 2023/9/10.
//
#include "../String_Algorithms1.hpp"
#include "../../Vector.hpp"

namespace STD {

    void MemSet(void *target, int val, Size size) {
        auto *ptr = static_cast<int *>(target);
        while (size) ptr[--size] = val;
    }

    Size Get_Length(const char *target) {
        if (!target) return 0;
        const char *temp = target;
        Size len = 0;
        while (*temp != '\0')
            ++temp, ++len;
        return len;
    }

    const char *Boyer_Moore(const char *pattern, Size pattern_len,
                            const char *target, Size target_length) {
        if (*pattern == '\0' || *target == '\0')
            return nullptr;
        int store[256];
        Fill_With(store, -1, 256);
        for (int i = 0; i < pattern_len; ++i)
            store[pattern[i]] = i;
        int skip;
        for (int i = 0; i <= target_length - pattern_len; i += skip) {
            skip = 0;
            for (int j = (int) pattern_len - 1; j > -1; --j) {
                if (pattern[j] != target[i + j]) {
                    skip = j - store[target[i + j]];
                    if (skip < 1) skip = 1;
                    break;
                }
            }
            if (!skip) return target + i;
        }
        return nullptr;
    }

    const char *Boyer_Moore(const char *pattern, const char *target,
                            Size target_length) {
        return Boyer_Moore(pattern, Get_Length(pattern), target,
                           target_length);
    }

    const char *Boyer_Moore(const char *pattern, const char *target) {
        return Boyer_Moore(pattern, target, Get_Length(target));
    }

    const char *rBoyer_Moore(const char *pattern, Size pattern_len,
                             const char *target, Size target_length) {
        if (*pattern == '\0' || *target == '\0')
            return nullptr;
        int store[256];
        Fill_With(store, -1, 256);
        for (int i = 0; i < pattern_len; ++i)
            store[pattern[i]] = i;
        int skip;
        for (int i = (int) target_length - 1; i >= pattern_len - 1; i -= skip) {
            skip = 0;
            for (int j = 0; j < pattern_len; ++j) {
                if (pattern[j] != target[i + j - pattern_len + 1]) {
                    skip = store[target[i + j - pattern_len + 1]];
                    if (skip < 1) skip = 1;
                    break;
                }
            }
            if (!skip) return target + i - pattern_len + 1;
        }
        return nullptr;
    }

    const char *rBoyer_Moore(const char *pattern, const char *target,
                             Size target_length) {
        return rBoyer_Moore(pattern, Get_Length(pattern), target,
                            target_length);
    }

    const char *rBoyer_Moore(const char *pattern, const char *target) {
        return rBoyer_Moore(pattern, target, Get_Length(target));
    }

    const char *Knuth_Morris_Pratt(const char *pattern, Size pattern_len,
                                   const char *target, Size target_length) {
        if (*pattern == '\0' || *target == '\0') return nullptr;
        Vector<Vector<int>> dp(256, Vector<int>(pattern_len, 0));
        dp[*pattern][0] = 1;
        for (int X = 0, j = 1; j < pattern_len; ++j) {
            for (int c = 0; c < 256; ++c) dp[c][j] = dp[c][X];
            dp[pattern[j]][j] = j + 1;
            X = dp[pattern[j]][X];
        }
        int i, j;
        for (i = 0, j = 0; i < target_length && j < pattern_len; ++i)
            j = dp[target[i]][j];
        if (j == pattern_len) return pattern + i - pattern_len;
        else return nullptr;
    }

    const char *Knuth_Morris_Pratt(const char *pattern, const char *target,
                                   Size target_length) {
        return Knuth_Morris_Pratt(pattern, Get_Length(pattern), target,
                                  target_length);
    }

    const char *Knuth_Morris_Pratt(const char *pattern, const char *target) {
        return Knuth_Morris_Pratt(pattern, target, Get_Length(target));
    }

    const char *rKnuth_Morris_Pratt(const char *pattern, Size pattern_len,
                                    const char *target, Size target_length) {
        if (*pattern == '\0' || *target == '\0') return nullptr;
        Vector<Vector<int>> dp(256, Vector<int>(pattern_len, 0));
        dp[pattern[pattern_len - 1]][0] = 1;
        for (int X = 0, j = 1; j < pattern_len; ++j) {
            for (int c = 0; c < 256; ++c) dp[c][j] = dp[c][X];
            dp[pattern[j]][j] = j + 1;
            X = dp[pattern[j]][X];
        }
        int i, j;
        for (i = 0, j = 0; i < target_length && j < pattern_len; ++i)
            j = dp[target[i]][j];
        if (j == pattern_len) return pattern + i - pattern_len;
        else return nullptr;
    }

    const char *rKnuth_Morris_Pratt(const char *pattern,
                                    const char *target, Size target_length) {
        return rKnuth_Morris_Pratt(pattern, Get_Length(pattern),
                                   target, target_length);
    }

    const char *rKnuth_Morris_Pratt(const char *pattern, const char *target) {
        return rKnuth_Morris_Pratt(pattern, target, Get_Length(target));
    }

} // namespace STD
