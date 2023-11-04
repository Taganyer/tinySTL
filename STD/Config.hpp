//
// Created by 86152 on 2023/9/24.
//

#ifndef TINYSTL_CONFIG_HPP
#define TINYSTL_CONFIG_HPP

namespace STD {

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define SIZEOF_LONG_ 4

#elif defined(__linux__) || defined(__gnu_linux__) || defined(__APPLE__) || defined(__MACH__)
#define SIZEOF_LONG_ 8

#else
#error "Unknown platform"
#endif

    using Size = unsigned long long;

    using Ptrdiff = long long;

    using Difference = long long;

    using Signed_Size = long long;

    using Int_8 = char;

    using Int_16 = short;

    using Int_32 = int;

    using Int_64 = long long;

    using UInt_8 = unsigned char;

    using UInt_16 = unsigned short;

    using UInt_32 = unsigned int;

    using UInt_64 = unsigned long long;

    constexpr short MAX_SHORT = 32767;

    constexpr short MIN_SHORT = -32768;

    constexpr short MAX_UINT_SHORT = -1;

    constexpr int MAX_INT = 2147483647;

    constexpr int MIN_INT = -2147483648;

    constexpr unsigned int MAX_UINT_INT = -1;

#if SIZEOF_LONG_ == 4
    constexpr long MAX_LONG = 2147483647;

    constexpr long MIN_LONG = -2147483648;

    constexpr unsigned long MAX_UINT_LONG = -1;

#elif SIZEOF_LONG_ == 8
    constexpr long MAX_LONG = 9223372036854775807;

    constexpr long MIN_LONG = -9223372036854775808;

    constexpr unsigned long MAX_UINT_LONG = -1;

#else
#error "Unknown platform"
#endif

    constexpr long long MAX_LLONG = 9223372036854775807;

    constexpr long long MIN_LLONG = -9223372036854775808;

    constexpr unsigned long long MAX_UINT_LLONG = -1;

    constexpr float MAX_FLOAT = 3.402823466e+38f;

    constexpr float MIN_FLOAT = -3.402823466e+38f;

    constexpr double MAX_DOUBLE = 1.7976931348623157e+308;

    constexpr double MIN_DOUBLE = -1.7976931348623157e+308;

    namespace Detail {

        using Size = STD::Size;

        using Signed_Size = STD::Signed_Size;

        using Ptrdiff = STD::Ptrdiff;

        using Difference = STD::Difference;

    }

}

#endif //TINYSTL_CONFIG_HPP
