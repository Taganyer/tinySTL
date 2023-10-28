//
// Created by 86152 on 2023/10/23.
//

#ifndef TINYSTL_STRING_ALGORITHMS2_HPP
#define TINYSTL_STRING_ALGORITHMS2_HPP

#include "../String.hpp"

namespace STD {

    String To_String(short target) {
        char store[10] = {'0'};
        int i = 0;
        bool flag;
        if ((flag = target < 0)) target = -target;
        while (target) {
            store[i++] = static_cast<char>(target % 10);
            target /= 10;
        }
        if (!i || flag) ++i;
        String temp(i, '0');
        int index = 0;
        if (flag) {
            temp[index++] = '-';
            --i;
        }
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(int target) {
        char store[10] = {'0'};
        int i = 0;
        bool flag;
        if ((flag = target < 0)) target = -target;
        while (target) {
            store[i++] = static_cast<char>(target % 10);
            target /= 10;
        }
        if (!i || flag) ++i;
        String temp(i, '0');
        int index = 0;
        if (flag) {
            temp[index++] = '-';
            --i;
        }
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(long target) {
        char store[10] = {'0'};
        int i = 0;
        bool flag;
        if ((flag = target < 0)) target = -target;
        while (target) {
            store[i++] = static_cast<char>(target % 10);
            target /= 10;
        }
        if (!i || flag) ++i;
        String temp(i, '0');
        int index = 0;
        if (flag) {
            temp[index++] = '-';
            --i;
        }
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(long long target) {
        char store[10];
        int i = 0;
        bool flag;
        if ((flag = target < 0)) target = -target;
        while (target) {
            store[i++] = static_cast<char>(target % 10);
            target /= 10;
        }
        if (!i || flag) ++i;
        String temp(i, '0');
        int index = 0;
        if (flag) {
            temp[index++] = '-';
            --i;
        } else if (i == 1) --i;
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(unsigned short target) {
        char store[10] = {'0'};
        int i = 0;
        while (target) {
            store[i++] = static_cast<char>(target % 10);
            target /= 10;
        }
        if (!i) ++i;
        String temp(i, '0');
        int index = 0;
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(unsigned target) {
        char store[10] = {'0'};
        int i = 0;
        while (target) {
            store[i++] = static_cast<char>(target % 10);
            target /= 10;
        }
        if (!i) ++i;
        String temp(i, '0');
        int index = 0;
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(unsigned long target) {
        char store[10] = {'0'};
        int i = 0;
        while (target) {
            store[i++] = static_cast<char>(target % 10);
            target /= 10;
        }
        if (!i) ++i;
        String temp(i, '0');
        int index = 0;
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(unsigned long long target) {
        char store[10] = {'0'};
        int i = 0;
        while (target) {
            store[i++] = static_cast<char>(target % 10);
            target /= 10;
        }
        if (!i) ++i;
        String temp(i, '0');
        int index = 0;
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(float target) {
        String temp;
        auto ptr = reinterpret_cast<const bool *>(&target);
        if (*ptr) temp.append('-');
        int exponent = 0;
        for (int i = 0, j = 8; j > 0; ++i, --j)
            exponent |= ptr[j] << i;
        ptr += 9;
        exponent -= 127;
        return temp;
    }

}

#endif //TINYSTL_STRING_ALGORITHMS2_HPP
