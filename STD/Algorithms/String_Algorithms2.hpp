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
            store[i++] = static_cast<char>(target % 10 + 48);
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
            store[i++] = static_cast<char>(target % 10 + 48);
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
            store[i++] = static_cast<char>(target % 10 + 48);
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
        char store[19];
        int i = 0;
        bool flag;
        if ((flag = target < 0)) target = -target;
        while (target) {
            store[i++] = static_cast<char>(target % 10 + 48);
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
            store[i++] = static_cast<char>(target % 10 + 48);
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
            store[i++] = static_cast<char>(target % 10 + 48);
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
            store[i++] = static_cast<char>(target % 10 + 48);
            target /= 10;
        }
        if (!i) ++i;
        String temp(i, '0');
        int index = 0;
        while (i) temp[index++] = store[--i];
        return temp;
    }

    String To_String(unsigned long long target) {
        char store[20] = {'0'};
        int i = 0;
        while (target) {
            store[i++] = static_cast<char>(target % 10 + 48);
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
        int count = 0, record;
        if (target < 0) {
            temp.push_back('-');
            target = -target;
        }
        while (target > MAX_LLONG) {
            target /= 10;
            ++count;
        }
        while (target > 1e6) {
            target /= 10;
            ++count;
        }
        temp += To_String(static_cast<UInt_64>(target));
        temp.push_back(count, '0');
        count = 0;
        record = temp.size();
        if (temp[0] == '-') --record;
        target -= static_cast<UInt_64>(target);
        bool flag = false;
        while (target != static_cast<UInt_64>(target)) {
            if (record >= 6) {
                target = static_cast<UInt_64>(target);
                break;
            }
            target *= 10;
            flag = true;
            if (target < 1) ++count;
            ++record;
        }
        if (flag) {
            if (target == 0) return temp;
            temp.push_back('.');
            temp.push_back(count, '0');
            temp += To_String(static_cast<UInt_64>(target));
        }
        return temp;
    }

    String To_String(double target) {
        String temp;
        int count = 0, record;
        if (target < 0) {
            temp.push_back('-');
            target = -target;
        }
        while (target > MAX_LLONG) {
            target /= 10;
            ++count;
        }
        while (target > 1e16) {
            target /= 10;
            ++count;
        }
        temp += To_String(static_cast<UInt_64>(target));
        temp.push_back(count, '0');
        count = 0;
        record = temp.size();
        if (temp[0] == '-') --record;
        target -= static_cast<UInt_64>(target);
        bool flag = false;
        while (target != static_cast<UInt_64>(target)) {
            if (record >= 16) {
                target = static_cast<UInt_64>(target);
                break;
            }
            target *= 10;
            flag = true;
            if (target < 1) ++count;
            ++record;
        }
        if (flag) {
            if (target == 0) return temp;
            temp.push_back('.');
            temp.push_back(count, '0');
            temp += To_String(static_cast<UInt_64>(target));
        }
        return temp;
    }

}

#endif //TINYSTL_STRING_ALGORITHMS2_HPP
