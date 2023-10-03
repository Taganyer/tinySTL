//
// Created by 86152 on 2023/9/18.
//

#ifndef TINYSTL_DEQUE_HPP
#define TINYSTL_DEQUE_HPP

#include "Iterator.hpp"
#include "Allocater.hpp"
#include "Algorithms/source/Container_algorithms.hpp"
#include <initializer_list>

namespace STD {

    const int BLOCK_SIZE = 128;

    template<typename Arg>
    class Deque;

    namespace Detail {

        template<typename Arg>
        struct Deque_Iterator : public Iterator<Random_iterator_tag, Arg> {
        public:
            using Basic = Iterator<Random_iterator_tag, Arg>;

            using Self = Deque_Iterator<Arg>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            using Container = Deque<Arg>;

            friend class Deque<Arg>;

            friend class rIterator<Self, Random_iterator_tag>;

            friend class cIterator<Self, Random_iterator_tag>;

            friend class crIterator<Self, Random_iterator_tag>;

        private:
            Arg **map = nullptr;

            int pos = 0;

            Arg *target = nullptr;

            explicit Deque_Iterator(Arg **map, int pos, Arg *target) :
                    map(map), pos(pos), target(target) {};

        public:
            Reference operator*() const {
                return *target;
            };

            Pointer operator->() const {
                return target;
            };

            Reference operator[](Step size) const {
                return Container::find_value(map, pos, target, size);
            }

            Self &operator++() {
                Container::add(map, pos, target);
                return *this;
            }

            Self operator++(int) {
                Self temp = Self(*this);
                Container::add(map, pos, target);
                return temp;
            };

            Self &operator--() {
                Container::subtract(map, pos, target);
                return *this;
            };

            Self operator--(int) {
                Self temp = Self(*this);
                Container::subtract(map, pos, target);
                return temp;
            };

            Self &operator+=(Step size) {
                Container::add(map, pos, target, size);
                return *this;
            };

            Self &operator-=(Step size) {
                Container::subtract(map, pos, target, size);
                return *this;
            };

            friend Self operator+(const Self &Iterator, Step size) {
                auto map_ = Iterator.map;
                auto pos_ = Iterator.pos;
                auto target_ = Iterator.target;
                Container::add(map_, pos_, target_, size);
                return Self(map_, pos_, target_);
            };

            friend Self operator-(const Self &Iterator, Step size) {
                auto map_ = Iterator.map;
                auto pos_ = Iterator.pos;
                auto target_ = Iterator.target;
                Container::subtract(map_, pos_, target_, size);
                return Self(map_, pos_, target_);
            };

            friend Difference operator-(const Self &left, const Self &right) {
                return (left.map - right.map) * BLOCK_SIZE + (left.pos - right.pos);
            };

            friend bool operator==(const Self &left, const Self &right) {
                return left.target == right.target;
            };

            friend bool operator!=(const Self &left, const Self &right) {
                return left.target != right.target;
            };

            friend bool operator<(const Self &left, const Self &right) {
                if (left.map != right.map)
                    return left.map < right.map;
                return left.pos < right.pos;
            };

            friend bool operator<=(const Self &left, const Self &right) {
                return !(left > right);
            };

            friend bool operator>(const Self &left, const Self &right) {
                if (left.map != right.map)
                    return left.map > right.map;
                return left.pos > right.pos;
            };

            friend bool operator>=(const Self &left, const Self &right) {
                return !(left < right);
            };
        };
    }

    template<typename Arg>
    class Deque {
    public:
        friend class Detail::Deque_Iterator<Arg>;

        using Iterator = Detail::Deque_Iterator<Arg>;

        using cIterator = STD::cIterator<Iterator, Random_iterator_tag>;

        using rIterator = STD::rIterator<Iterator, Random_iterator_tag>;

        using crIterator = STD::crIterator<Iterator, Random_iterator_tag>;

    private:
        Size size_ = 0;

        int val_begin, val_end, map_begin, map_end, map_size;

        Arg **map = nullptr;

    public:
        Deque();

        Deque(Size size, const Arg &target = Arg());

        Deque(const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Deque(const Input_iterator &begin, const Input_iterator &end);

        Deque(const Deque<Arg> &other);

        Deque(Deque<Arg> &&other) noexcept;

        ~Deque<Arg>();

        void clear();

        void shirk_to_fit();

        Deque<Arg> &assign(Size size, const Arg &target);

        Deque<Arg> &assign(const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Deque<Arg> &assign(const Input_iterator &begin, const Input_iterator &end);

        template<typename... args>
        void emplace_front(args &&...vals);

        void push_front(const Arg &val);

        void push_front(Arg &&val);

        void push_front(Size size, const Arg &val);

        template<typename Input_iterator>
        void push_front(const Input_iterator &begin, const Input_iterator &end);

        void pop_front();

        template<typename... args>
        void emplace_back(args &&...vals);

        void push_back(const Arg &val);

        void push_back(Arg &&val);

        void push_back(Size size, const Arg &val);;

        template<typename Input_iterator>
        void push_back(const Input_iterator &begin, const Input_iterator &end);

        void pop_back();

        template<typename... args>
        Iterator emplace(Size pos, args &&...vals);

        template<typename... args>
        Iterator emplace(const Iterator &pos, args &&...vals);

        template<typename... args>
        cIterator emplace(const cIterator &pos, args &&...vals);

        template<typename... args>
        rIterator emplace(const rIterator &pos, args &&...vals);

        template<typename... args>
        crIterator emplace(const crIterator &pos, args &&...vals);

        Iterator insert(Size pos, const Arg &val);

        Iterator insert(const Iterator &pos, const Arg &val);

        cIterator insert(const cIterator &pos, const Arg &val);

        rIterator insert(const rIterator &pos, const Arg &val);

        crIterator insert(const crIterator &pos, const Arg &val);

        Iterator insert(Size pos, Size size, const Arg &val);

        Iterator insert(Size pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Iterator insert(Size pos, const Input_iterator &begin, const Input_iterator &end);

        Iterator insert(const Iterator &pos, Size size, const Arg &val);

        Iterator insert(const Iterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        Iterator insert(const Iterator &pos, const Input_iterator &begin, const Input_iterator &end);

        cIterator insert(const cIterator &pos, Size size, const Arg &val);

        cIterator insert(const cIterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        cIterator insert(const cIterator &pos, const Input_iterator &begin, const Input_iterator &end);

        rIterator insert(const rIterator &pos, Size size, const Arg &val);

        rIterator insert(const rIterator &pos, const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        rIterator insert(const rIterator &pos,
                         const Input_iterator &begin, const Input_iterator &end);

        crIterator insert(const crIterator &pos, Size size, const Arg &val);

        crIterator insert(const crIterator &pos,
                          const std::initializer_list<Arg> &list);

        template<typename Input_iterator>
        crIterator insert(const crIterator &pos,
                          const Input_iterator &begin, const Input_iterator &end);

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
            if (pos >= size_)
                throw outOfRange("You provided an out-of-range subscript int the 'Deque::at' function");
            auto temp = find_ptr(pos);
            return temp.first[0][temp.second];
        };

        Size size() const { return size_; };

        Size capacity() const { return BLOCK_SIZE * map_size; };

        Arg &front() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'Deque::front' function");
            return map[map_begin][val_begin];
        };

        Arg &back() const {
            if (!size_)
                throw outOfRange("You're accessing a non-existent element in the 'Deque::back' function");
            return map[val_end ? map_end : map_end - 1]
            [val_end ? val_end - 1 : BLOCK_SIZE - 1];
        };

        bool empty() const { return !size_; };

        void swap(Deque<Arg> &other) noexcept {
            Size temp1 = size_;
            int temp2 = val_begin, temp3 = val_end,
                    temp4 = map_begin, temp5 = map_end;
            Arg **temp6 = map;
            size_ = other.size_;
            val_begin = other.val_begin;
            val_end = other.val_end;
            map_begin = other.map_begin;
            map_end = other.map_end;
            map = other.map;
            other.size_ = temp1;
            other.val_begin = temp2;
            other.val_end = temp3;
            other.map_begin = temp4;
            other.map_end = temp5;
            other.map = temp6;
        };

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

        template<typename Type>
        friend inline void swap(Deque<Type> &left, Deque<Type> &right) noexcept;

        Iterator begin() const {
            return Iterator(map + map_begin, val_begin, *(map + map_begin) + val_begin);
        };

        Iterator end() const {
            return Iterator(map + map_end, val_end, *(map + map_end) + val_end);
        };

        cIterator cbegin() const {
            return cIterator(Iterator(map + map_begin, val_begin, *(map + map_begin) + val_begin));
        };

        cIterator cend() const {
            return cIterator(Iterator(map + map_end, val_end, *(map + map_end) + val_end));
        };

        rIterator rbegin() const {
            auto temp = find_ptr(size_ - 1);
            return rIterator(Iterator(temp.first, temp.second, *temp.first + temp.second));
        };

        rIterator rend() const {
            auto temp = find_ptr(0);
            Arg *target = *temp.first + temp.second;
            subtract(temp.first, temp.second, target);
            return rIterator(Iterator(temp.first, temp.second, target));
        };

        crIterator crbegin() const {
            auto temp = find_ptr(size_ - 1);
            return crIterator(Iterator(temp.first, temp.second, *temp.first + temp.second));
        };

        crIterator crend() const {
            auto temp = find_ptr(0);
            Arg *target = *temp.first + temp.second;
            subtract(temp.first, temp.second, target);
            return crIterator(Iterator(temp.first, temp.second, target));
        };

    private:
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
            } else
                ++ptr;
        };

        inline static void subtract(Arg **&i, int &j, Arg *&ptr) {
            if (--j < 0) {
                j = BLOCK_SIZE - 1;
                --i;
                ptr = *i + j;
            } else
                --ptr;
        }

        inline static void add(int &i, int &j, const Size &size) {
            if (j + size < BLOCK_SIZE)
                j += static_cast<int>(size);
            else {
                i += static_cast<int>((j + size) / BLOCK_SIZE);
                j = static_cast<int>((j + size) % BLOCK_SIZE);
            }
        };

        inline static void subtract(int &i, int &j, const Size &size) {
            if (size <= j)
                j -= static_cast<int>(size);
            else {
                i -= static_cast<int>((size - j + BLOCK_SIZE - 1) / BLOCK_SIZE);
                j = BLOCK_SIZE - static_cast<int>((size - j) % BLOCK_SIZE);
                if (j == BLOCK_SIZE)
                    j = 0;
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
                if (j == BLOCK_SIZE)
                    j = 0;
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

        inline Arg &get_value(Arg **i, int j, Arg *ptr, const Size &size) {
            add(i, j, ptr, size);
            return *ptr;
        };

        Iterator get_Iterator(const Size &pos) {
            int i = (pos + val_begin) / BLOCK_SIZE + map_begin,
                    j = (pos + val_begin) % BLOCK_SIZE;
            if (j < 0)
                j += BLOCK_SIZE;
            return Iterator(map + i, j, map[i] + j);
        };

        Size left_rest() const { return map_begin * BLOCK_SIZE + val_begin; };

        Size right_rest() const {
            return (map_size - map_end) * BLOCK_SIZE - val_end;
        };

        void init();

        void forward_move_element(Size step, Size size);

        void backward_move_element(Size step, Size size);

        void expand_capacity(int size, bool to_end);

        void insert_move(Size pos, Size size);

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    void Deque<Arg>::init() {
        map_size = (size_ + 7 + BLOCK_SIZE) / BLOCK_SIZE;
        map = Allocate_n<Arg *>(map_size);
        for (int i = 0; i < map_size; ++i)
            map[i] = Allocate_n<Arg>(BLOCK_SIZE);
        int rest = map_size * BLOCK_SIZE - size_;
        val_begin = rest / 2;
        val_end = (size_ + rest / 2) % BLOCK_SIZE;
        map_begin = 0;
        map_end = map_size - 1;
    }

    template<typename Arg>
    void Deque<Arg>::expand_capacity(int size, bool to_end) {
        int i = 0;
        auto the_new = Allocate_n<Arg *>(map_size + size);
        if (to_end) {
            for (; i < map_size; ++i)
                the_new[i] = map[i];
            map_size += size;
            for (; i < map_size; ++i)
                the_new[i] = Allocate_n<Arg>(BLOCK_SIZE);
        } else {
            for (; i < size; ++i)
                the_new[i] = Allocate_n<Arg>(BLOCK_SIZE);
            map_size += size;
            for (; i < map_size; ++i)
                the_new[i] = map[i - size];
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

    template<typename Arg>
    void Deque<Arg>::insert_move(Size pos, Size size) {
        if (pos <= size_ / 2) {
            auto rest = left_rest();
            if (rest < size)
                expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
            forward_move_element(size, pos);
        } else {
            auto rest = right_rest();
            if (rest < size)
                expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
            backward_move_element(size, size_ - pos);
        }
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Arg>
    Deque<Arg>::Deque() { init(); }

    template<typename Arg>
    Deque<Arg>::Deque(Size size, const Arg &target) : size_(size) {
        init();
        fill_with(this->begin(), size, target);
    }

    template<typename Arg>
    Deque<Arg>::Deque(const std::initializer_list<Arg> &list) : size_(list.size()) {
        init();
        fill_with(this->begin(), list);
    }

    template<typename Arg>
    template<typename Input_iterator>
    Deque<Arg>::Deque(const Input_iterator &begin, const Input_iterator &end)
            : size_(get_size(begin, end)) {
        init();
        fill_with(this->begin(), begin, end);
    }

    template<typename Arg>
    Deque<Arg>::Deque(const Deque<Arg> &other) : size_(other.size_) {
        init();
        fill_with(this->begin(), other.begin(), other.end());
    }

    template<typename Arg>
    Deque<Arg>::Deque(Deque<Arg> &&other) noexcept
            : size_(other.size_), val_begin(other.val_begin),
              val_end(other.val_end), map_begin(other.map_begin),
              map_end(other.map_end) {
        for (int i = 0; i < map_size; ++i)
            Deallocate_n(map[i]);
        map_size = other.map_size;
        Deallocate_n(map);
        map = other.map;
        other.size_ = 0;
        other.init();
    }

    template<typename Arg>
    Deque<Arg>::~Deque<Arg>() {
        for (int i = 0; i < map_size; ++i)
            Deallocate_n(map[i]);
        Deallocate_n(map);
    }

    template<typename Arg>
    void Deque<Arg>::clear() {
        size_ = 0;
        for (int i = 0; i < map_size; ++i)
            Deallocate_n(map[i]);
        Deallocate_n(map);
        init();
    }

    template<typename Arg>
    void Deque<Arg>::shirk_to_fit() {
        auto new_map_size = static_cast<int>((size_ + BLOCK_SIZE - 1) / BLOCK_SIZE);
        if (new_map_size == 0)
            new_map_size = 1;
        auto new_map = Allocate_n<Arg *>(new_map_size);
        for (int i = 0; i < new_map_size; ++i)
            new_map[i] = Allocate_n<Arg>(BLOCK_SIZE);

        int rest = new_map_size * BLOCK_SIZE - size_;
        int i1 = 0, j1 = rest / 2, i2 = map_begin, j2 = val_begin;
        for (int i = 0; i < size_; ++i) {
            new_map[i1][j1] = move(map[i2][j2]);
            add(i1, j1);
            add(i2, j2);
        }

        for (int j = 0; j < map_size; ++j)
            Deallocate_n(map[j]);
        Deallocate_n(map);

        map = new_map;
        map_size = new_map_size;
        val_begin = rest / 2;
        val_end = j1;
        map_begin = 0;
        map_end = i1;
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::assign(Size size, const Arg &target) {
        for (int i = 0; i < map_size; ++i)
            Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = size;
        init();
        fill_with(this->begin(), size, target);
        return *this;
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::assign(const std::initializer_list<Arg> &list) {
        for (int i = 0; i < map_size; ++i)
            Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = list.size();
        init();
        fill_with(this->begin(), list);
        return *this;
    }

    template<typename Arg>
    template<typename Input_iterator>
    Deque<Arg> &Deque<Arg>::assign(const Input_iterator &begin, const Input_iterator &end) {
        for (int i = 0; i < map_size; ++i)
            Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = get_size(begin, end);
        init();
        fill_with(this->begin(), begin, end);
        return *this;
    }

    template<typename Arg>
    template<typename... args>
    void Deque<Arg>::emplace_front(args &&...vals) {
        if (map_begin == 0 && val_begin == 0)
            expand_capacity(1, false);
        subtract(map_begin, val_begin);
        map[map_begin][val_begin] = Arg(vals...);
        ++size_;
    }

    template<typename Arg>
    void Deque<Arg>::push_front(const Arg &val) {
        if (map_begin == 0 && val_begin == 0)
            expand_capacity(1, false);
        subtract(map_begin, val_begin);
        map[map_begin][val_begin] = val;
        ++size_;
    }

    template<typename Arg>
    void Deque<Arg>::push_front(Arg &&val) {
        if (map_begin == 0 && val_begin == 0)
            expand_capacity(1, false);
        subtract(map_begin, val_begin);
        map[map_begin][val_begin] = move(val);
        ++size_;
    }

    template<typename Arg>
    void Deque<Arg>::push_front(Size size, const Arg &val) {
        auto rest = left_rest();
        if (rest < size)
            expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
        subtract(map_begin, val_begin, size);
        fill_with(this->begin(), size, val);
        size_ += size;
    }

    template<typename Arg>
    template<typename Input_iterator>
    void Deque<Arg>::push_front(const Input_iterator &begin, const Input_iterator &end) {
        auto size = get_size(begin, end);
        auto rest = left_rest();
        if (rest < size)
            expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, false);
        subtract(map_begin, val_begin, size);
        fill_with(this->begin(), begin, end);
        size_ += size;
    }

    template<typename Arg>
    void Deque<Arg>::pop_front() {
        map[map_begin][val_begin].~Arg();
        add(map_begin, val_begin);
        --size_;
    }

    template<typename Arg>
    template<typename... args>
    void Deque<Arg>::emplace_back(args &&...vals) {
        if (map_end == map_size)
            expand_capacity(1, true);
        map[map_end][val_end] = Arg(vals...);
        add(map_end, val_end);
        ++size_;
    }

    template<typename Arg>
    void Deque<Arg>::push_back(const Arg &val) {
        if (map_end == map_size)
            expand_capacity(1, true);
        map[map_end][val_end] = val;
        add(map_end, val_end);
        ++size_;
    }

    template<typename Arg>
    void Deque<Arg>::push_back(Arg &&val) {
        if (map_end == map_size)
            expand_capacity(1, true);
        map[map_end][val_end] = move(val);
        add(map_end, val_end);
        ++size_;
    }

    template<typename Arg>
    void Deque<Arg>::push_back(Size size, const Arg &val) {
        auto rest = right_rest();
        if (rest < size)
            expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
        fill_with(this->end(), size, val);
        add(map_end, val_end, size);
        size_ += size;
    }

    template<typename Arg>
    template<typename Input_iterator>
    void Deque<Arg>::push_back(const Input_iterator &begin, const Input_iterator &end) {
        auto size = get_size(begin, end);
        auto rest = right_rest();
        if (rest < size)
            expand_capacity((size - rest + BLOCK_SIZE - 1) / BLOCK_SIZE, true);
        fill_with(this->end(), begin, end);
        add(map_end, val_end, size);
        size_ += size;
    }

    template<typename Arg>
    void Deque<Arg>::pop_back() {
        subtract(map_end, val_end);
        map[map_end][val_end].~Arg();
        --size_;
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::Iterator Deque<Arg>::emplace(Size pos, args &&...vals) {
        if (pos > size_)
            throw outOfRange("You passed an out-of-range value in the 'Deque::emplace' function");
        insert_move(pos, 1);
        Pair<Arg **, int> pair = find_ptr(pos);
        pair.first[0][pair.second] = Arg(vals...);
        ++size_;
        return Iterator(pair.first, pair.second, *pair.first + pair.second);
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::Iterator Deque<Arg>::emplace(const Iterator &pos, args &&...vals) {
        return emplace(find_pos(pos.map, pos.pos), forward<args>(vals)...);
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::cIterator
    Deque<Arg>::emplace(const cIterator &pos, args &&...vals) {
        return cIterator(emplace(find_pos(pos.target.map, pos.target.pos), forward<args>(vals)...));
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::rIterator
    Deque<Arg>::emplace(const Deque::rIterator &pos, args &&...vals) {
        return rIterator(emplace((--pos).target, forward(vals)...));
    }

    template<typename Arg>
    template<typename... args>
    typename Deque<Arg>::crIterator
    Deque<Arg>::emplace(const crIterator &pos, args &&...vals) {
        return crIterator(emplace((--pos).target, forward(vals)...));
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(Size pos, const Arg &val) {
        return insert(pos, 1, val);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(const Iterator &pos, const Arg &val) {
        return insert(pos, 1, val);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator
    Deque<Arg>::insert(const cIterator &pos, const Arg &val) {
        return insert(pos, 1, val);
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator
    Deque<Arg>::insert(const rIterator &pos, const Arg &val) {
        return insert(pos, 1, val);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator
    Deque<Arg>::insert(const crIterator &pos, const Arg &val) {
        return insert(pos, 1, val);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(Size pos, Size size, const Arg &val) {
        if (pos > size_)
            throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        insert_move(pos, size);
        fill_with(get_Iterator(pos), size, val);
        size_ += size;
        auto pair = find_ptr(pos);
        return Iterator(pair.first, pair.second, *pair.first + pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(Size pos, const std::initializer_list<Arg> &list) {
        if (pos > size_)
            throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        auto size = list.size();
        insert_move(pos, size);
        fill_with(get_Iterator(pos), list);
        size_ += size;
        auto pair = find_ptr(pos);
        return Iterator(pair.first, pair.second, *pair.first + pair.second);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(Size pos, const Input_iterator &begin, const Input_iterator &end) {
        if (pos > size_)
            throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        auto size = get_size(begin, end);
        insert_move(pos, size);
        fill_with(get_Iterator(pos), begin, end);
        size_ += size;
        auto pair = find_ptr(pos);
        return Iterator(pair.first, pair.second, *pair.first + pair.second);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(const Iterator &pos, Size size, const Arg &val) {
        return insert(find_pos(pos.map, pos.pos), size, val);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(const Iterator &pos, const std::initializer_list<Arg> &list) {
        return insert(find_pos(pos.map, pos.pos), list);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Deque<Arg>::Iterator
    Deque<Arg>::insert(const Iterator &pos, const Input_iterator &begin, const Input_iterator &end) {
        return insert(find_pos(pos.map, pos.pos), begin, end);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator
    Deque<Arg>::insert(const cIterator &pos, Size size, const Arg &val) {
        return cIterator(insert(pos.target, size, val));
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator
    Deque<Arg>::insert(const cIterator &pos, const std::initializer_list<Arg> &list) {
        return cIterator(insert(pos.target, list));
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Deque<Arg>::cIterator
    Deque<Arg>::insert(const cIterator &pos, const Input_iterator &begin, const Input_iterator &end) {
        return cIterator(insert(pos.target, begin, end));
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator
    Deque<Arg>::insert(const rIterator &pos, Size size, const Arg &val) {
        auto index = find_pos(pos.target.map, pos.target.pos) + 1;
        if (index > size_)
            throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        insert_move(index, size);
        auto iter = get_Iterator(index + size - 1);
        rfill_with(iter, val, size);
        size_ += size;
        return rIterator(iter);
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator
    Deque<Arg>::insert(const rIterator &pos, const std::initializer_list<Arg> &list) {
        auto size = list.size();
        auto index = find_pos(pos.target.map, pos.target.pos) + 1;
        insert_move(index, size);
        auto iter = get_Iterator(index + size - 1);
        rfill_with(iter, list);
        size_ += size;
        return rIterator(iter);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Deque<Arg>::rIterator
    Deque<Arg>::insert(const rIterator &pos, const Input_iterator &begin, const Input_iterator &end) {
        auto size = get_size(begin, end);
        auto index = find_pos(pos.target.map, pos.target.pos) + 1;
        if (index > size_)
            throw outOfRange("You passed an out-of-range value in the 'Deque::insert' function");
        insert_move(index, size);
        auto iter = get_Iterator(index + size - 1);
        rfill_with(iter, begin, end);
        size_ += size;
        return rIterator(iter);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator
    Deque<Arg>::insert(const crIterator &pos, Size size, const Arg &val) {
        rIterator temp = insert(rIterator(pos.target), size, val);
        return crIterator(temp.target);
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator
    Deque<Arg>::insert(const crIterator &pos, const std::initializer_list<Arg> &list) {
        rIterator temp = insert(rIterator(pos.target), list);
        return crIterator(temp.target);
    }

    template<typename Arg>
    template<typename Input_iterator>
    typename Deque<Arg>::crIterator
    Deque<Arg>::insert(const crIterator &pos, const Input_iterator &begin, const Input_iterator &end) {
        rIterator temp = insert(rIterator(pos.target), begin, end);
        return crIterator(temp.target);
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::erase(Size pos, Size size) {
        if (pos > size_)
            throw outOfRange("You passed an out-of-range value in the 'Deque::erase' function");
        auto temp = find_ptr(pos);
        if (size == 0)
            return Iterator(temp.first, temp.second, *temp.first + temp.second);
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
            return Iterator(temp.first, temp.second, *temp.first + temp.second);
        } else {
            auto record_map_begin = map_begin, record_val_begin = val_begin;
            map_begin = temp.first - map;
            val_begin = temp.second;
            forward_move_element(size, size_ - pos);
            map_begin = record_map_begin;
            val_begin = record_val_begin;
            subtract(map_end, val_end, size);
            subtract(temp.first, temp.second, target, size);
            return Iterator(temp.first, temp.second, *temp.first + temp.second);
        }
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator Deque<Arg>::erase(const Iterator &iter) {
        return erase(find_pos(iter.map, iter.pos), 1);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator Deque<Arg>::erase(const cIterator &iter) {
        return cIterator(erase(find_pos(iter.target.map, iter.target.pos), 1));
    }

    template<typename Arg>
    typename Deque<Arg>::Iterator
    Deque<Arg>::erase(const Iterator &begin, const Iterator &end) {
        return erase(find_pos(begin.map, begin.pos), end - begin);
    }

    template<typename Arg>
    typename Deque<Arg>::cIterator
    Deque<Arg>::erase(const cIterator &begin, const cIterator &end) {
        return cIterator(erase(find_pos(begin.target.map, begin.target.pos), end - begin));
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator
    Deque<Arg>::erase(const rIterator &iter) {
        return rIterator(erase(find_pos(iter.target.map, iter.target.pos), 1));
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator
    Deque<Arg>::erase(const crIterator &iter) {
        return crIterator(erase(find_pos(iter.target.map, iter.target.pos), 1));
    }

    template<typename Arg>
    typename Deque<Arg>::rIterator
    Deque<Arg>::erase(const rIterator &begin, const rIterator &end) {
        return rIterator(erase(find_pos((end.target + 1).map, (end.target + 1).pos), end - begin));
    }

    template<typename Arg>
    typename Deque<Arg>::crIterator
    Deque<Arg>::erase(const crIterator &begin, const crIterator &end) {
        return crIterator(erase(find_pos((end.target + 1).map, (end.target + 1).pos), end - begin));
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::operator=(const Deque<Arg> &other) {
        if (this == &other)
            return *this;
        for (int i = 0; i < map_size; ++i)
            Deallocate_n(map[i]);
        Deallocate_n(map);
        size_ = other.size_;
        init();
        fill_with(this->begin(), other.begin(), other.end());
        return *this;
    }

    template<typename Arg>
    Deque<Arg> &Deque<Arg>::operator=(Deque<Arg> &&other) noexcept {
        if (this == &other)
            return *this;
        for (int i = 0; i < map_size; ++i)
            Deallocate_n(map[i]);
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
        if (left.size_ != right.size_)
            return false;
        int i1 = left.map_begin, j1 = left.val_begin, i2 = right.map_begin,
                j2 = right.val_begin;
        for (int i = 0; i < left.size_; ++i) {
            if (left.map[i1][j1] != right.map[i2][j2])
                return false;
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
        int i1 = left.map_begin, j1 = left.val_begin, i2 = right.map_begin,
                j2 = right.val_begin;
        for (int i = 0; i < size; ++i) {
            if (left.map[i1][j1] != right.map[i2][j2])
                return left.map[i1][j1] < right.map[i2][j2];
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
        int i1 = left.map_begin, j1 = left.val_begin, i2 = right.map_begin,
                j2 = right.val_begin;
        for (int i = 0; i < size; ++i) {
            if (left.map[i1][j1] != right.map[i2][j2])
                return left.map[i1][j1] > right.map[i2][j2];
            Deque<Type>::add(i1, j1);
            Deque<Type>::add(i2, j2);
        }
        return right.size_ == size && left.size_ != size;
    }

    template<typename Type>
    bool operator>=(const Deque<Type> &left, const Deque<Type> &right) {
        return !(left < right);
    }

    template<typename Type>
    inline void swap(Deque<Type> &left, Deque<Type> &right) noexcept {
        left.swap(right);
    }

//----------------------------------------------------------------------------------------------------------------------

} // namespace STD

#endif // TINYSTL_DEQUE_HPP
