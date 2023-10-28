//
// Created by 86152 on 2023/9/24.
//

#ifndef TINYSTL_CONFIG_HPP
#define TINYSTL_CONFIG_HPP

namespace STD {

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

    constexpr int MAX_INT = 2147483647;

    constexpr int MIN_INT = -2147483648;

    constexpr unsigned int MAX_UINT_INT = 4294967295;

    constexpr long long MAX_LLONG = 9223372036854775807;

    constexpr long long MIN_LLONG = -9223372036854775808;

    constexpr unsigned long long MAX_UINT_LLONG = -1;

    namespace Detail {

        using Size = STD::Size;

        using Signed_Size = STD::Signed_Size;

        using Ptrdiff = STD::Ptrdiff;

        using Difference = STD::Difference;

    }

}

#endif //TINYSTL_CONFIG_HPP
