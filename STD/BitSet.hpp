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
        Int_8 value[(size_ + 7) / 8];

        inline void set_to(Size pos, bool target) {
            if (target) value[pos / 8] |= static_cast<Int_8>(1 << (7 - pos % 8));
            else value[pos / 8] &= static_cast<Int_8>(~(1 << (7 - pos % 8)));
        }

    public:
        BitSet() {
            reset();
        }

        template<typename Chart = char>
        explicit BitSet(const Chart *target, Size pos = 0, Size len = -1, Chart zero = '0', Chart one = '1') {
            if (len == -1) len = Get_Length(target + pos);
            if (len == 0) return;
            if (len < size_) {
                Size temp = (len - size_ + 7) / 8;
                for (Size i = 0; i < temp; ++i) value[i] = 0;
            }
            target += pos + len - 1;
            for (Size i = size_ > len ? size_ - len : 0; i < size_; ++i, ++target) {
                if (*target == zero) set_to(i, false);
                else if (*target == one) set_to(i, true);
                else throw errorType("You passed an error-type in BitSet\n");
            }
        };

        template<typename Chart = char>
        explicit BitSet(const String &target, Size pos = 0, Size len = -1, Chart zero = '0', Chart one = '1') :
                BitSet(target.c_str(), pos, len, zero, one) {};

        BitSet(Size target) {
            reset();
            Size len = size_;
            while (len) {
                if (!target) break;
                set_to(--len, target & 1);
                target >>= 1;
            }
        };

        constexpr Size size() const {
            return size_;
        };

        Size count() const {
            Size count = 0;
            for (auto i: value) {
                while (i) {
                    count += i & 1;
                    i >>= 1;
                }
            }
            return count;
        }

        bool all() const {
            Size count = size_ / 8;
            while (count)
                if (value[--count] != 0b11111111) return false;
            count = size_ / 8 * 8;
            while (count < size_)
                if (!get(count++)) return false;
            return true;
        }

        bool any() const {
            Size count = size_ / 8;
            while (count)
                if (value[--count] > 0b00000000) return true;
            count = size_ / 8 * 8;
            while (count < size_)
                if (get(count++)) return true;
            return false;
        }

        bool none() const {
            Size count = size_ / 8;
            while (count)
                if (value[--count] != 0b00000000) return false;
            count = size_ / 8 * 8;
            while (count < size_)
                if (get(count++)) return false;
            return true;
        }

        inline bool get(Size pos) const {
            return value[pos / 8] & (1 << (7 - pos % 8));
        }

        template<Size size>
        BitSet<size_> &operator&=(const BitSet<size> &other) {
            Size i = (size_ + 7) / 8;
            while (i) value[--i] &= other.value[i];
            return *this;
        }

        template<Size size>
        BitSet<size_> &operator|=(const BitSet<size> &other) {
            Size i = (size_ + 7) / 8;
            while (i) value[--i] |= other.value[i];
            return *this;
        }

        template<Size size>
        BitSet<size_> &operator^=(const BitSet<size> &other) {
            Size i = (size_ + 7) / 8;
            while (i) value[--i] ^= other.value[i];
            return *this;
        }

        BitSet<size_> operator~() const {
            Size size = (size_ + 7) / 8;
            BitSet<size_> temp;
            while (size) temp[--size] = ~value[size];
            return temp;
        }

        BitSet<size_> &set() {
            MemSet(value, 1, size_ / 32);
            switch (size_ % 4) {
                case 3:
                    value[size_ - 3] = 0b11111111;
                case 2:
                    value[size_ - 2] = 0b11111111;
                case 1:
                    value[size_ - 1] = 0b11111111;
                default:
                    break;
            }
            return *this;
        }

        BitSet<size_> &set(Size pos, Size len = 1, bool val = true) {
            while (len) set_to(pos + --len, val);
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
            Size size = (size_ + 7) / 8;
            while (size) value[--size] = ~value[size];
        }

        BitSet<size_> &flip(Size pos, Size len = 1) {
            while (len--) set_to(pos + len, !get(pos + len));
        }

        String to_String(char zero = '0', char one = '1') const {
            String temp(size_, zero);
            for (Size i = 0; i < size_; ++i)
                if (get(i)) temp[i] = one;
            return temp;
        }

        Int_64 to_Int_64() const {
            if (size_ > 64)
                throw overFLow("The BitSet cannot transfer to unsigned long long\n");
            Int_64 temp = 0, mask = 1;
            int pos = size_;
            for (int i = 0; i < size_; ++i)
                if (get(--pos)) temp |= (mask << i);
            return temp;
        }

        Int_32 to_Int_32() const {
            if (size_ > 32)
                throw overFLow("The BitSet cannot transfer to unsigned long long\n");
            Int_32 temp = 0, mask = 1;
            int pos = size_;
            for (int i = 0; i < size_; ++i)
                if (get(--pos)) temp |= (mask << i);
            return temp;
        }

        Int_16 to_Int_16() const {
            if (size_ > 16)
                throw overFLow("The BitSet cannot transfer to unsigned long long\n");
            Int_16 temp = 0, mask = 1;
            int pos = size_;
            for (int i = 0; i < size_; ++i)
                if (get(--pos)) temp |= (mask << i);
            return temp;
        }

        Int_8 to_Int_8() const {
            if (size_ > 16)
                throw overFLow("The BitSet cannot transfer to unsigned long long\n");
            Int_8 temp = 0, mask = 1;
            int pos = size_;
            for (int i = 0; i < size_; ++i)
                if (get(--pos)) temp |= (mask << i);
            return temp;
        }

        friend BitSet<size_>
        operator|(const BitSet<size_> &left, const BitSet<size_> &right) {
            BitSet<size_> temp(left);
            temp |= right;
            return temp;
        }

        friend BitSet<size_>
        operator&(const BitSet<size_> &left, const BitSet<size_> &right) {
            BitSet<size_> temp(left);
            temp &= right;
            return temp;
        }

        friend BitSet<size_>
        operator^(const BitSet<size_> &left, const BitSet<size_> &right) {
            BitSet<size_> temp(left);
            temp ^= right;
            return temp;
        }

        friend BitSet<size_> operator~(const BitSet<size_> &target) {
            BitSet<size_> temp(target);
            temp.flip();
            return temp;
        }

        friend bool operator==(const BitSet<size_> &left, const BitSet<size_> &right) {
            Size index = (size_ + 7) / 8;
            while (index)
                if (left[--index] != right[index]) return false;
            return true;
        }

        friend bool operator!=(const BitSet<size_> &left, const BitSet<size_> &right) {
            return !(left == right);
        }

    };

}

#endif //TINYSTL_BITSET_HPP
