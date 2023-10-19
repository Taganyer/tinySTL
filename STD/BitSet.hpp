//
// Created by 86152 on 2023/10/16.
//

#ifndef TINYSTL_BITSET_HPP
#define TINYSTL_BITSET_HPP

#include "String.hpp"

namespace STD {

    template<Size size_>
    class BitSet {
    private:
        bool value[size_];

    public:
        BitSet() {
            reset();
        }

        template<typename Chart = char>
        explicit BitSet(const char *target, Size pos = 0, Size len = -1, Chart zero = '0', Chart one = '1') {
            if (len == -1) len = Get_Length(target + pos);
            Size size = len > size_ ? size_ : len;
            for (Size i = 0; i < size_ - size; ++i) value[i] = false;
            for (Size i = pos + len, j = size_ - size; j < size && i; ++j) {
                if (target[--i] == zero) value[j] = false;
                else if (target[i] == one) value[j] = true;
                else throw errorType("You passed an error-type in BitSet\n");
            }
        };

        template<typename Chart = char>
        explicit BitSet(const String &target, Size pos = 0, Size len = -1, Chart zero = '0', Chart one = '1') :
                BitSet(target.c_str(), pos, len, zero, one) {};

        explicit BitSet(Size target) {
            reset();
            Size len = size_;
            while (len) {
                if (!target) break;
                value[--len] = target & 1;
                target >>= 1;
            }
        };

        constexpr Size size() const {
            return size_;
        };

        Size count() const {
            Size count = 0;
            for (const auto &i: value) count += i;
            return count;
        }

        bool all() const {
            for (const auto &i: value) if (!i) return false;
            return true;
        }

        bool any() const {
            for (const auto &i: value) if (i) return true;
            return false;
        }

        bool none() const {
            for (const auto &i: value) if (i) return false;
            return true;
        }

        bool operator[](Size size) const {
            return value[size];
        }

        template<Size size>
        BitSet<size_> &operator&=(const BitSet<size> &other) {
            Size i = size_, j = size;
            while (i && j) {
                --i, --j;
                if (value[i] && other.value[j]) value[i] = true;
                else value[i] = false;
            }
            return *this;
        }

        template<Size size>
        BitSet<size_> &operator|=(const BitSet<size> &other) {
            Size i = size_, j = size;
            while (i && j) {
                --i, --j;
                if (value[i] || other.value[j]) value[i] = true;
                else value[i] = false;
            }
            return *this;
        }

        template<Size size>
        BitSet<size_> &operator^=(const BitSet<size> &other) {
            Size i = size_, j = size;
            while (i && j) {
                --i, --j;
                if (value[i] != other.value[j]) value[i] = true;
                else value[i] = false;
            }
            return *this;
        }

        BitSet<size_> operator~() const {
            Size size = size_;
            BitSet<size_> temp;
            while (size) temp[--size] = !value[size];
            return temp;
        }

        BitSet<size_> &set() {
            MemSet(value, 1, size_ / 4);
            switch (size_ % 4) {
                case 3:
                    value[size_ - 3] = 1;
                case 2:
                    value[size_ - 2] = 1;
                case 1:
                    value[size_ - 1] = 1;
                default:
                    break;
            }
            return *this;
        }

        BitSet<size_> &set(Size pos, Size len = 1, bool val = true) {
            while (len) value[pos + --len] = val;
            return *this;
        }

        BitSet<size_> &reset() {
            MemSet(value, 0, size_ / 4);
            switch (size_ % 4) {
                case 3:
                    value[size_ - 3] = 0;
                case 2:
                    value[size_ - 2] = 0;
                case 1:
                    value[size_ - 1] = 0;
                default:
                    break;
            }
            return *this;
        }

        BitSet<size_> &reset(Size pos, Size len = 1, bool val = false) {
            return set(pos, len, val);
        }

        BitSet<size_> &flip() {
            Size size = size_;
            while (size) value[--size] = !value[size];
        }

        BitSet<size_> &flip(Size pos, Size len = 1) {
            while (len) value[pos + --len] = !value[pos + len];
        }

        String to_String(char zero = '0', char one = '1') const {
            String temp(size_, zero);
            for (Size i = 0; i < size_; ++i)
                if (value[i]) temp[i] = one;
            return temp;
        }

        Size to_ullong() const {
            if (size_ > 64)
                throw overFLow("The BitSet cannot transfer to unsigned long long\n");
            Size temp = 0;
            int pos = size_;
            for (int i = 0; i < size_; ++i)
                if (value[--pos]) temp |= (1 << i);
            return temp;
        }

        friend BitSet<size_>
        operator|=(const BitSet<size_> &left, const BitSet<size_> &right) {
            BitSet<size_> temp(left);
            temp |= right;
            return temp;
        }

        friend BitSet<size_>
        operator&=(const BitSet<size_> &left, const BitSet<size_> &right) {
            BitSet<size_> temp(left);
            temp &= right;
            return temp;
        }

        friend BitSet<size_>
        operator^=(const BitSet<size_> &left, const BitSet<size_> &right) {
            BitSet<size_> temp(left);
            temp ^= right;
            return temp;
        }

        friend bool operator==(const BitSet<size_> &left, const BitSet<size_> &right) {
            Size index = size_;
            while (size_) {
                if (left[--index] != right[index]) return false;
            }
            return true;
        }

        friend bool operator!=(const BitSet<size_> &left, const BitSet<size_> &right) {
            return !(left == right);
        }

    };

}

#endif //TINYSTL_BITSET_HPP
