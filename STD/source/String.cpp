//
// Created by 86152 on 2023/9/9.
//

#include "../String.hpp"

using namespace STD;


//----------------------------------------------------------------------------------------------------------------------


void String::reallocate(Size size) {
    auto the_new = Allocate_n<char>(size), the_old = val_begin;
    store_end = the_new + size_;
    for (int i = 0; i < size_; ++i) *the_new++ = *the_old++;
    val_begin = store_end - size;
    val_end = val_begin + size_;
    Deallocate_n(the_old - size_);
    *val_end = '\0';
}

Size String::calculateLength(const char *target) {
    const char *temp = target;
    Size len = 0;
    while (*temp != '\0') ++temp, ++len;
    return len;
}

String::String(Size size, char target) : size_(size), val_begin(Allocate_n<char>(size + size / 5 + 1)) {
    val_end = val_begin + size;
    store_end = val_begin + size + size / 5 + 1;
    auto temp = val_begin;
    for (int i = 0; i < size; ++i) {
        *temp = target;
        ++temp;
    }
    *val_end = '\0';
}

String::String(const char *target, Size len) : size_(len), val_begin(Allocate_n<char>(len + 1)),
                                               store_end(val_begin + len + 1) {
    val_end = val_begin;
    for (int i = 0; i < len; ++i) {
        *val_end = *target;
        ++val_end, ++target;
    }
    *val_end = '\0';
}

String::String(const char *target) : String(target, calculateLength(target)) {}

String::String(const String &other, Size pos) : String(other.c_str() + pos) {}

String::String(const String &other, Size pos, Size len) : String(other.c_str() + pos, len) {}

String::String(const String &other) : String(other.c_str(), other.size_) {}

String::String(String &&other) noexcept: size_(other.size_), val_begin(other.val_begin),
                                         val_end(other.val_end), store_end(other.store_end) {
    other.size_ = 0;
    other.val_begin = other.val_end = other.store_end = nullptr;
}

String::String(const Iter<char> &begin, const Iter<char> &end) : size_(STD::calculateLength(begin, end)) {
    auto temp(begin.deep_copy());
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

String::String(const cIter<char> &begin, const cIter<char> &end) : size_(STD::calculateLength(begin, end)) {
    auto temp(begin.deep_copy());
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

String& String::assign(const Iter<char> &begin, const Iter<char> &end) {
    Deallocate_n(val_begin);
    auto temp = begin.deep_copy();
    Size count = STD::calculateLength(begin, end);
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
    return *this;
}

String& String::assign(const cIter<char> &begin, const cIter<char> &end) {
    Deallocate_n(val_begin);
    auto temp = begin.deep_copy();
    Size count = STD::calculateLength(begin, end);
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
    return *this;
}

void String::append(char t) {
    if (capacity() <= size_ + 1)
        reallocate(size_ + 1 > size_ + size_ / 5 ? size_ + 2 : size_ + size_ / 5 + 1);
    *(val_begin + size_) = t;
    ++size_;
    *++val_end = '\0';
}

void String::append(const char *target) {
    Size count = calculateLength(target);
    if (!count) return;
    auto temp = target;
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
    Size count = STD::calculateLength(begin, end);
    if (capacity() - size_ - 1 < count) reallocate(capacity() + count + 1);
    size_ += count;
    while (*temp != end) {
        *val_end = **temp;
        ++val_end, ++(*temp);
    }
    *val_end = '\0';
}

void String::append(const cIter<char> &begin, const cIter<char> &end) {
    auto temp = begin.deep_copy();
    Size count = STD::calculateLength(begin, end);
    if (capacity() - size_ - 1 < count) reallocate(capacity() + count + 1);
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

String::Iterator String::insert(Size pos, char t, Size size) {
    if (!size) return String::Iterator(val_begin + pos);
    if (pos >= size_) throw outOfRange("You passed an out-of-range value in the 'insert' function");
    if (capacity() - size_ - 1 < size) {
        Size record = capacity() + size;
        auto the_new = Allocate_n<char>(record), the_old = val_begin;
        store_end = the_new + record;
        for (int i = 0; i < pos; ++i) {
            *the_new = *val_begin;
            ++the_new, ++val_begin;
        }
        for (int i = 0; i < size; ++i) *the_new++ = t;
        while (val_begin != val_end) {
            *the_new = *val_begin;
            ++the_new, ++val_begin;
        }
        size_ += size;
        val_end = the_new;
        val_begin = val_end - size_;
        Deallocate_n(the_old);
        *val_end = '\0';
    } else {
        auto temp1 = val_end - 1, temp2 = val_end + size - 1, target_end = val_begin + pos;
        while (temp1 >= target_end) {
            *temp2 = *temp1;
            --temp1, --temp2;
        }
        for (int i = 0; i < size; ++i) *target_end++ = t;
        size_ += size;
        val_end = val_begin + size_;
        *val_end = '\0';
    }
    return String::Iterator(val_begin + pos);
}

String::Iterator String::insert(Size pos, const char *target, Size target_len) {
    if (!target_len) return String::Iterator(val_begin + pos);
    if (pos >= size_) throw outOfRange("You passed an out-of-range value in the 'insert' function");
    if (capacity() - size_ - 1 < target_len) {
        Size record = capacity() + target_len;
        auto the_new = Allocate_n<char>(record), the_old = val_begin;
        store_end = the_new + record;
        for (int i = 0; i < pos; ++i) {
            *the_new = *val_begin;
            ++the_new, ++val_begin;
        }
        for (int i = 0; i < target_len; ++i) {
            *the_new = *target;
            ++the_new, ++target;
        }
        while (val_begin != val_end) {
            *the_new = *val_begin;
            ++the_new, ++val_begin;
        }
        size_ += target_len;
        val_end = the_new;
        val_begin = val_end - size_;
        Deallocate_n(the_old);
        *val_end = '\0';
    } else {
        auto temp1 = val_end - 1, temp2 = val_end + target_len - 1, target_end = val_begin + pos;
        while (temp1 >= target_end) {
            *temp2 = *temp1;
            --temp1, --temp2;
        }
        for (int i = 0; i < target_len; ++i) {
            *target_end = *target;
            ++target_end, ++target;
        }
        size_ += target_len;
        val_end = val_begin + size_;
        *val_end = '\0';
    }
    return String::Iterator(val_begin + pos);
}

String::Iterator String::insert(Size pos, const char *target, Size target_pos, Size target_len) {
    return insert(pos, target + target_pos, target_len);
}

String::Iterator String::insert(Size pos, const Iter<char> &begin, const Iter<char> &end) {
    if (begin == end) return String::Iterator(val_begin + pos);
    auto target = begin.deep_copy();
    if (pos >= size_) throw outOfRange("You passed an out-of-range value in the 'insert' function");
    Size target_len = STD::calculateLength(begin, end);
    if (capacity() - size_ - 1 < target_len) {
        Size record = capacity() + target_len;
        auto the_new = Allocate_n<char>(record), the_old = val_begin;
        store_end = the_new + record;
        for (int i = 0; i < pos; ++i) {
            *the_new = *val_begin;
            ++the_new, ++val_begin;
        }
        for (int i = 0; i < target_len; ++i) {
            *the_new = **target;
            ++the_new, ++(*target);
        }
        while (val_begin != val_end) {
            *the_new = *val_begin;
            ++the_new, ++val_begin;
        }
        size_ += target_len;
        val_end = the_new;
        val_begin = val_end - size_;
        Deallocate_n(the_old);
        *val_end = '\0';
    } else {
        auto temp1 = val_end - 1, temp2 = val_end + target_len - 1, target_end = val_begin + pos;
        while (temp1 >= target_end) {
            *temp2 = *temp1;
            --temp1, --temp2;
        }
        for (int i = 0; i < target_len; ++i) {
            *target_end = **target;
            ++target_end, ++(*target);
        }
        size_ += target_len;
        val_end = val_begin + size_;
        *val_end = '\0';
    }
    return String::Iterator(val_begin + pos);
}

String::cIterator String::insert(Size pos, const cIter<char> &begin, const cIter<char> &end) {
    if (begin == end) return String::cIterator(val_begin + pos);
    auto target = begin.deep_copy();
    if (pos >= size_) throw outOfRange("You passed an out-of-range value in the 'insert' function");
    Size target_len = STD::calculateLength(begin, end);
    if (capacity() - size_ - 1 < target_len) {
        Size record = capacity() + target_len;
        auto the_new = Allocate_n<char>(record), the_old = val_begin;
        store_end = the_new + record;
        for (int i = 0; i < pos; ++i) {
            *the_new = *val_begin;
            ++the_new, ++val_begin;
        }
        for (int i = 0; i < target_len; ++i) {
            *the_new = **target;
            ++the_new, ++(*target);
        }
        while (val_begin != val_end) {
            *the_new = *val_begin;
            ++the_new, ++val_begin;
        }
        size_ += target_len;
        val_end = the_new;
        val_begin = val_end - size_;
        Deallocate_n(the_old);
        *val_end = '\0';
    } else {
        auto temp1 = val_end - 1, temp2 = val_end + target_len - 1, target_end = val_begin + pos;
        while (temp1 >= target_end) {
            *temp2 = *temp1;
            --temp1, --temp2;
        }
        for (int i = 0; i < target_len; ++i) {
            *target_end = **target;
            ++target_end, ++(*target);
        }
        size_ += target_len;
        val_end = val_begin + size_;
        *val_end = '\0';
    }
    return String::cIterator(val_begin + pos);
}

String::Iterator String::insert(const String::Iterator &iter, char t, Size size) {
    return insert(iter.target - val_begin, t, size);
}

String::Iterator String::insert(const String::Iterator &iter, const char *target, Size target_len) {
    return insert(iter.target - val_begin, target, target_len);
}

String::Iterator String::insert(const String::Iterator &iter, const char *target, Size target_pos, Size target_len) {
    return insert(iter.target - val_begin, target, target_len);
}

String::Iterator String::insert(const String::Iterator &iter, const Iter<char> &begin, const Iter<char> &end) {
    return insert(iter.target - val_begin, begin, end);
}

String::cIterator String::insert(const String::Iterator &iter, const cIter<char> &begin, const cIter<char> &end) {
    return insert(iter.target - val_begin, begin, end);
}


