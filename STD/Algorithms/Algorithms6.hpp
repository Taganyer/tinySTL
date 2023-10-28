//
// Created by 86152 on 2023/10/25.
//

#ifndef TINYSTL_ALGORITHMS6_HPP
#define TINYSTL_ALGORITHMS6_HPP

namespace STD {

    constexpr double PI = 3.14159265351415;

    template<typename Type>
    Type Abs(const Type &target) {
        return target >= 0 ? target : -target;
    }

    float Sqrt(float target) {
        float number = target, temp_f = target * 0.5f;
        Int_32 temp_l = *(Int_32 *) &number;
        temp_l = 0x5f3759df - (temp_l >> 1);
        number = *(float *) &temp_l;
        number *= 1.5f - temp_f * number * number;
        number *= 1.5f - temp_f * number * number;
        return target * number;
    }

    double Sqrt(double target) {
        double number = target, temp_f = target * 0.5;
        Int_64 temp_l = *(Int_64 *) &number;
        temp_l = 0x5fe6eb50c7b537aaLL - (temp_l >> 1);
        number = *(double *) &temp_l;
        number *= 1.5 - temp_f * number * number;
        number *= 1.5 - temp_f * number * number;
        number *= 1.5 - temp_f * number * number;
        return target * number;
    }

    template<typename Type>
    double Sqrt(Type target) {
        return Sqrt(static_cast<double>(target));
    }

    float Ceil(float target) {
        if (target == 0) return 0;
        auto ptr = reinterpret_cast<Int_32 *>(&target);
        int exponent = 0;
        for (int i = 0; i < 8; ++i)
            if (*ptr & (1 << (23 + i)))
                exponent |= 1 << i;
        exponent -= 127;
        if (exponent >= 23) return target;
        if (target < 0) {
            if (exponent < 0) return 0;
            else return static_cast<Int_64>(target);
        } else {
            if (exponent < 0) return 1;
            else {
                auto temp = static_cast<Int_64>(target);
                if (temp != target) ++temp;
                return temp;
            }
        }
    }

    double Ceil(double target) {
        if (target == 0) return 0;
        auto ptr = reinterpret_cast<Int_64 *>(&target);
        int exponent = 0;
        Int_64 mask = 1;
        for (int i = 0; i < 11; ++i)
            if (*ptr & (mask << (52 + i)))
                exponent |= 1 << i;
        exponent -= 1023;
        if (exponent >= 52) return target;
        if (target < 0) {
            if (exponent < 0) return 0;
            else return static_cast<Int_64>(target);
        } else {
            if (exponent < 0) return 1;
            else {
                auto temp = static_cast<Int_64>(target);
                if (temp != target) ++temp;
                return temp;
            }
        }
    }

    float Floor(float target) {
        if (target == 0) return 0;
        auto ptr = reinterpret_cast<Int_32 *>(&target);
        int exponent = 0;
        for (int i = 0; i < 8; ++i)
            if (*ptr & (1 << (23 + i)))
                exponent |= 1 << i;
        exponent -= 127;
        if (exponent >= 52) return target;
        if (target < 0 && exponent < 0) return -1;
        return static_cast<Int_64>(target);
    }

    double Floor(double target) {
        if (target == 0) return 0;
        auto ptr = reinterpret_cast<Int_64 *>(&target);
        int exponent = 0;
        Int_64 mask = 1;
        for (int i = 0; i < 11; ++i)
            if (*ptr & (mask << (52 + i)))
                exponent |= 1 << i;
        exponent -= 1023;
        if (exponent >= 52) return target;
        if (target < 0 && exponent < 0) return -1;
        return static_cast<Int_64>(target);
    }

    template<typename Type, typename Input_iterator>
    Type Accumulate(Input_iterator begin, const Input_iterator &end, Type init) {
        while (begin != end) {
            init = init + *begin;
            ++begin;
        }
        return init;
    }

    template<typename Fun, typename Type, typename Input_iterator>
    Type Accumulate(Input_iterator begin, const Input_iterator &end, Type init, Fun fun) {
        while (begin != end) {
            init = fun(init, *begin);
            ++begin;
        }
        return init;
    }

}

#endif //TINYSTL_ALGORITHMS6_HPP
