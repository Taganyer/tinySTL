//
// Created by 86152 on 2023/9/10.
//
#include "../Algorithms.hpp"
using namespace STD;

Size STD::calculateLength(const char *target) {
    const char *temp = target;
    Size len = 0;
    while (*temp != '\0') ++temp, ++len;
    return len;
}

const char *STD::Boyer_Moore(const char *pattern, Size pattern_len, const char *target, Size target_length) {
    if (*pattern == '\0' || *target == '\0') return nullptr;
    int store[256];
    Memset(store, -1, 256);
    for (int i = 0; i < pattern_len; ++i) store[pattern[i]] = i;
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

const char *STD::Boyer_Moore(const char *pattern, const char *target, Size target_length) {
    return Boyer_Moore(pattern, calculateLength(pattern), target, target_length);
}

const char *STD::Boyer_Moore(const char *pattern, const char *target) {
    return Boyer_Moore(pattern, target, calculateLength(target));
}

const char *STD::rBoyer_Moore(const char *pattern, Size pattern_len, const char *target, Size target_length) {
    if (*pattern == '\0' || *target == '\0') return nullptr;
    int store[256];
    Memset(store, -1, 256);
    for (int i = 0; i < pattern_len; ++i) store[pattern[i]] = i;
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

const char *STD::rBoyer_Moore(const char *pattern, const char *target, Size target_length) {
    return rBoyer_Moore(pattern, calculateLength(pattern), target, target_length);
}

const char *STD::rBoyer_Moore(const char *pattern, const char *target) {
    return rBoyer_Moore(pattern, target, calculateLength(target));
}

//const char *STD::Knuth_Morris_Pratt(const char *pattern, Size pattern_len, const char *target, Size target_length) {
//    if (*pattern == '\0' || *target == '\0') return nullptr;
//    int dp[256][pattern_len];
//    for (int i = 0; i < 256; ++i) Memset(dp[i], 0, pattern_len);
//    dp[*pattern][0] = 1;
//    for (int X = 0, j = 1; j < pattern_len; ++j) {
//        for (int c = 0; c < 256; ++c) dp[c][j] = dp[c][X];
//        dp[pattern[j]][j] = j + 1;
//        X = dp[pattern[j]][X];
//    }
//    int i, j;
//    for (i = 0, j = 0; i < target_length && j < pattern_len; ++i) j = dp[target[i]][j];
//    if (j == pattern_len) return pattern + i - pattern_len;
//    else return nullptr;
//}

//const char *STD::Knuth_Morris_Pratt(const char *pattern, const char *target, Size target_length) {
//    return Knuth_Morris_Pratt(pattern, calculateLength(pattern), target, target_length);
//}
//
//const char *STD::Knuth_Morris_Pratt(const char *pattern, const char *target) {
//    return Knuth_Morris_Pratt(pattern, target, calculateLength(target));
//}

//const char *STD::rKnuth_Morris_Pratt(const char *pattern, Size pattern_len, const char *target, Size target_length) {
//    if (*pattern == '\0' || *target == '\0') return nullptr;
//    int dp[256][pattern_len];
//    for (int i = 0; i < 256; ++i) Memset(dp[i], 0, pattern_len);
//    dp[pattern[pattern_len - 1]][0] = 1;
//    for (int X = 0, j = 1; j < pattern_len; ++j) {
//        for (int c = 0; c < 256; ++c) dp[c][j] = dp[c][X];
//        dp[pattern[j]][j] = j + 1;
//        X = dp[pattern[j]][X];
//    }
//    int i, j;
//    for (i = 0, j = 0; i < target_length && j < pattern_len; ++i) j = dp[target[i]][j];
//    if (j == pattern_len) return pattern + i - pattern_len;
//    else return nullptr;
//}

//const char *STD::rKnuth_Morris_Pratt(const char *pattern, const char *target, Size target_length) {
//    return rKnuth_Morris_Pratt(pattern, calculateLength(pattern), target, target_length);
//}
//
//const char *STD::rKnuth_Morris_Pratt(const char *pattern, const char *target) {
//    return rKnuth_Morris_Pratt(pattern, target, calculateLength(target));
//}
