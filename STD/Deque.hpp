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

    const int BLOCK_SIZE = 128;

    template<typename Arg>
    class Deque {
    private:

        Size size_ = 0;

        int val_begin, val_end, map_begin, map_end, map_size;

        Arg **map = nullptr;

        inline static void add(int &i, int &j) {
            if (++j == BLOCK_SIZE) {
                j = 0;
                ++i;
            }
        };

        inline static void subtract(int &i, int &j) {
            if (--j < 0) {
                j = BLOCK_SIZE - 1;
                --i;
            }
        }

        inline static void add(Arg **&i, int &j, Arg *&ptr) {
            if (++j == BLOCK_SIZE) {
                j = 0;
                ++i;
                ptr = *i;
            } else ++ptr;
        };

        inline static void subtract(Arg **&i, int &j, Arg *&ptr) {
            if (--j < 0) {
                j = BLOCK_SIZE - 1;
                --i;
                ptr = *i + j;
            } else --ptr;
        }

        inline static void add(int &i, int &j, const Size &size) {
            if (j + size < BLOCK_SIZE) j += static_cast<int>(size);
            else {
                i += static_cast<int>((j + size) / BLOCK_SIZE);
                j = static_cast<int>((j + size) % BLOCK_SIZE);
            }
        };

        inline static void subtract(int &i, int &j, const Size &size) {
            if (size <= j) j -= static_cast<int>(size);
            else {
                i -= static_cast<int>((size - j + BLOCK_SIZE - 1) / BLOCK_SIZE);
                j = BLOCK_SIZE - static_cast<int>((size - j) % BLOCK_SIZE);
                if (j == BLOCK_SIZE) j = 0;
            }
        }

        inline static void add(Arg **&i, int &j, Arg *&ptr, const Size &size) {
            if (j + size < BLOCK_SIZE) {
                j += static_cast<int>(size);
                ptr += size;
            } else {
                i += (j + size) / BLOCK_SIZE;
                j = static_cast<int>((j + size) % BLOCK_SIZE);
                ptr = *i + j;
            }
        };

        inline static void subtract(Arg **&i, int &j, Arg *&ptr, const Size &size) {
            if (size <= j) {
                j -= static_cast<int>(size);
                ptr -= size;
            } else {
                i -= (size - j + BLOCK_SIZE - 1) / BLOCK_SIZE;
                j = BLOCK_SIZE - static_cast<int>((size - j) % BLOCK_SIZE);
                if (j == BLOCK_SIZE) j = 0;
                ptr = *i + j;
            }
        }

        inline long long find_pos(Arg **target_map, int pos) {
            return (target_map - map - map_begin) * BLOCK_SIZE + pos - val_begin;
        };

        inline Pair<Arg **, int> find_ptr(const Size &pos) const {
            auto target_map = (pos + val_begin) / BLOCK_SIZE + map + map_begin;
            auto target_pos = static_cast<int>((pos + val_begin) % BLOCK_SIZE);
            return {target_map, target_pos};
        };

        Size left_rest() const { return map_begin * BLOCK_SIZE + val_begin; };

        Size right_rest() const { return (map_size - map_end) * BLOCK_SIZE - val_end; };

        void init();

        void fill(Size pos, const Arg &target, Size size, void (*fun)(int &, int &));

        void fill(Size pos, const std::initializer_list<Arg> &list, void (*fun)(int &, int &));

        void fill(Size pos, const cIter<Arg> &begin, Size size, void (*fun)(int &, int &));

        void forward_move_element(Size step, Size size);

        void backward_move_element(Size step, Size size);

        void expand_capacity(int size, bool to_end);

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

        void clear();

        void shirk_to_fit();

        Deque<Arg> &assign(Size size, const Arg &target);

        Deque<Arg> &assign(const std::initializer_list<Arg> &list);

        Deque<Arg> &assign(const Iter<Arg> &begin, const Iter<Arg> &end);

        Deque<Arg> &assign(const cIter<Arg> &begin, const cIter<Arg> &end);

        template<typename ...args>
        void emplace_front(args &&...vals);

        void push_front(const Arg &val);

        void push_front(Size size, const Arg &val);

        void push_front(const Iter<Arg> &begin, const Iter<Arg> &end);

        void push_front(const cIter<Arg> &begin, const cIter<Arg> &end);

        void pop_front();

        template<typename ...args>
        void emplace_back(args &&...vals);

        void push_back(const Arg &val);

        void push_back(Size size, const Arg &val);;

        void push_back(const Iter<Arg> &begin, const Iter<Arg> &end);

        void push_back(const cIter<Arg> &begin, const cIter<Arg> &end);

        void pop_back();

        template<typename ...args>
        Iterator emplace(Size pos, args &&...vals);

        template<typename ...args>
        Iterator emplace(const Iterator &pos, args &&...vals);

        template<typename ...args>
        cIterator emplace(const cIterator &pos, args &&...vals);

        template<typename ...args>
        rIterator emplace(const rIterator &pos, args &&...vals);

        template<typename ...args>
        crIterator emplace(const crIterator &pos, args &&...vals);

        Iterator insert(Size pos, const Arg &val, Size size = 1);

        Iterator insert(Size pos, const std::initializer_list<Arg> &list);

        Iterator insert(Size pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert(Size pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator insert(const Iterator &pos, Size size, const Arg &val);

        Iterator insert(const Iterator &pos, const std::initializer_list<Arg> &list);

        Iterator insert(const Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        Iterator insert(const Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        cIterator insert(const cIterator &pos, Size size, const Arg &val);

        cIterator insert(const cIterator &pos, const std::initializer_list<Arg> &list);

        cIterator insert(const cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        cIterator insert(const cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        rIterator insert(const rIterator &pos, Size size, const Arg &val);

        rIterator insert(const rIterator &pos, const std::initializer_list<Arg> &list);

        rIterator insert(const rIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        rIterator insert(const rIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        crIterator insert(const crIterator &pos, Size size, const Arg &val);

        crIterator insert(const crIterator &pos, const std::initializer_list<Arg> &list);

        crIterator insert(const crIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end);

        crIterator insert(const crIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end);

        Iterator erase(Size pos, Size size = 1);

        Iterator erase(const Iterator &iter);

        cIterator erase(const cIterator &iter);

        Iterator erase(const Iterator &begin, const Iterator &end);

        cIterator erase(const cIterator &begin, const cIterator &end);

        rIterator erase(const rIterator &iter);

        crIterator erase(const crIterator &iter);

        rIterator erase(const rIterator &begin, const rIterator &end);

        crIterator erase(const crIterator &begin, const crIterator &end);

        Arg &operator[](Size pos) const {
            auto temp = find_ptr(pos);
            return temp.first[0][temp.second];
        };

        Arg &at(Size pos) const {
            if (pos >= size_) throw outOfRange("You provided an out-of-range subscript int the 'Deque::at' function");
            auto temp = find_ptr(pos);
            return temp.first[0][temp.second];
        };

        Size size() const { return size_; };

        Size capacity() const { return BLOCK_SIZE * map_size; };

        Arg &front() const {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'Deque::front' function");
            return map[map_begin][val_begin];
        };

        Arg &back() const {
            if (!size_) throw outOfRange("You're accessing a non-existent element in the 'Deque::back' function");
            return map[val_end ? map_end : map_end - 1][val_end ? val_end - 1 : BLOCK_SIZE - 1];
        };

        bool empty() const { return !size_; };

        Deque<Arg> &operator=(const Deque<Arg> &other);

        Deque<Arg> &operator=(Deque<Arg> &&other) noexcept;

        template<typename Type>
        friend bool operator==(const Deque<Type> &left, const Deque<Type> &right);

        template<typename Type>
        friend bool operator!=(const Deque<Type> &left, const Deque<Type> &right);

        template<typename Type>
        friend bool operator<(const Deque<Type> &left, const Deque<Type> &right);

        template<typename Type>
        friend bool operator<=(const Deque<Type> &left, const Deque<Type> &right);

        template<typename Type>
        friend bool operator>(const Deque<Type> &left, const Deque<Type> &right);

        template<typename Type>
        friend bool operator>=(const Deque<Type> &left, const Deque<Type> &right);

        Iterator begin() const {
            return Deque<Arg>::Iterator(*(map + map_begin) + val_begin, map + map_begin, val_begin);
        };

        Iterator end() const {
            return Deque<Arg>::Iterator(*(map + map_end) + val_end, map + map_end, val_end);
        };

        cIterator cbegin() const {
            return Deque<Arg>::cIterator(*(map + map_begin) + val_begin, map + map_begin, val_begin);
        };

        cIterator cend() const {
            return Deque<Arg>::cIterator(*(map + map_end) + val_end, map + map_end, val_end);
        };

        rIterator rbegin() const {
            auto temp = find_ptr(size_ - 1);
            return Deque<Arg>::rIterator(*temp.first + temp.second, temp.first, temp.second);
        };

        rIterator rend() const {
            auto temp = find_ptr(0);
            Arg *target = *temp.first + temp.second;
            subtract(temp.first, temp.second, target);
            return Deque<Arg>::rIterator(target, temp.first, temp.second);
        };

        crIterator crbegin() const {
            auto temp = find_ptr(size_ - 1);
            return Deque<Arg>::crIterator(*temp.first + temp.second, temp.first, temp.second);
        };

        crIterator crend() const {
            auto temp = find_ptr(0);
            Arg *target = *temp.first + temp.second;
            subtract(temp.first, temp.second, target);
            return Deque<Arg>::crIterator(target, temp.first, temp.second);
        };

    };


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    void Deque<Arg>::init() {
        map_size = (size_ + 7 + BLOCK_SIZE) / BLOCK_SIZE;
        map = Allocate_n<Arg *>(map_size);
        for (int i = 0; i < map_size; ++i) map[i] = Allocate_n<Arg>(BLOCK_SIZE);
        int rest = map_size * BLOCK_SIZE - size_;
        val_begin = rest / 2;
        val_end = (size_ + rest / 2) % BLOCK_SIZE;
        map_begin = 0;
        map_end = map_size - 1;
    }

    template<typename Arg>
    void Deque<Arg>::fill(Size pos, const Arg &target, Size size, void (*fun)(int &, int &)) {
        int i = (pos + val_begin) / BLOCK_SIZE + map_begin, j = (pos + val_begin) % BLOCK_SIZE;
        if (j < 0) j += BLOCK_SIZE;
        for (int index = 0; index < size; ++index) {
            map[i][j] = target;
            fun(i, j);
        }
    }

    template<typename Arg>
    void Deque<Arg>::fill(Size pos, const std::initializer_list<Arg> &list, void (*fun)(int &, int &)) {
        int i = (pos + val_begin) / BLOCK_SIZE + map_begin, j = (pos + val_begin) % BLOCK_SIZE;
        if (j < 0) j += BLOCK_SIZE;
        auto temp = list.begin();
        for (int index = 0; index < list.size(); ++index) {
            map[i][j] = *temp;
            ++temp;
            fun(i, j);
        }
    }

    template<typename Arg>
    void Deque<Arg>::fill(Size pos, const cIter<Arg> &begin, Size size, void (*fun)(int &, int &)) {
        int i = (pos + val_begin) / BLOCK_SIZE + map_begin, j = (pos + val_begin) % BLOCK_SIZE;
        if (j < 0) j += BLOCK_SIZE;
        auto temp = begin.deep_copy();
        for (int index = 0; index < size; ++index) {
            map[i][j] = **temp;
            ++(*temp);
            fun(i, j);
        }
    }

    template<typename Arg>
    void Deque<Arg>::expand_capacity(int size, bool to_end) {
        int i = 0;
        auto the_new = Allocate_n<Arg *>(map_size + size);
        if (to_end) {
            for (; i < map_size; ++i) the_new[i] = map[i];
            map_size += size;
            for (; i < map_size; ++i) the_new[i] = Allocate_n<Arg>(BLOCK_SIZE);
        } else {
            for (; i < size; ++i) the_new[i] = Allocate_n<Arg>(BLOCK_SIZE);
            map_size += size;
            for (; i < map_size; ++i) the_new[i] = map[i - size];
            map_begin += size;
            map_end += size;
        }
        Deallocate_n(map);
        map = the_new;
    }

    template<typename Arg>
    void Deque<Arg>::forward_move_element(Size step, Size size) {
        int i1 = map_begin, j1 = val_begin, i2 = map_begin, j2 = val_begin;
        subtract(i1, j1, step);
        map_begin = i1;
        val_begin = j1;
        for (Size i = 0; i < size; ++i) {
            map[i1][j1] = move(map[i2][j2]);
            add(i1, j1);
            add(i2, j2);
        }
    }

    template<typename Arg>
    void Deque<Arg>::backward_move_element(Size step, Size size) {
        int i1 = map_end, j1 = val_end, i2 = map_end, j2 = val_end;
        add(i1, j1, step);
        map_end = i1;
        val_end = j1;
        subtract(i1, j1);
        subtract(i2, j2);
        for (Size i = 0; i < size; ++i) {
            map[i1][j1] = move(map[i2][j2]);
            subtract(i1, j1);
            subtract(i2, j2);
        }
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Deque<Arg>::Deque() {
        init();
    }

    template<typename Arg>
    Deque<Arg>::Deque(Size size, const Arg &target) : size_(size) {
        init();
        fill(0, target, size, add);
    }

    template<typename Arg>
    Deque<Arg>::Deque(const std::initializer_list<Arg> &list) : size_(list.size()) {
        init();
        fill(0, list, add);
    }

    template<typename Arg>
    Deque<Arg>::Deque(const Iter<Arg> &begin, const Iter<Arg> &end) : Deque(*begin.to_const(), *end.to_const()) {}

    template<typename Arg>
    Deque<Arg>::Deque(const cIter<Arg> &begin, const cIter<Arg> &end) : size_(calculateLength(begin, end)) {
        init();
        fill(0, begin, size_, add);
    }

    template<typename Arg>
    Deque<Arg>::Deque(const Deque<Arg> &other) : size_(other.size_) {
        init();
        fill(0, other.cbegin(), size_, add);
    }

    template<typename Arg>
    Deque<Arg>::Deque(Deque<Arg> &&other) noexcept : size_(other.size_), map_size(other.map_size),
                                                     val_begin(other.val_begin), val_end(other.val_end),
                                                     map_begin(other.map_begin), map_end(other.map_end) {
        for (int i = 0; i < map_size; ++i) Deallocate_n(map[i]);
        Deallocate_n(map);
        map = other.map;
        other.size_ = 0;
        other.init();
    }

    template<typename Arg>
    Deque<Arg>::~Deque<Arg>() {
        for (int i = 0; i < map_size; ++i) Deallocate_n(map[i]);
        Deallocate_n(map);
    }

    template<typename Arg>
    void Deque<Arg>::clear() {
        size_ = 0;
        for (int i = 0; i < map_size; ++i) Deallocate_n(map[i]);
        Deallocate_n(map);
        init();
    }

    template<typename Arg>
    void Deque<Arg>::shirk_to_fit() {
        auto new_map_size = static_cast<int>((size_ + BLOCK_SIZE - 1) / BLOCK_SIZE);
        if (new_map_size == 0) new_map_size = 1;
        auto new_map = Allocate_n<Arg *>(new_map_size);
        for (int i = 0; i < new_map_size; ++i) new_map[i] = Allocate_n<Arg>(BLOCK_SIZE);

        int rest = new_map_size * BLOCK_SIZE - size_;
        int i1 = 0, j1 = rest / 2, i2 = map_begin, j2 = val_begin;
        for (int i = 0; i < size_; ++i) {
            new_map[i1][j1] = move(map[i2][j2]);
            add(i1, j1);
            add(i2, j2);
        }

        for (int j = 0; j < map_size; ++j) Deallocate_n(map[j]);
        Deallocate_n(map);

        map = new_map;
        map_size = new_map_size;
        val_begin = rest / 2;
        val_end = j1 - 1;
        map_begin = 0;
        map_end = map_size - 1;
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::assign(Size size, const Arg &target) {
        for (int i = 0; i < map_size; ++i) Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = size;
        init();
        fill(0, target, size, add);
        return *this;
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::assign(const std::initializer_list<Arg> &list) {
        for (int i = 0; i < map_size; ++i) Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = list.size();
        init();
        fill(0, list, add);
        return *this;
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::assign(const Iter<Arg> &begin, const Iter<Arg> &end) {
        return assign(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::assign(const cIter<Arg> &begin, const cIter<Arg> &end) {
        for (int i = 0; i < map_size; ++i) Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = calculateLength(begin, end);
        init();
        fill(0, begin, size_, add);
        return *this;
    }

    template<typename Arg>
    template<typename... args>
    void Deque<Arg>::emplace_front(args &&... vals) {
        ++size_;
        if (map_begin == 0 && val_begin == 0)
            expand_capacity(1, false);
        subtract(map_begin, val_begin);
        map[map_begin][val_begin] = Arg(vals...);
    }

    template<typename Arg>
    void Deque<Arg>::push_front(const Arg &val) {
        auto target = Allocate<Arg>(val);
        ++size_;
        if (map_begin == 0 && val_begin == 0)
            expand_capacity(1, false);
        subtract(map_begin, val_begin);
        map[map_begin][val_begin] = val;
    }

    template<typename Arg>
    void Deque<Arg>::push_front(Size size, const Arg &val) {
        size_ += size;
        auto rest = left_rest();
        if (rest < size)
            expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
        subtract(map_begin, val_begin, size);
        fill(0, val, size, add);
    }

    template<typename Arg>
    void Deque<Arg>::push_front(const Iter<Arg> &begin, const Iter<Arg> &end) {
        push_front(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    void Deque<Arg>::push_front(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto size = calculateLength(begin, end);
        size_ += size;
        auto rest = left_rest();
        if (rest < size)
            expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
        subtract(map_begin, val_begin, size);
        fill(0, begin, size, add);
    }

    template<typename Arg>
    void Deque<Arg>::pop_front() {
        map[map_begin][val_begin].~Arg();
        add(map_begin, val_begin);
    }

    template<typename Arg>
    template<typename... args>
    void Deque<Arg>::emplace_back(args &&... vals) {
        ++size_;
        if (map_end == map_size) expand_capacity(1, true);
        map[map_end][val_end] = Arg(vals...);
        add(map_end, val_end);
    }

    template<typename Arg>
    void Deque<Arg>::push_back(const Arg &val) {
        if (map_end == map_size) expand_capacity(1, true);
        map[map_end][val_end] = val;
        ++size_;
        add(map_end, val_end);
    }

    template<typename Arg>
    void Deque<Arg>::push_back(Size size, const Arg &val) {
        size_ += size;
        auto rest = right_rest();
        if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
        add(map_end, val_end, size);
        fill(size_ - size, val, size, add);
    }

    template<typename Arg>
    void Deque<Arg>::push_back(const Iter<Arg> &begin, const Iter<Arg> &end) {
        push_back(*begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    void Deque<Arg>::push_back(const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto size = calculateLength(begin, end);
        size_ += size;
        auto rest = right_rest();
        if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
        add(map_end, val_end, size);
        fill(size_ - size, begin, size, add);
    }

    template<typename Arg>
    void Deque<Arg>::pop_back() {
        subtract(map_end, val_end);
        map[map_end][val_end].~Arg();
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::Iterator Deque<Arg>::emplace(Size pos, args &&... vals) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::emplace' function");
        Pair<Arg **, int> pair;
        if (pos <= size_ / 2) {
            if (map_begin == 0 && val_begin == 0) expand_capacity(1, false);
            forward_move_element(1, pos);
            pair = find_ptr(pos);
            pair.first[0][pair.second] = Arg(vals...);
        } else {
            if (map_end == map_size) expand_capacity(1, true);
            backward_move_element(1, size_ - pos);
            pair = find_ptr(pos);
            pair.first[0][pair.second] = Arg(vals...);
        }
        ++size_;
        return Deque::Iterator(*pair.first + pair.second, pair.first, pair.second);
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::Iterator Deque<Arg>::emplace(const Deque::Iterator &pos, args &&... vals) {
        return emplace(find_pos(pos.map, pos.pos), forward<args>(vals)...);
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::cIterator Deque<Arg>::emplace(const Deque::cIterator &pos, args &&... vals) {
        Iterator temp = emplace(find_pos(pos.map, pos.pos), forward<args>(vals)...);
        return Deque::cIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::rIterator Deque<Arg>::emplace(const Deque::rIterator &pos, args &&... vals) {
        rIterator temp = emplace(*pos.to_const(), forward<args>(vals)...);
        return Deque::rIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::crIterator Deque<Arg>::emplace(const Deque::crIterator &pos, args &&... vals) {
        auto index = find_pos(pos.map, pos.pos) + 1;
        if (index > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::emplace' function");
        Pair<Arg **, int> pair;
        if (pos <= size_ / 2) {
            if (map_begin == 0 && val_begin == 0) expand_capacity(1, false);
            forward_move_element(1, index);
            pair = find_ptr(index);
            pair.first[0][pair.second] = Arg(vals...);
        } else {
            if (map_end == map_size) expand_capacity(1, true);
            backward_move_element(1, size_ - index);
            pair = find_ptr(index);
            pair.first[0][pair.second] = Arg(vals...);
        }
        ++size_;
        Arg *ptr = *pair.first + pair.second;
        subtract(pair.first, pair.second, ptr);
        return Deque::crIterator(ptr, pair.first, pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::insert(Size pos, const Arg &val, Size size) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        if (pos <= size_ / 2) {
            auto rest = left_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
            forward_move_element(size, pos);
            fill(pos, val, size, add);
        } else {
            auto rest = right_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
            backward_move_element(size, size_ - pos);
            fill(pos, val, size, add);
        }
        ++size_;
        auto pair = find_ptr(pos);
        return Deque::Iterator(*pair.first + pair.second, pair.first, pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::insert(Size pos, const std::initializer_list<Arg> &list) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        auto size = list.size();
        if (pos <= size_ / 2) {
            auto rest = left_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
            forward_move_element(size, pos);
            fill(pos, list, add);
        } else {
            auto rest = right_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
            backward_move_element(size, size_ - pos);
            fill(pos, list, add);
        }
        ++size_;
        auto pair = find_ptr(pos);
        return Deque::Iterator(*pair.first + pair.second, pair.first, pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::insert(Size pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return insert(pos, *begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::insert(Size pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        auto size = calculateLength(begin, end);
        if (pos <= size_ / 2) {
            auto rest = left_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
            forward_move_element(size, pos);
            fill(pos, begin, size, add);
        } else {
            auto rest = right_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
            backward_move_element(size, size_ - pos);
            fill(pos, begin, size, add);
        }
        ++size_;
        auto pair = find_ptr(pos);
        return Deque::Iterator(*pair.first + pair.second, pair.first, pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::insert(const Deque::Iterator &pos, Size size, const Arg &val) {
        return insert(find_pos(pos.map, pos.pos), size, val);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(const Deque::Iterator &pos, const std::initializer_list<Arg> &list) {
        return insert(find_pos(pos.map, pos.pos), list);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(const Deque::Iterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return insert(find_pos(pos.map, pos.pos), *begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(const Deque::Iterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        return insert(find_pos(pos.map, pos.pos), begin, end);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator Deque<Arg>::insert(const Deque::cIterator &pos, Size size, const Arg &val) {
        Iterator temp = insert(find_pos(pos.map, pos.pos), size, val);
        return Deque::cIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator
    Deque<Arg>::insert(const Deque::cIterator &pos, const std::initializer_list<Arg> &list) {
        Iterator temp = insert(find_pos(pos.map, pos.pos), list);
        return Deque::cIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator
    Deque<Arg>::insert(const Deque::cIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        Iterator temp = insert(find_pos(pos.map, pos.pos), *begin.to_const(), *end.to_const());
        return Deque::cIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator
    Deque<Arg>::insert(const Deque::cIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        Iterator temp = insert(find_pos(pos.map, pos.pos), begin, end);
        return Deque::cIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator Deque<Arg>::insert(const Deque::rIterator &pos, Size size, const Arg &val) {
        auto index = find_pos(pos.map, pos.pos) + 1;
        if (index > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        if (index <= size_ / 2) {
            auto rest = left_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
            forward_move_element(size, index);
            fill(index + size - 1, val, size, subtract);
        } else {
            auto rest = right_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
            backward_move_element(size, size_ - index);
            fill(index + size - 1, val, size, subtract);
        }
        ++size_;
        auto pair = find_ptr(index + size - 1);
        return Deque::rIterator(*pair.first + pair.second, pair.first, pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator
    Deque<Arg>::insert(const Deque::rIterator &pos, const std::initializer_list<Arg> &list) {
        auto size = list.size();
        auto index = find_pos(pos.map, pos.pos) + 1;
        if (index > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        if (index <= size_ / 2) {
            auto rest = left_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
            forward_move_element(size, index);
            fill(index + size - 1, list, subtract);
        } else {
            auto rest = right_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
            backward_move_element(size, size_ - index);
            fill(index + size - 1, list, subtract);
        }
        ++size_;
        auto pair = find_ptr(index + size - 1);
        return Deque::rIterator(*pair.first + pair.second, pair.first, pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator
    Deque<Arg>::insert(const Deque::rIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        return insert(pos, *begin.to_const(), *end.to_const());
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator
    Deque<Arg>::insert(const Deque::rIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        auto size = calculateLength(begin, end);
        auto index = find_pos(pos.map, pos.pos) + 1;
        if (index > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        if (index <= size_ / 2) {
            auto rest = left_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
            forward_move_element(size, index);
            fill(index + size - 1, begin, size, subtract);
        } else {
            auto rest = right_rest();
            if (rest < size) expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
            backward_move_element(size, size_ - index);
            fill(index + size - 1, begin, size, subtract);
        }
        ++size_;
        auto pair = find_ptr(index + size - 1);
        return Deque::rIterator(*pair.first + pair.second, pair.first, pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator Deque<Arg>::insert(const Deque::crIterator &pos, Size size, const Arg &val) {
        rIterator temp = insert(rIterator(pos.target, pos.map, pos.pos), size, val);
        return Deque::crIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator
    Deque<Arg>::insert(const Deque::crIterator &pos, const std::initializer_list<Arg> &list) {
        rIterator temp = insert(rIterator(pos.target, pos.map, pos.pos), list);
        return Deque::crIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator
    Deque<Arg>::insert(const Deque::crIterator &pos, const Iter<Arg> &begin, const Iter<Arg> &end) {
        rIterator temp = insert(rIterator(pos.target, pos.map, pos.pos), *begin.to_const(), *end.to_const());
        return Deque::crIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator
    Deque<Arg>::insert(const Deque::crIterator &pos, const cIter<Arg> &begin, const cIter<Arg> &end) {
        rIterator temp = insert(rIterator(pos.target, pos.map, pos.pos), begin, end);
        return Deque::crIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::erase(Size pos, Size size) {
        if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'Deque::erase' function");
        auto temp = find_ptr(pos);
        if (size == 0) return Deque<Arg>::Iterator(*temp.first + temp.second, temp.first, temp.second);
        auto target = *temp.first + temp.second;
        for (int i = 0; i < size; ++i) {
            target->~Arg();
            add(temp.first, temp.second, target);
        }
        size_ -= size;
        if (left_rest() < right_rest()) {
            subtract(temp.first, temp.second, target, size);
            auto record_map_end = map_end, record_val_end = val_end;
            map_end = temp.first - map;
            val_end = temp.second;
            backward_move_element(size, pos);
            map_end = record_map_end;
            val_end = record_val_end;
            add(map_begin, val_begin, size);
            return Deque<Arg>::Iterator(*temp.first + temp.second, temp.first, temp.second);
        } else {
            auto record_map_begin = map_begin, record_val_begin = val_begin;
            map_begin = temp.first - map;
            val_begin = temp.second;
            forward_move_element(size, size_ - pos);
            map_begin = record_map_begin;
            val_begin = record_val_begin;
            subtract(map_end, val_end, size);
            subtract(temp.first, temp.second, target, size);
            return Deque<Arg>::Iterator(*temp.first + temp.second, temp.first, temp.second);
        }
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::erase(const Deque::Iterator &iter) {
        return erase(find_pos(iter.map, iter.pos), 1);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator Deque<Arg>::erase(const Deque::cIterator &iter) {
        Iterator temp = erase(find_pos(iter.map, iter.pos), 1);
        return Deque::cIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::erase(const Deque::Iterator &begin, const Deque::Iterator &end) {
        return erase(find_pos(begin.map, begin.pos), calculateLength(begin, end));
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator Deque<Arg>::erase(const Deque::cIterator &begin, const Deque::cIterator &end) {
        Iterator temp = erase(find_pos(begin.map, begin.pos), calculateLength(begin, end));
        return Deque::cIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator Deque<Arg>::erase(const Deque::rIterator &iter) {
        Iterator temp = erase(find_pos(iter.map, iter.pos), 1);
        return Deque::rIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator Deque<Arg>::erase(const Deque::crIterator &iter) {
        Iterator temp = erase(find_pos(iter.map, iter.pos), 1);
        return Deque::crIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator Deque<Arg>::erase(const Deque::rIterator &begin, const Deque::rIterator &end) {
        Iterator temp = erase(find_pos(end.map, end.pos) + 1, calculateLength(begin, end));
        return Deque::rIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator Deque<Arg>::erase(const Deque::crIterator &begin, const Deque::crIterator &end) {
        Iterator temp = erase(find_pos(end.map, end.pos) + 1, calculateLength(begin, end));
        return Deque::crIterator(temp.target, temp.map, temp.pos);
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::operator=(const Deque<Arg> &other) {
        if (this == &other) return *this;
        for (int i = 0; i < map_size; ++i) Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = other.size_;
        init();
        fill(0, other.cbegin(), size_, add);
        return *this;
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::operator=(Deque<Arg> &&other) noexcept {
        if (this == &other) return *this;
        for (int i = 0; i < map_size; ++i) Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = other.size_;
        map_size = other.map_size;
        map_begin = other.map_begin;
        val_begin = other.val_begin;
        map_end = other.map_end;
        val_end = other.val_end;
        map = other.map;
        other.size_ = 0;
        other.init();
        return *this;
    }

    template<typename Type>
    bool operator==(const Deque<Type> &left, const Deque<Type> &right) {
        if (left.size_ != right.size_) return false;
        int i1 = left.map_begin, j1 = left.val_begin, i2 = right.map_begin, j2 = right.val_begin;
        for (int i = 0; i < left.size_; ++i) {
            if (left.map[i1][j1] != right.map[i2][j2]) return false;
            Deque<Type>::add(i1, j1);
            Deque<Type>::add(i2, j2);
        }
        return true;
    }

    template<typename Type>
    bool operator!=(const Deque<Type> &left, const Deque<Type> &right) {
        return !(left == right);
    }

    template<typename Type>
    bool operator<(const Deque<Type> &left, const Deque<Type> &right) {
        auto size = left.size_ > right.size_ ? right.size_ : left.size_;
        int i1 = left.map_begin, j1 = left.val_begin, i2 = right.map_begin, j2 = right.val_begin;
        for (int i = 0; i < size; ++i) {
            if (left.map[i1][j1] != right.map[i2][j2]) return left.map[i1][j1] < right.map[i2][j2];
            Deque<Type>::add(i1, j1);
            Deque<Type>::add(i2, j2);
        }
        return left.size_ == size && right.size_ != size;
    }

    template<typename Type>
    bool operator<=(const Deque<Type> &left, const Deque<Type> &right) {
        return !(left > right);
    }

    template<typename Type>
    bool operator>(const Deque<Type> &left, const Deque<Type> &right) {
        auto size = left.size_ > right.size_ ? right.size_ : left.size_;
        int i1 = left.map_begin, j1 = left.val_begin, i2 = right.map_begin, j2 = right.val_begin;
        for (int i = 0; i < size; ++i) {
            if (left.map[i1][j1] != right.map[i2][j2]) return left.map[i1][j1] > right.map[i2][j2];
            Deque<Type>::add(i1, j1);
            Deque<Type>::add(i2, j2);
        }
        return right.size_ == size && left.size_ != size;
    }

    template<typename Type>
    bool operator>=(const Deque<Type> &left, const Deque<Type> &right) {
        return !(left < right);
    }


//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    class Deque<Arg>::
    Iterator : public Iter<Arg> {
    protected:
        using Iter<Arg>::target;

        Arg **map;

        int pos;

        explicit Iterator(Arg *ptr, Arg **map_ptr, int index) : Iter<Arg>(ptr), map(map_ptr), pos(index) {};

    public:
        friend class Deque<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Deque<Arg>::Iterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Deque<Arg>::cIterator>(Deque<Arg>::cIterator(target, map, pos));
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
            add(map, pos, target);
            return *this;
        };

        Iterator operator++(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            add(map, pos, target);
            return Deque<Arg>::Iterator(temp1, temp2, temp3);
        };

        Iterator &operator--() {
            subtract(map, pos, target);
            return *this;
        };

        Iterator operator--(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            subtract(map, pos, target);
            return Deque<Arg>::Iterator(temp1, temp2, temp3);
        };

        Iterator operator+(Size size) const {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            add(temp2, temp3, temp1, size);
            return Deque<Arg>::Iterator(temp1, temp2, temp3);
        };

        Iterator operator-(Size size) const {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            subtract(temp2, temp3, temp1, size);
            return Deque<Arg>::Iterator(temp1, temp2, temp3);
        };

        Iterator &operator+=(Size size) {
            add(map, pos, target, size);
            return *this;
        };

        Iterator &operator-=(Size size) {
            subtract(map, pos, target, size);
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

        int pos;

        explicit cIterator(Arg *ptr, Arg **map_ptr, int index) : cIter<Arg>(ptr), map(map_ptr), pos(index) {};

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
            add(map, pos, target);
            return *this;
        };

        cIterator operator++(int) {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            add(map, pos, target);
            return Deque<Arg>::cIterator(temp1, temp2, temp3);
        };

        cIterator &operator--() {
            subtract(map, pos, target);
            return *this;
        };

        cIterator operator--(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            subtract(map, pos, target);
            return Deque<Arg>::cIterator(temp1, temp2, temp3);
        };

        cIterator operator+(Size size) const {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            add(temp2, temp3, temp1, size);
            return Deque<Arg>::cIterator(temp1, temp2, temp3);
        };

        cIterator operator-(Size size) const {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            subtract(temp2, temp3, temp1, size);
            return Deque<Arg>::cIterator(temp1, temp2, temp3);
        };

        cIterator &operator+=(Size size) {
            add(map, pos, target, size);
            return *this;
        };

        cIterator &operator-=(Size size) {
            subtract(map, pos, target, size);
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

        int pos;

        explicit rIterator(Arg *ptr, Arg **map_ptr, int index) : Iter<Arg>(ptr), map(map_ptr), pos(index) {};

    public:
        friend class Deque<Arg>;

        Shared_ptr<Iter<Arg>> deep_copy() const override { return make_shared<Deque<Arg>::rIterator>(*this); };

        Shared_ptr<cIter<Arg>> to_const() const override {
            return make_shared<Deque<Arg>::crIterator>(Deque<Arg>::crIterator(target, map, pos));
        };

        rIterator(const rIterator &other) : Iter<Arg>(other.target), map(other.map), pos(other.pos) {};

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
            add(map, pos, target);
            return *this;
        };

        rIterator operator--(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            add(map, pos, target);
            return Deque<Arg>::rIterator(temp1, temp2, temp3);
        };

        rIterator &operator++() override {
            subtract(map, pos, target);
            return *this;
        };

        rIterator operator++(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            subtract(map, pos, target);
            return Deque<Arg>::rIterator(temp1, temp2, temp3);
        };

        rIterator operator-(Size size) const {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            add(temp2, temp3, temp1, size);
            return Deque<Arg>::rIterator(temp1, temp2, temp3);
        };

        rIterator operator+(Size size) const {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            subtract(temp2, temp3, temp1, size);
            return Deque<Arg>::rIterator(temp1, temp2, temp3);
        };

        rIterator &operator-=(Size size) {
            add(map, pos, target, size);
            return *this;
        };

        rIterator &operator+=(Size size) {
            subtract(map, pos, target, size);
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

        int pos;

        explicit crIterator(Arg *ptr, Arg **map_ptr, int index) : cIter<Arg>(ptr), map(map_ptr), pos(index) {};

    public:
        friend class Deque<Arg>;

        friend class Deque<Arg>::rIterator;

        Shared_ptr<cIter<Arg>> deep_copy() const override { return make_shared<Deque<Arg>::crIterator>(*this); };

        crIterator(const crIterator &other) : cIter<Arg>(other.target), map(other.map), pos(other.pos) {};

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
            add(map, pos, target);
            return *this;
        };

        crIterator operator--(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            add(map, pos, target);
            return Deque<Arg>::crIterator(temp1, temp2, temp3);
        };

        crIterator &operator++() override {
            subtract(map, pos, target);
            return *this;
        };

        crIterator operator++(int) {
            auto temp1 = target;
            auto temp2 = map;
            auto temp3 = pos;
            subtract(map, pos, target);
            return Deque<Arg>::crIterator(temp1, temp2, temp3);
        };

        crIterator operator-(Size size) const {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            add(temp2, temp3, temp1, size);
            return Deque<Arg>::crIterator(temp1, temp2, temp3);
        };

        crIterator operator+(Size size) const {
            auto temp2 = map;
            auto temp3 = pos;
            auto temp1 = target;
            subtract(temp2, temp3, temp1, size);
            return Deque<Arg>::crIterator(temp1, temp2, temp3);
        };

        crIterator &operator-=(Size size) {
            add(map, pos, target, size);
            return *this;
        };

        crIterator &operator+=(Size size) {
            subtract(map, pos, target, size);
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
