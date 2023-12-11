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
        if (target > MAX_LLONG || target < MIN_LLONG ||
            target == static_cast<Int_64>(target))
            return target;
        if (target < 0) return static_cast<Int_64>(target);
        else return static_cast<Int_64>(target) + 1;
    }

    double Ceil(double target) {
        if (target > MAX_LLONG || target < MIN_LLONG ||
            target == static_cast<Int_64>(target))
            return target;
        if (target < 0) return static_cast<Int_64>(target);
        else return static_cast<Int_64>(target) + 1;
    }

    float Floor(float target) {
        if (target > MAX_LLONG || target < MIN_LLONG ||
            target == static_cast<Int_64>(target))
            return target;
        if (target < 0) return static_cast<Int_64>(target) - 1;
        else return static_cast<Int_64>(target);
    }

    double Floor(double target) {
        if (target > MAX_LLONG || target < MIN_LLONG ||
            target == static_cast<Int_64>(target))
            return target;
        if (target < 0) return static_cast<Int_64>(target) - 1;
        else return static_cast<Int_64>(target);
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

    namespace Detail {
        const double magic = 0.017453292519943295769236907684886;

        const double sin_table[] = {
                0.0,                                    //sin(0)
                0.17364817766693034885171662676931,    //sin(10)
                0.34202014332566873304409961468226,    //sin(20)
                0.5,                                   //sin(30)
                0.64278760968653932632264340990726,    //sin(40)
                0.76604444311897803520239265055542,    //sin(50)
                0.86602540378443864676372317075294,    //sin(60)
                0.93969262078590838405410927732473,    //sin(70)
                0.98480775301220805936674302458952,    //sin(80)
                1.0                                     //sin(90)
        };

        const double cos_table[] = {
                1.0,                                   //cos(0)
                0.99984769515639123915701155881391,    //cos(1)
                0.99939082701909573000624344004393,    //cos(2)
                0.99862953475457387378449205843944,    //cos(3)
                0.99756405025982424761316268064426,    //cos(4)
                0.99619469809174553229501040247389,    //cos(5)
                0.99452189536827333692269194498057,    //cos(6)
                0.99254615164132203498006158933058,    //cos(7)
                0.99026806874157031508377486734485,    //cos(8)
                0.98768834059513772619004024769344      //cos(9)
        };
    }

    double sin(double target) {
        int sig = 0;

        if (target > 0.0) {
            while (target >= 360.0)
                target = target - 360.0;
        } else {
            while (target < 0.0)
                target = target + 360.0;
        }
        if (target >= 180.0) {
            sig = 1;
            target = target - 180.0;
        }
        target = (target > 90.0) ? (180.0 - target) : target;
        int a = target * 0.1;
        double b = target - 10 * a;
        target = Detail::sin_table[a] * Detail::cos_table[(int) b] + b * Detail::magic * Detail::sin_table[9 - a];
        return (sig > 0) ? -target : target;
    }

    double cos(double target) {
        return sin(target + 90);
    }

    double tan(double target) {
        return sin(target) / sin(target + 90);
    }

}

#endif //TINYSTL_ALGORITHMS6_HPP
