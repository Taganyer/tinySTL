//
// Created by 86152 on 2023/9/9.
//

#include "../String.hpp"

using namespace STD;

//----------------------------------------------------------------------------------------------------------------------

class String::Iterator : public Iter<char> {
protected:
    using Iter<char>::target;

    virtual Iterator &operator=(char *ptr) {
        target = ptr;
        return *this;
    };

    explicit Iterator(char *ptr) : Iter<char>(ptr) {};

public:
    friend class String;

    Shared_ptr<Iter<char>> deep_copy() const override { return make_shared<String::Iterator>(*this); };

    Iterator(const Iterator &other) : Iter<char>(other.target) {};

    ~Iterator() = default;

    //该函数如果传入rIterator作为参数可能会有意外的结果
    Iterator &operator=(const Iterator &other) {
        target = other.target;
        return *this;
    };


    using Iter<char>::operator*;

    using Iter<char>::operator->;

    Iterator &operator++() & override {
        ++target;
        return *this;
    };

    Iterator operator++(int) &{ return String::Iterator(target++); };

    virtual Iterator &operator--() &{
        --target;
        return *this;
    };

    Iterator operator--(int) &{ return String::Iterator(target--); };

    Iterator operator+(Size size) { return Iterator(target + size); };

    Iterator operator-(Size size) { return Iterator(target - size); };

    Iterator &operator+=(Size size) {
        target += size;
        return *this;
    };

    Iterator &operator-=(Size size) {
        target -= size;
        return *this;
    };

    friend bool
    operator==(const Iterator &left, const Iterator &right) { return left.target == right.target; };

    friend bool
    operator!=(const Iterator &left, const Iterator &right) { return left.target != right.target; };

    friend bool
    operator>(const Iterator &left, const Iterator &right) { return left.target > right.target; };

    friend bool
    operator<(const Iterator &left, const Iterator &right) { return left.target < right.target; };

    friend bool
    operator>=(const Iterator &left, const Iterator &right) { return left.target >= right.target; };

    friend bool
    operator<=(const Iterator &left, const Iterator &right) { return left.target <= right.target; };

    friend long long
    operator-(const Iterator &left, const Iterator &right) { return left.target - right.target; };
};

//----------------------------------------------------------------------------------------------------------------------

class String::cIterator : public cIter<char> {
protected:
    using cIter<char>::target;

    virtual cIterator &operator=(char *ptr) {
        target = ptr;
        return *this;
    };

    explicit cIterator(char *ptr) : cIter<char>(ptr) {};

public:
    friend class String;

    Shared_ptr<cIter<char>> deep_copy() const override { return make_shared<String::cIterator>(*this); };

    cIterator(const cIterator &other) : cIter<char>(other.target) {};

    ~cIterator() = default;

    //该函数如果传入rIterator作为参数可能会有意外的结果
    cIterator &operator=(const cIterator &other) {
        target = other.target;
        return *this;
    };


    using cIter<char>::operator*;

    using cIter<char>::operator->;

    cIterator &operator++() & override {
        ++target;
        return *this;
    };

    cIterator operator++(int) &{ return String::cIterator(target++); };

    virtual cIterator &operator--() &{
        --target;
        return *this;
    };

    cIterator operator--(int) &{ return String::cIterator(target--); };

    cIterator operator+(Size size) { return cIterator(target + size); };

    cIterator operator-(Size size) { return cIterator(target - size); };

    cIterator &operator+=(Size size) {
        target += size;
        return *this;
    };

    cIterator &operator-=(Size size) {
        target -= size;
        return *this;
    };

    friend bool
    operator==(const cIterator &left, const cIterator &right) { return left.target == right.target; };

    friend bool
    operator!=(const cIterator &left, const cIterator &right) { return left.target != right.target; };

    friend bool
    operator>(const cIterator &left, const cIterator &right) { return left.target > right.target; };

    friend bool
    operator<(const cIterator &left, const cIterator &right) { return left.target < right.target; };

    friend bool
    operator>=(const cIterator &left, const cIterator &right) { return left.target >= right.target; };

    friend bool
    operator<=(const cIterator &left, const cIterator &right) { return left.target <= right.target; };

    friend long long
    operator-(const cIterator &left, const cIterator &right) { return left.target - right.target; };
};

//----------------------------------------------------------------------------------------------------------------------

class String::rIterator : public String::Iterator {
protected:
    using Iter<char>::target;

    rIterator &operator=(char *ptr) override {
        target = ptr;
        return *this;
    }

    explicit rIterator(char *ptr) : String::Iterator(ptr) {};

public:
    friend class String;

    Shared_ptr<Iter<char>> deep_copy() const override { return make_shared<String::rIterator>(*this); };

    rIterator(const rIterator &other) : String::Iterator(other.target) {};

    ~rIterator() = default;

    rIterator &operator=(const rIterator &other) {
        target = other.target;
        return *this;
    };

    using Iterator::operator*;

    using Iterator::operator->;

    rIterator &operator++() & override {
        --target;
        return *this;
    };

    rIterator operator++(int) &{ return String::rIterator(target--); };

    rIterator &operator--() & override {
        ++target;
        return *this;
    };

    rIterator operator--(int) &{ return String::rIterator(target++); };

    rIterator operator+(Size size) const { return String::rIterator(target - size); };

    rIterator operator-(Size size) const { return String::rIterator(target + size); };

    rIterator &operator+=(Size size) {
        target -= size;
        return *this;
    };

    rIterator &operator-=(Size size) {
        target += size;
        return *this;
    };

    friend bool
    operator==(const rIterator &left, const rIterator &right) { return left.target == right.target; };

    friend bool
    operator!=(const rIterator &left, const rIterator &right) { return left.target != right.target; };

    friend bool
    operator>(const rIterator &left, const rIterator &right) { return left.target < right.target; };

    friend bool
    operator<(const rIterator &left, const rIterator &right) { return left.target > right.target; };

    friend bool
    operator>=(const rIterator &left, const rIterator &right) { return left.target <= right.target; };

    friend bool
    operator<=(const rIterator &left, const rIterator &right) { return left.target >= right.target; };

    friend long long
    operator-(const rIterator &left, const rIterator &right) { return right.target - left.target; };
};

//----------------------------------------------------------------------------------------------------------------------

class String::crIterator : public String::cIterator {
protected:
    using cIter<char>::target;

    crIterator &operator=(char *ptr) override {
        target = ptr;
        return *this;
    };

    explicit crIterator(char *ptr) : String::cIterator(ptr) {};

public:
    friend class String;

    Shared_ptr<cIter<char>> deep_copy() const override { return make_shared<String::crIterator>(*this); };

    crIterator(const crIterator &other) : String::cIterator(other.target) {};

    ~crIterator() = default;

    crIterator &operator=(const crIterator &other) {
        target = other.target;
        return *this;
    };

    using cIterator::operator*;

    using cIterator::operator->;

    crIterator &operator++() & override {
        --target;
        return *this;
    };

    crIterator operator++(int) &{ return String::crIterator(target--); };

    crIterator &operator--() & override {
        ++target;
        return *this;
    };

    crIterator operator--(int) &{ return String::crIterator(target++); };

    crIterator operator+(Size size) const { return String::crIterator(target - size); };

    crIterator operator-(Size size) const { return String::crIterator(target + size); };

    crIterator &operator+=(Size size) {
        target -= size;
        return *this;
    };

    crIterator &operator-=(Size size) {
        target += size;
        return *this;
    };

    friend bool
    operator==(const crIterator &left, const crIterator &right) { return left.target == right.target; };

    friend bool
    operator!=(const crIterator &left, const crIterator &right) { return left.target != right.target; };

    friend bool
    operator>(const crIterator &left, const crIterator &right) { return left.target < right.target; };

    friend bool
    operator<(const crIterator &left, const crIterator &right) { return left.target > right.target; };

    friend bool
    operator>=(const crIterator &left, const crIterator &right) { return left.target <= right.target; };

    friend bool
    operator<=(const crIterator &left, const crIterator &right) { return left.target >= right.target; };

    friend long long
    operator-(const crIterator &left, const crIterator &right) { return right.target - left.target; };
};

//----------------------------------------------------------------------------------------------------------------------


String::String(Size size, char target) : size_(size), val_begin(Allocate_n<char>(size + size / 5 + 1)) {
    val_end = val_begin + size;
    store_end = val_begin + size + size / 5 + 1;
    auto temp = val_begin;
    for (int i = 0; i < size; ++i) {
        *temp = target;
        ++temp;
    }
    *store_end = '\0';
}

String::String(const Iter<char> &begin, const Iter<char> &end) {
    auto temp(begin.deep_copy());
    while (*temp != end) ++(*temp), ++size_;
    temp = begin.deep_copy();
    val_begin = Allocate_n<char>(size_ + 1);
    store_end = val_begin + size_ + 1;
    val_end = val_begin + size_;
    while (*temp != end) {
        *val_begin = **temp;
        ++val_begin, ++(*temp);
    }
    val_begin = val_end - size_;
    *val_end = '\0';
}

String::String(const cIter<char> &begin, const cIter<char> &end) {
    auto temp(begin.deep_copy());
    while (*temp != end) ++(*temp), ++size_;
    temp = begin.deep_copy();
    val_begin = Allocate_n<char>(size_ + 1);
    store_end = val_begin + size_ + 1;
    val_end = val_begin + size_;
    while (*temp != end) {
        *val_begin = **temp;
        ++val_begin, ++(*temp);
    }
    val_begin = val_end - size_;
    *val_end = '\0';
}

String::String(const String &other) : size_(other.size_), val_begin(Allocate_n<char>(other.capacity())) {
    val_end = val_begin + size_;
    store_end = val_begin + other.capacity();
    auto l = val_begin, r = other.val_begin;
    for (int i = 0; i < size_; ++i) {
        *l = *r;
        ++l, ++r;
    }
    *val_end = '\0';
}

String::String(String &&other) noexcept: size_(other.size_), val_begin(other.val_begin),
                                         val_end(other.val_end), store_end(other.store_end) {
    other.size_ = 0;
    other.val_begin = other.val_end = other.store_end = nullptr;
}

void String::reallocate(Size size) {
    auto the_new = Allocate_n<char>(size), the_old = val_begin;
    store_end = the_new + size_;
    for (int i = 0; i < size_; ++i) *the_new++ = *the_old++;
    val_begin = store_end - size;
    val_end = val_begin + size_;
    Deallocate_n(the_old - size_);
    *val_end = '\0';
}

void String::assign(const Iter<char> &begin, const Iter<char> &end) {
    Deallocate_n(val_begin);
    auto temp = begin.deep_copy();
    Size count = 0;
    while (*temp != end) ++(*temp), ++count;
    temp = begin.deep_copy();
    auto t = Allocate_n<char>(count + 1);
    while (*temp != end) {
        *t = **temp;
        ++t, ++(*temp);
    }
    store_end = t + 1;
    val_end = t;
    val_begin = t - count;
    size_ = count;
    *val_end = '\0';
}

void String::assign(const cIter<char> &begin, const cIter<char> &end) {
    Deallocate_n(val_begin);
    auto temp = begin.deep_copy();
    Size count = 0;
    while (*temp != end) ++(*temp), ++count;
    temp = begin.deep_copy();
    auto t = Allocate_n<char>(count + 1);
    while (*temp != end) {
        *t = **temp;
        ++t, ++(*temp);
    }
    store_end = t + 1;
    val_end = t;
    val_begin = t - count;
    size_ = count;
    *val_end = '\0';
}

void String::append(char t) {
    if (capacity() <= size_ + 1)
        reallocate(size_ + 1 > size_ + size_ / 5 ? size_ + 2 : size_ + size_ / 5 + 1);
    *(val_begin + size_) = t;
    ++size_;
    *++val_end = '\0';
}

void String::append(const char *target) {
    auto temp = target;
    Size count = 0;
    while (*temp != '\0') ++temp, ++count;
    if (!count) return;
    temp = target;
    if (capacity() - size_ - 1 < count) reallocate(capacity() + count + 1);
    size_ += count;
    while (*temp != '\0') {
        *val_end = *temp;
        ++val_end, ++temp;
    }
    *val_end = '\0';
}

void String::append(const String &target) {
    if (!target.size_) return;
    if (capacity() - size_ - 1 < target.size_) reallocate(capacity() + target.size_ + 1);
    auto temp = target.val_begin;
    size_ += target.size_;
    while (*temp != '\0') {
        *val_end = *temp;
        ++val_end, ++temp;
    }
    *val_end = '\0';
}

void String::append(const Iter<char> &begin, const Iter<char> &end) {
    auto temp = begin.deep_copy();
    Size count = 0;
    while (*temp != end) ++(*temp), ++count;
    if (capacity() - size_ - 1 < count) reallocate(capacity() + count + 1);
    temp = begin.deep_copy();
    size_ += count;
    while (*temp != end) {
        *val_end = **temp;
        ++val_end, ++(*temp);
    }
    *val_end = '\0';
}

void String::append(const cIter<char> &begin, const cIter<char> &end) {
    auto temp = begin.deep_copy();
    Size count = 0;
    while (*temp != end) ++(*temp), ++count;
    if (capacity() - size_ - 1 < count) reallocate(capacity() + count + 1);
    temp = begin.deep_copy();
    size_ += count;
    while (*temp != end) {
        *val_end = **temp;
        ++val_end, ++(*temp);
    }
    *val_end = '\0';
}

void String::push_back(char t) {
    append(t);
}

void String::push_back(const char *target) {
    append(target);
}

void String::push_back(const String &target) {
    append(target);
}

void String::push_back(const Iter<char> &begin, const Iter<char> &end) {
    append(begin, end);
}

void String::push_back(const cIter<char> &begin, const cIter<char> &end) {
    append(begin, end);
}

void String::pop_back() {
    --val_end;
    *val_end = '\0';
}
