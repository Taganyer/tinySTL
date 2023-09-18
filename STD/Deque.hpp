//
// Created by 86152 on 2023/9/18.
//

#ifndef TINYSTL_DEQUE_HPP
#define TINYSTL_DEQUE_HPP

#include "Iter.hpp"
#include "Move.hpp"
#include "Allocater.hpp"
#include "Memory.hpp"

namespace STD {

    const int MID = 64;

    const int BLOCK_SIZE = 128;

    template<typename Arg>
    class Deque {
    private:

        static constexpr Size the_size_of_map = BLOCK_SIZE * sizeof(Arg);

        Size size_ = 0, map_size, val_begin, val_end;

        Arg **map = nullptr;

        void map_insert(int size, bool to_end);

        void init();

        void fill(Size pos, const Arg &target, Size size);

        void fill(Size pos, const std::initializer_list<Arg> &list);

        void fill(Size pos, const cIter<Arg> &begin, Size size);

    public:

        class Iterator;

        class cIterator;

        class rIterator;

        class crIterator;

        Deque();

        Deque(Size size, const Arg &target = Arg());

        Deque(const std::initializer_list<Arg> &list);

        Deque(const Iter<Arg> &begin, const Iter<Arg> &end);

        Deque(const cIter<Arg> &begin, const cIter<Arg> &end);

        Deque(const Deque<Arg> &other);

        Deque(Deque<Arg> &&other) noexcept;

        ~Deque<Arg>();

        bool empty() const { return !size_; };

        Deque<Arg> &operator=(const Deque<Arg> &other);

        Deque<Arg> &operator=(Deque<Arg> &&other) noexcept;

        template<typename Type>
        friend bool operator==(const Deque<Arg> &left, const Deque<Arg> &right);

        template<typename Type>
        friend bool operator!=(const Deque<Arg> &left, const Deque<Arg> &right);

        template<typename Type>
        friend bool operator<(const Deque<Arg> &left, const Deque<Arg> &right);

        template<typename Type>
        friend bool operator<=(const Deque<Arg> &left, const Deque<Arg> &right);

        template<typename Type>
        friend bool operator>(const Deque<Arg> &left, const Deque<Arg> &right);

        template<typename Type>
        friend bool operator>=(const Deque<Arg> &left, const Deque<Arg> &right);

        Iterator begin() const {
            return Deque<Arg>::Iterator(val_begin, map, 0);
        };

        Iterator end() const {
            return Deque<Arg>::Iterator(*(map + map_size - 1) + val_end, map + map_size - 1, val_end);
        };

        cIterator cbegin() const {
            return Deque<Arg>::cIterator(val_begin, map, 0);
        };

        cIterator cend() const {
            return Deque<Arg>::cIterator(*(map + map_size - 1) + val_end, map + map_size - 1, val_end);
        };

        rIterator rbegin() const {
            auto temp1 = *(map + map_size - 1);
            auto  temp2 = val_end - 1;
            if (temp2 == -1) {
                temp2 = BLOCK_SIZE - 1;
                --temp1;
            }
            return Deque<Arg>::rIterator(*temp1 + temp2, temp1, temp2);
        };

        rIterator rend() const {
            return Deque<Arg>::rIterator(nullptr, map - 1, BLOCK_SIZE - 1);
        };

        crIterator crbegin() const {
            auto temp1 = *(map + map_size - 1);
            auto  temp2 = val_end - 1;
            if (temp2 == -1) {
                temp2 = BLOCK_SIZE - 1;
                --temp1;
            }
            return Deque<Arg>::crIterator(*temp1 + temp2, temp1, temp2);
        };

        crIterator crend() const {
            return Deque<Arg>::crIterator(nullptr, map - 1, BLOCK_SIZE - 1);
        }

    };

    template<typename Arg>
    Deque<Arg>::Deque() : size_(0) {
        init();
    }


    template<typename Arg>
    Deque<Arg>::Deque(Size size, const Arg &target) : size_(size) {
        init();
        fill(0, target, size);
    }


    template<typename Arg>
    Deque<Arg>::Deque(const std::initializer_list<Arg> &list) : size_(list.size()) {
        init();
        fill(0, list);
    }

    template<typename Arg>
    void Deque<Arg>::init() {
        map_size = (size_ + 7 + BLOCK_SIZE) / BLOCK_SIZE;
        map = Allocate_n<Arg *>(map_size);
        for (int i = 0; i < map_size; ++i) map[i] = Allocate_n<Arg>(BLOCK_SIZE);
        int rest = map_size * BLOCK_SIZE - size_;
        val_begin = rest / 2;
        val_end = (size_ + rest / 2) % BLOCK_SIZE;
    }

    template<typename Arg>
    void Deque<Arg>::fill(Size pos, const Arg &target, Size size) {
        int i = (pos + val_begin + BLOCK_SIZE - 1) / BLOCK_SIZE - 1, j = (pos + val_begin) % BLOCK_SIZE;
        for (int index = 0; index < size; ++index) {
            map[i][j] = target;
            if (++j == BLOCK_SIZE) {
                j = 0;
                ++i;
            }
        }
    }

    template<typename Arg>
    void Deque<Arg>::fill(Size pos, const std::initializer_list<Arg> &list) {
        int i = (pos + val_begin + BLOCK_SIZE - 1) / BLOCK_SIZE - 1, j = (pos + val_begin) % BLOCK_SIZE;
        auto temp = list.begin();
        for (int index = 0; index < list.size(); ++index) {
            map[i][j] = *temp;
            ++j, ++temp;
            if (j == BLOCK_SIZE) {
                j = 0;
                ++i;
            }
        }
    }

    template<typename Arg>
    void Deque<Arg>::fill(Size pos, const cIter<Arg> &begin, Size size) {
        int i = (pos + val_begin + BLOCK_SIZE - 1) / BLOCK_SIZE - 1, j = (pos + val_begin) % BLOCK_SIZE;
        auto temp = begin.deep_copy();
        for (int index = 0; index < size; ++index) {
            map[i][j] = **temp;
            ++j, ++(*temp);
            if (j == BLOCK_SIZE) {
                j = 0;
                ++i;
            }
        }
    }

    template<typename Arg>
    Deque<Arg>::Deque(const Iter<Arg> &begin, const Iter<Arg> &end) : Deque(*begin.to_const(), *end.to_const()) {}

    template<typename Arg>
    Deque<Arg>::Deque(const cIter<Arg> &begin, const cIter<Arg> &end) : size_(calculateLength(begin, end)) {
        init();
        fill(0, begin, size_);
    }

    template<typename Arg>
    Deque<Arg>::Deque(const Deque<Arg> &other) : size_(other.size_) {
        init();
        fill(0, other.cbegin(), size_);
    }

    template<typename Arg>
    Deque<Arg>::Deque(Deque<Arg> &&other) noexcept : size_(other.size_), map_size(other.map_size),
                                                     val_begin(other.val_begin),
                                                     val_end(other.val_end), map(other.map) {
        for (int i = 0; i < map_size; ++i) {
            Deallocate_n(map);
            ++map;
        }
        other.size_ = 0;
        other.map_size = 0;
        other.val_begin = MID;
        other.val_end = MID;
        other.map = Allocate_n<Arg *>(1);
    }

    template<typename Arg>
    Deque<Arg>::~Deque<Arg>() {
        for (int i = 0; i < map_size; ++i) {
            Deallocate_n(map);
            ++map;
        }
        Deallocate_n(map - map_size);
    }

    template<typename Arg>
    void Deque<Arg>::map_insert(int size, bool to_end) {
        int i = 0;
        auto the_new = Allocate_n<Arg *>(map_size + size);
        if (to_end) {
            for (; i < map_size; ++i) the_new[i] = map[i];
            for (; i < map_size; ++i) the_new[i] = Allocate_n<Arg>(the_size_of_map);
        } else {
            for (; i < map_size; ++i) the_new[i] = Allocate_n<Arg>(the_size_of_map);
            for (; i < map_size; ++i) the_new[i] = map[i];
        }
        Deallocate_n(map);
        map_size += size;
        map = the_new;
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Deque<Arg>::Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        Arg **map;

        short pos;

        explicit Iterator(Arg *ptr, Arg **map_ptr, short index) : Iter<Arg>(ptr), map(map_ptr), pos(index) {};

    public:
        friend class Deque<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Deque<Arg>::Iterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Deque<Arg>::cIterator>(Deque<Arg>::cIterator(target));
        };

        Iterator(const Iterator &other) : Iter<Arg>(other.target), map(other.map), pos(other.pos) {};

        ~Iterator() = default;

        Iterator &operator=(const Iterator &other) {
            target = other.target;
            map = other.map;
            pos = other.pos;
            return *this;
        };

        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        Iterator &operator++() override {
            if (++pos == BLOCK_SIZE) {
                pos = 0;
                ++map;
                target = *map;
            } else ++target;
            return *this;
        };

        Iterator operator++(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            if (++pos == BLOCK_SIZE) {
                pos = 0;
                ++map;
                target = *map;
            } else ++target;
            return Deque<Arg>::Iterator(temp1, temp2, temp3);
        };

        Iterator &operator--() {
            if (--pos < 0) {
                pos += BLOCK_SIZE;
                --map;
                target = *map + pos;
            } else --target;
            return *this;
        };

        Iterator operator--(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            if (--pos < 0) {
                pos += BLOCK_SIZE;
                --map;
                target = *map + pos;
            } else --target;
            return Deque<Arg>::Iterator(temp1, temp2, temp3);
        };

        Iterator operator+(Size size) const {
            auto temp2 = map + (pos + size) / BLOCK_SIZE;
            auto temp3 = (pos + size) % BLOCK_SIZE;
            auto temp1 = *temp2 + temp3;
            return Deque<Arg>::Iterator(temp1, temp2, temp3);
        };

        Iterator operator-(Size size) const {
            if (size <= pos + 1) {
                return Deque<Arg>::Iterator(target - size, map, pos - size);
            } else {
                auto temp2 = map - (size - pos - 1) / BLOCK_SIZE;
                auto temp3 = BLOCK_SIZE - (size - pos - 1) % BLOCK_SIZE;
                auto temp1 = *temp2 + temp3;
                if (temp3 == BLOCK_SIZE) temp3 = 0;
                return Deque<Arg>::Iterator(temp1, temp2, temp3);
            }
        };

        Iterator &operator+=(Size size) {
            map += (pos + size) / BLOCK_SIZE;
            pos = (pos + size) % BLOCK_SIZE;
            target = *map + pos;
            return *this;
        };

        Iterator &operator-=(Size size) {
            if (size <= pos + 1) {
                pos -= size;
                target -= size;
            } else {
                map -= (size - pos - 1) / BLOCK_SIZE;
                pos = BLOCK_SIZE - (size - pos - 1) % BLOCK_SIZE;
                if (pos == BLOCK_SIZE) pos = 0;
                target = *map + pos;
            }
            return *this;
        };

        friend bool
        operator==(const Iterator &left, const Iterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const Iterator &left, const Iterator &right) { return left.target != right.target; };

        friend bool
        operator>(const Iterator &left, const Iterator &right) {
            if (left.map != right.map) return left.map > right.map;
            return left.pos > right.pos;
        };

        friend bool
        operator<(const Iterator &left, const Iterator &right) {
            if (left.map != right.map) return left.map < right.map;
            return left.pos < right.pos;
        };

        friend bool
        operator>=(const Iterator &left, const Iterator &right) { return left.target >= right.target; };

        friend bool
        operator<=(const Iterator &left, const Iterator &right) { return left.target <= right.target; };

        friend long long
        operator-(const Iterator &left, const Iterator &right) {
            return (left.map - right.map) * BLOCK_SIZE + left.pos - right.pos;
        };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Deque<Arg>::cIterator : public cIter<Arg> {
    protected:
        using cIter<Arg>::target;

        Arg **map;

        short pos;

        explicit cIterator(Arg *ptr, Arg **map_ptr, short index) : cIter<Arg>(ptr), map(map_ptr), pos(index) {};

    public:
        friend class Deque<Arg>;

        friend class Deque<Arg>::Iterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Deque<Arg>::cIterator>(*this); };

        cIterator(const cIterator &other) : cIter<Arg>(other.target), map(other.map), pos(other.pos) {};

        ~cIterator() = default;

        cIterator &operator=(const cIterator &other) {
            target = other.target;
            map = other.map;
            pos = other.pos;
            return *this;
        };

        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        cIterator &operator++() override {
            if (++pos == BLOCK_SIZE) {
                pos = 0;
                ++map;
                target = *map;
            } else ++target;
            return *this;
        };

        cIterator operator++(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            if (++pos == BLOCK_SIZE) {
                pos = 0;
                ++map;
                target = *map;
            } else ++target;
            return Deque<Arg>::cIterator(temp1, temp2, temp3);
        };

        cIterator &operator--() {
            if (--pos < 0) {
                pos += BLOCK_SIZE;
                --map;
                target = *map + pos;
            } else --target;
            return *this;
        };

        cIterator operator--(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            if (--pos < 0) {
                pos += BLOCK_SIZE;
                --map;
                target = *map + pos;
            } else --target;
            return Deque<Arg>::cIterator(temp1, temp2, temp3);
        };

        cIterator operator+(Size size) const {
            auto temp2 = map + (pos + size) / BLOCK_SIZE;
            auto temp3 = (pos + size) % BLOCK_SIZE;
            auto temp1 = *temp2 + temp3;
            return Deque<Arg>::cIterator(temp1, temp2, temp3);
        };

        cIterator operator-(Size size) const {
            if (size <= pos + 1) {
                return Deque<Arg>::cIterator(target - size, map, pos - size);
            } else {
                auto temp2 = map - (size - pos - 1) / BLOCK_SIZE;
                auto temp3 = BLOCK_SIZE - (size - pos - 1) % BLOCK_SIZE;
                auto temp1 = *temp2 + temp3;
                if (temp3 == BLOCK_SIZE) temp3 = 0;
                return Deque<Arg>::cIterator(temp1, temp2, temp3);
            }
        };

        cIterator &operator+=(Size size) {
            map += (pos + size) / BLOCK_SIZE;
            pos = (pos + size) % BLOCK_SIZE;
            target = *map + pos;
            return *this;
        };

        cIterator &operator-=(Size size) {
            if (size <= pos + 1) {
                pos -= size;
                target -= size;
            } else {
                map -= (size - pos - 1) / BLOCK_SIZE;
                pos = BLOCK_SIZE - (size - pos - 1) % BLOCK_SIZE;
                if (pos == BLOCK_SIZE) pos = 0;
                target = *map + pos;
            }
            return *this;
        };

        friend bool
        operator==(const cIterator &left, const cIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const cIterator &left, const cIterator &right) { return left.target != right.target; };

        friend bool
        operator>(const cIterator &left, const cIterator &right) {
            if (left.map != right.map) return left.map > right.map;
            return left.pos > right.pos;
        };

        friend bool
        operator<(const cIterator &left, const cIterator &right) {
            if (left.map != right.map) return left.map < right.map;
            return left.pos < right.pos;
        };

        friend bool
        operator>=(const cIterator &left, const cIterator &right) { return left.target >= right.target; };

        friend bool
        operator<=(const cIterator &left, const cIterator &right) { return left.target <= right.target; };

        friend long long
        operator-(const cIterator &left, const cIterator &right) {
            return (left.map - right.map) * BLOCK_SIZE + left.pos - right.pos;
        };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Deque<Arg>::rIterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        Arg **map;

        short pos;

        explicit rIterator(Arg *ptr, Arg **map_ptr, short index) : Iter<Arg>(ptr), map(map_ptr), pos(index) {};

    public:
        friend class Deque<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Deque<Arg>::rIterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Deque<Arg>::crIterator>(Deque<Arg>::crIterator(target));
        };

        rIterator(const Iterator &other) : Iter<Arg>(other.target), map(other.map), pos(other.pos) {};

        ~rIterator() = default;

        rIterator &operator=(const rIterator &other) {
            target = other.target;
            map = other.map;
            pos = other.pos;
            return *this;
        };

        using Iter<Arg>::operator*;

        using Iter<Arg>::operator->;

        rIterator &operator--() {
            if (++pos == BLOCK_SIZE) {
                pos = 0;
                ++map;
                target = *map;
            } else ++target;
            return *this;
        };

        rIterator operator--(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            if (++pos == BLOCK_SIZE) {
                pos = 0;
                ++map;
                target = *map;
            } else ++target;
            return Deque<Arg>::rIterator(temp1, temp2, temp3);
        };

        rIterator &operator++() override {
            if (--pos < 0) {
                pos += BLOCK_SIZE;
                --map;
                target = *map + pos;
            } else --target;
            return *this;
        };

        rIterator operator++(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            if (--pos < 0) {
                pos += BLOCK_SIZE;
                --map;
                target = *map + pos;
            } else --target;
            return Deque<Arg>::rIterator(temp1, temp2, temp3);
        };

        rIterator operator-(Size size) const {
            auto temp2 = map + (pos + size) / BLOCK_SIZE;
            auto temp3 = (pos + size) % BLOCK_SIZE;
            auto temp1 = *temp2 + temp3;
            return Deque<Arg>::Iterator(temp1, temp2, temp3);
        };

        rIterator operator+(Size size) const {
            if (size <= pos + 1) {
                return Deque<Arg>::rIterator(target - size, map, pos - size);
            } else {
                auto temp2 = map - (size - pos - 1) / BLOCK_SIZE;
                auto temp3 = BLOCK_SIZE - (size - pos - 1) % BLOCK_SIZE;
                auto temp1 = *temp2 + temp3;
                if (temp3 == BLOCK_SIZE) temp3 = 0;
                return Deque<Arg>::rIterator(temp1, temp2, temp3);
            }
        };

        rIterator &operator-=(Size size) {
            map += (pos + size) / BLOCK_SIZE;
            pos = (pos + size) % BLOCK_SIZE;
            target = *map + pos;
            return *this;
        };

        rIterator &operator+=(Size size) {
            if (size <= pos + 1) {
                pos -= size;
                target -= size;
            } else {
                map -= (size - pos - 1) / BLOCK_SIZE;
                pos = BLOCK_SIZE - (size - pos - 1) % BLOCK_SIZE;
                if (pos == BLOCK_SIZE) pos = 0;
                target = *map + pos;
            }
            return *this;
        };

        friend bool
        operator==(const rIterator &left, const rIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const rIterator &left, const rIterator &right) { return left.target != right.target; };

        friend bool
        operator<(const rIterator &left, const rIterator &right) {
            if (left.map != right.map) return left.map > right.map;
            return left.pos > right.pos;
        };

        friend bool
        operator>(const rIterator &left, const rIterator &right) {
            if (left.map != right.map) return left.map < right.map;
            return left.pos < right.pos;
        };

        friend bool
        operator<=(const rIterator &left, const rIterator &right) { return left.target >= right.target; };

        friend bool
        operator>=(const rIterator &left, const rIterator &right) { return left.target <= right.target; };

        friend long long
        operator-(const rIterator &left, const rIterator &right) {
            return (right.map - left.map) * BLOCK_SIZE + right.pos - left.pos;
        };
    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Deque<Arg>::crIterator : public cIter<Arg> {
    protected:
        using cIter<Arg>::target;

        Arg **map;

        short pos;

        explicit crIterator(Arg *ptr, Arg **map_ptr, short index) : cIter<Arg>(ptr), map(map_ptr), pos(index) {};

    public:
        friend class Deque<Arg>;

        friend class Deque<Arg>::rIterator;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Deque<Arg>::crIterator>(*this); };

        crIterator(const cIterator &other) : cIter<Arg>(other.target), map(other.map), pos(other.pos) {};

        ~crIterator() = default;

        crIterator &operator=(const crIterator &other) {
            target = other.target;
            map = other.map;
            pos = other.pos;
            return *this;
        };

        using cIter<Arg>::operator*;

        using cIter<Arg>::operator->;

        crIterator &operator--() {
            if (++pos == BLOCK_SIZE) {
                pos = 0;
                ++map;
                target = *map;
            } else ++target;
            return *this;
        };

        crIterator operator--(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            if (++pos == BLOCK_SIZE) {
                pos = 0;
                ++map;
                target = *map;
            } else ++target;
            return Deque<Arg>::crIterator(temp1, temp2, temp3);
        };

        crIterator &operator++() override {
            if (--pos < 0) {
                pos += BLOCK_SIZE;
                --map;
                target = *map + pos;
            } else --target;
            return *this;
        };

        crIterator operator++(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            if (--pos < 0) {
                pos += BLOCK_SIZE;
                --map;
                target = *map + pos;
            } else --target;
            return Deque<Arg>::crIterator(temp1, temp2, temp3);
        };

        crIterator operator-(Size size) const {
            auto temp2 = map + (pos + size) / BLOCK_SIZE;
            auto temp3 = (pos + size) % BLOCK_SIZE;
            auto temp1 = *temp2 + temp3;
            return Deque<Arg>::cIterator(temp1, temp2, temp3);
        };

        crIterator operator+(Size size) const {
            if (size <= pos + 1) {
                return Deque<Arg>::crIterator(target - size, map, pos - size);
            } else {
                auto temp2 = map - (size - pos - 1) / BLOCK_SIZE;
                auto temp3 = BLOCK_SIZE - (size - pos - 1) % BLOCK_SIZE;
                auto temp1 = *temp2 + temp3;
                if (temp3 == BLOCK_SIZE) temp3 = 0;
                return Deque<Arg>::crIterator(temp1, temp2, temp3);
            }
        };

        crIterator &operator-=(Size size) {
            map += (pos + size) / BLOCK_SIZE;
            pos = (pos + size) % BLOCK_SIZE;
            target = *map + pos;
            return *this;
        };

        crIterator &operator+=(Size size) {
            if (size <= pos + 1) {
                pos -= size;
                target -= size;
            } else {
                map -= (size - pos - 1) / BLOCK_SIZE;
                pos = BLOCK_SIZE - (size - pos - 1) % BLOCK_SIZE;
                if (pos == BLOCK_SIZE) pos = 0;
                target = *map + pos;
            }
            return *this;
        };

        friend bool
        operator==(const crIterator &left, const crIterator &right) { return left.target == right.target; };

        friend bool
        operator!=(const crIterator &left, const crIterator &right) { return left.target != right.target; };

        friend bool
        operator<(const crIterator &left, const crIterator &right) {
            if (left.map != right.map) return left.map > right.map;
            return left.pos > right.pos;
        };

        friend bool
        operator>(const crIterator &left, const crIterator &right) {
            if (left.map != right.map) return left.map < right.map;
            return left.pos < right.pos;
        };

        friend bool
        operator<=(const crIterator &left, const crIterator &right) { return left.target >= right.target; };

        friend bool
        operator>=(const crIterator &left, const crIterator &right) { return left.target <= right.target; };

        friend long long
        operator-(const crIterator &left, const crIterator &right) {
            return (right.map - left.map) * BLOCK_SIZE + right.pos - left.pos;
        };
    };
}

#endif //TINYSTL_DEQUE_HPP
