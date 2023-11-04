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
        if ((flag = target < 0)) {
            if (target == MIN_SHORT) return {"-32768"};
            else target = -target;
        }
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
        if ((flag = target < 0)) {
            if (target == MIN_INT) return {"-2147483648"};
            else target = -target;
        }
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
        if ((flag = target < 0)) {
            if (target == MIN_LONG) return {"-2147483648"};
            else target = -target;
        }
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
        if ((flag = target < 0)) {
            if (target == MIN_LLONG) return {"-9223372036854775808"};
            else target = -target;
        }
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

    int Stos(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stos\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("-0123456789", pos, n - pos);
        if (pos == n)
            throw errorArgument("Error input in Stos\n");
        bool flag = false, judge = true;
        if (target[pos] == '-') flag = ++pos;
        int value = 0;
        constexpr short mark = MAX_SHORT / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            judge = false;
            if (value > mark || value == mark && (flag && target[pos] > '8' || !flag && target[pos] > '7'))
                throw overFLow("Input Value overFlow in Stos\n");
            value = value * 10 + (target[pos] - '0');
            ++pos;
        }
        if (judge) throw errorArgument("Error input in Stos\n");
        return flag ? -value : value;
    }

    unsigned short Stous(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stous\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("0123456789", pos, n - pos);
        if (pos == n || pos > 0 && target[pos - 1] == '-')
            throw errorArgument("Error input in Stous\n");
        unsigned short value = 0;
        constexpr unsigned short mark = MAX_UINT_SHORT / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            if (value > mark || value == mark && target[pos] > '5')
                throw overFLow("Input Value overFlow in Stous\n");
            value = value * 10 + (target[pos] - '0');
            ++pos;
        }
        return value;
    }

    int Stoi(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stoi\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("-0123456789", pos, n - pos);
        if (pos == n)
            throw errorArgument("Error input in Stoi\n");
        bool flag = false, judge = true;
        if (target[pos] == '-') flag = ++pos;
        int value = 0;
        constexpr int mark = MAX_INT / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            judge = false;
            if (value > mark || value == mark && (flag && target[pos] > '8' || !flag && target[pos] > '7'))
                throw overFLow("Input Value overFlow in Stoi\n");
            value = value * 10 + (target[pos] - '0');
            ++pos;
        }
        if (judge) throw errorArgument("Error input in Stoi\n");
        return flag ? -value : value;
    }

    unsigned Stou(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stou\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("0123456789", pos, n - pos);
        if (pos == n || pos > 0 && target[pos - 1] == '-')
            throw errorArgument("Error input in Stou\n");
        unsigned value = 0;
        constexpr unsigned mark = MAX_UINT_INT / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            if (value > mark || value == mark && target[pos] > '5')
                throw overFLow("Input Value overFlow in Stou\n");
            value = value * 10 + (target[pos] - '0');
            ++pos;
        }
        return value;
    }

    long Stol(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stol\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("-0123456789", pos, n - pos);
        if (n == target.size())
            throw errorArgument("Error input in Stol\n");
        bool flag = false, judge = true;
        if (target[pos] == '-') flag = ++pos;
        long value = 0;
        constexpr long mark = MAX_LONG / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            judge = false;
            if (value > mark || value == mark && (flag && target[pos] > '8' || !flag && target[pos] > '7'))
                throw overFLow("Input Value overFlow in Stol\n");
            value = value * 10 + (target[pos] - '0');
            ++pos;
        }
        if (judge) throw errorArgument("Error input in Stol\n");
        return flag ? -value : value;
    }

    unsigned long Stoul(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stoul\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("0123456789", pos, n - pos);
        if (pos == n || pos > 0 && target[pos - 1] == '-')
            throw errorArgument("Error input in Stoul\n");
        unsigned long value = 0;
        constexpr unsigned long mark = MAX_UINT_LONG / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            if (value > mark || value == mark && target[pos] > '5')
                throw overFLow("Input Value overFlow in Stoul\n");
            value = value * 10 + (target[pos] - '0');
            ++pos;
        }
        return value;
    }

    long long Stoll(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stoll\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("-0123456789", pos, n - pos);
        if (pos == n)
            throw errorArgument("Error input in Stoll\n");
        bool flag = false, judge = true;
        if (target[pos] == '-') flag = ++pos;
        long long value = 0;
        constexpr long long mark = MAX_LLONG / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            judge = false;
            if (value > mark || value == mark && (flag && target[pos] > '8' || !flag && target[pos] > '7'))
                throw overFLow("Input Value overFlow in Stoll\n");
            value = value * 10 + (target[pos] - '0');
            ++pos;
        }
        if (judge) throw errorArgument("Error input in Stoll\n");
        return flag ? -value : value;
    }

    unsigned long long Stoull(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stoull\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("0123456789", pos, n - pos);
        if (pos == n || pos > 0 && target[pos - 1] == '-')
            throw errorArgument("Error input in Stoull\n");
        unsigned long long value = 0;
        constexpr unsigned long long mark = MAX_UINT_LLONG / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            if (value > mark || value == mark && target[pos] > '5')
                throw overFLow("Input Value overFlow in Stoull\n");
            value = value * 10 + (target[pos] - '0');
            ++pos;
        }
        return value;
    }

    float Stof(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stof\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("-.0123456789", pos, n - pos);
        if (pos == n)
            throw errorArgument("Error input in Stof\n");
        bool flag = false, judge = true;
        if (target[pos] == '-') flag = ++pos;
        float value = 0;
        int count = 0;
        constexpr float mark1 = MAX_FLOAT / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            judge = false;
            if (value > mark1)
                throw overFLow("Input Value overFlow in Stof\n");
            value *= 10;
            if (count < 6) value += static_cast<float>(target[pos] - '0');
            ++pos, ++count;
        }
        if (pos < n && target[pos] == '.') {
            float temp = 0.1f;
            ++pos;
            while (pos < n && count < 6 && target[pos] >= '0' && target[pos] <= '9') {
                judge = false;
                value += temp * static_cast<float>(target[pos] - '0');
                temp *= 0.1f;
                ++pos, ++count;
            }
        }
        if (judge) throw errorArgument("Error input in Stod\n");
        return flag ? -value : value;
    }

    double Stod(const String &target, Size pos = 0, Size n = -1) {
        if (pos >= target.size())
            throw errorArgument("Error input in Stod\n");
        if (n == -1) n = target.size() - pos;
        n += pos;
        pos = target.find_first_of("-.0123456789", pos, n - pos);
        if (pos == n)
            throw errorArgument("Error input in Stod\n");
        bool flag = false, judge = true;
        if (target[pos] == '-') flag = ++pos;
        double value = 0;
        int count = 0;
        constexpr double mark1 = MAX_DOUBLE / 10;
        while (pos < n && target[pos] >= '0' && target[pos] <= '9') {
            judge = false;
            if (value > mark1)
                throw overFLow("Input Value overFlow in Stod\n");
            value *= 10;
            if (count < 16) value += target[pos] - '0';
            ++pos, ++count;
        }
        if (pos < n && target[pos] == '.') {
            double temp = 0.1;
            ++pos;
            while (pos < n && count < 16 && target[pos] >= '0' && target[pos] <= '9') {
                judge = false;
                value += temp * (target[pos] - '0');
                temp *= 0.1;
                ++pos, ++count;
            }
        }
        if (judge) throw errorArgument("Error input in Stod\n");
        return flag ? -value : value;
    }

}

#endif //TINYSTL_STRING_ALGORITHMS2_HPP
