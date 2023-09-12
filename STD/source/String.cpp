//
// Created by 86152 on 2023/9/9.
//

#include "../String.hpp"

using namespace STD;


//----------------------------------------------------------------------------------------------------------------------


void String::reallocate(Size size) {
    auto the_new = Allocate_n<char>(size), the_old = val_begin;
    store_end = the_new + size;
    for (int i = 0; i < size_; ++i) *the_new++ = *the_old++;
    val_begin = store_end - size;
    val_end = val_begin + size_;
    Deallocate_n(the_old - size_);
    *val_end = '\0';
}

String::String() : val_begin(Allocate_n<char>(1)), val_end(val_begin), store_end(val_begin + 1) {
    *val_begin = '\0';
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

String::String(const char *target) : String(target, STD::calculateLength(target)) {}

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

String String::sub_str(Size begin, Size len) {
    return {val_begin + begin, len};
}

String &String::assign(const Iter<char> &begin, const Iter<char> &end) {
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

String &String::assign(const cIter<char> &begin, const cIter<char> &end) {
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

void String::clear(bool whether_to_release) {
    if (whether_to_release) {
        Deallocate_n(val_begin);
        val_begin = val_end = store_end = nullptr;
    } else {
        char *temp = val_begin;
        while (temp != val_end) *(temp++) = '\0';
        val_end = val_begin;
    }
    size_ = 0;
}

void String::append(char t) {
    if (capacity() <= size_ + 1)
        reallocate(size_ + 1 > size_ + size_ / 5 ? size_ + 2 : size_ + size_ / 5 + 1);
    *(val_begin + size_) = t;
    ++size_;
    *++val_end = '\0';
}

void String::append(const char *target, Size len) {
    auto temp = target;
    if (capacity() - size_ - 1 < len) reallocate(capacity() + len + 1);
    size_ += len;
    for (int i = 0; i < len; ++i) {
        *val_end = *temp;
        ++val_end, ++temp;
    }
    *val_end = '\0';
}

void String::append(const char *target) {
    append(target, 0, STD::calculateLength(target));
}

void String::append(const char *target, Size pos, Size len) {
    append(target + pos, len);
}

void String::append(const String &target) {
    if (!target.size_) return;
    append(target.c_str());
}

void String::append(const String &target, Size pos) {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::append or String::push_back' function");
    append(target.c_str() + pos);
}

void String::append(const String &target, Size pos, Size len) {
    if (pos + len > size_)
        throw outOfRange("You selected an out-of-range value in the 'String::append or String::push_back' function");
    append(target.c_str() + pos, len);
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

void String::push_back(const char *target, Size len) {
    append(target, len);
}

void String::push_back(const char *target, Size pos, Size len) {
    append(target, pos, len);
}

void String::push_back(const String &target) {
    append(target);
}

void String::push_back(const String &target, Size pos) {
    append(target, pos);
}

void String::push_back(const String &target, Size pos, Size len) {
    append(target, pos, len);
}


void String::push_back(const Iter<char> &begin, const Iter<char> &end) {
    append(begin, end);
}

void String::push_back(const cIter<char> &begin, const cIter<char> &end) {
    append(begin, end);
}

String::Iterator String::insert(Size pos, char t, Size size) {
    if (!size) return String::Iterator(val_begin + pos);
    if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
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
    if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
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
    if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
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
    if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
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

String::Iterator String::erase(Size pos, Size size) {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::erase' function");
    if (!size) return String::Iterator(val_begin + pos);
    size = pos + size > size_ ? size_ - pos : size;
    auto temp1 = val_begin + pos, temp2 = val_begin + size;
    while (temp2 < val_end) {
        *temp1 = *temp2;
        ++temp1, ++temp2;
    }
    size_ -= size;
    val_end -= size;
    *val_end = '\0';
    return String::Iterator(val_begin + pos);
}

String::Iterator String::erase(const String::Iterator &iter) {
    return erase(iter.target - val_begin, 1);
}

String::cIterator String::erase(const String::cIterator &iter) {
    return String::cIterator(erase(iter.target - val_begin, 1).target);
}

String::Iterator String::erase(const String::Iterator &begin, const String::Iterator &end) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::erase' function");
    return erase(begin.target - val_begin, end - begin);
}

String::cIterator String::erase(const String::cIterator &begin, const String::cIterator &end) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::erase' function");
    return String::cIterator(erase(begin.target - val_begin, end - begin).target);
}

String &String::replace(Size pos, Size len, Size n, char t) {
    if (pos + len > size_) throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    if (capacity() - size_ + len - 1 < n) {
        auto the_new = Allocate_n<char>(capacity() + n - len), the_old = val_begin;
        store_end = the_new + capacity() + n - len;
        for (int i = 0; i < pos; ++i) {
            *the_new = *val_begin;
            ++the_new, ++val_end;
        }
        for (int i = 0; i < n; ++i) {
            *the_new = t;
            ++the_new;
        }
        val_begin += len;
        while (val_begin < val_end) {
            *the_new = *val_begin;
            ++the_new, ++val_end;
        }
        Deallocate_n(the_old);
        size_ += n - len;
        val_end = the_new;
        val_begin = val_end - size_;
    } else {
        if (len > n) {
            auto temp1 = val_begin + pos, temp2 = val_begin + pos + len;
            for (int i = 0; i < n; ++i) {
                *temp1 = t;
                ++temp1;
            }
            while (temp2 < val_end) {
                *temp1 = *temp2;
                ++temp1, ++temp2;
            }
            size_ -= len - n;
            val_end = temp1;
        } else if (len < n) {
            auto temp1 = val_end - 1, temp2 = val_end - 1 + n - len, target_end = val_begin + pos + len;
            size_ += n - len;
            val_end = temp2 + 1;
            while (temp1 >= target_end) {
                *temp2 = *temp1;
                --temp1, --temp2;
            }
            target_end -= len;
            for (int i = 0; i < n; ++i) {
                *target_end = t;
                ++target_end;
            }
        } else {
            auto temp = val_begin + pos;
            for (int i = 0; i < n; ++i) {
                *temp = t;
                ++temp;
            }
        }
    }
    *val_end = '\0';
    return *this;
}

String &String::replace(Size pos, Size len, const char *target, Size target_len) {
    if (pos + len > size_) throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    if (capacity() - size_ + len - 1 < target_len) {
        auto the_new = Allocate_n<char>(capacity() + target_len - len), the_old = val_begin;
        store_end = the_new + capacity() + target_len - len;
        for (int i = 0; i < pos; ++i) {
            *the_new = *val_begin;
            ++the_new, ++val_end;
        }
        for (int i = 0; i < target_len; ++i) {
            *the_new = *target;
            ++the_new, ++target;
        }
        val_begin += len;
        while (val_begin < val_end) {
            *the_new = *val_begin;
            ++the_new, ++val_end;
        }
        Deallocate_n(the_old);
        size_ += target_len - len;
        val_end = the_new;
        val_begin = val_end - size_;
    } else {
        if (len > target_len) {
            auto temp1 = val_begin + pos, temp2 = val_begin + pos + len;
            for (int i = 0; i < target_len; ++i) {
                *temp1 = *target;
                ++temp1, ++target;
            }
            while (temp2 < val_end) {
                *temp1 = *temp2;
                ++temp1, ++temp2;
            }
            size_ -= len - target_len;
            val_end = temp1;
        } else if (len < target_len) {
            auto temp1 = val_end - 1, temp2 = val_end - 1 + target_len - len, target_end = val_begin + pos + len;
            size_ += target_len - len;
            val_end = temp2 + 1;
            while (temp1 >= target_end) {
                *temp2 = *temp1;
                --temp1, --temp2;
            }
            target_end -= len;
            for (int i = 0; i < target_len; ++i) {
                *target_end = *target;
                ++target_end, ++target;
            }
        } else {
            auto temp = val_begin + pos;
            for (int i = 0; i < target_len; ++i) {
                *temp = *target;
                ++temp, ++target;
            }
        }
    }
    *val_end = '\0';
    return *this;
}

String &String::replace(Size pos, Size len, const char *target) {
    return replace(pos, len, target, STD::calculateLength(target));
}

String &String::replace(Size pos, Size len, const String &target) {
    return replace(pos, len, target.c_str());
}

String &String::replace(Size pos, Size len, const String &target, Size target_pos, Size target_len) {
    if (target_pos + target_len > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(pos, len, target.c_str() + target_pos, target_len);
}

String &String::replace(const String::Iterator &begin, const String::Iterator &end, Size n, char t) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, n, t);
}

String &
String::replace(const String::Iterator &begin, const String::Iterator &end, const char *target, Size target_len) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, target, target_len);
}

String &String::replace(const String::Iterator &begin, const String::Iterator &end, const char *target) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, target, STD::calculateLength(target));
}

String &String::replace(const String::Iterator &begin, const String::Iterator &end, const String &target) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, target);
}

String &String::replace(const String::Iterator &begin, const String::Iterator &end, const String &target,
                        Size target_pos, Size target_len) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, target.c_str() + target_pos, target_len);
}

String &String::replace(const String::cIterator &begin, const String::cIterator &end, Size n, char t) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, n, t);
}

String &
String::replace(const String::cIterator &begin, const String::cIterator &end, const char *target, Size target_len) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, target, target_len);
}

String &String::replace(const String::cIterator &begin, const String::cIterator &end, const char *target) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, target, STD::calculateLength(target));
}

String &String::replace(const String::cIterator &begin, const String::cIterator &end, const String &target) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, target);
}

String &String::replace(const String::cIterator &begin, const String::cIterator &end, const String &target,
                        Size target_pos, Size target_len) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, target.c_str() + target_pos, target_len);
}

String &String::operator=(const String &other) {
    Deallocate_n(val_begin);
    val_end = val_begin = Allocate_n<char>(other.size_ + 1);
    size_ = other.size_;
    store_end = val_begin + other.size_ + 1;
    auto temp = other.val_begin;
    for (int i = 0; i < other.size_; ++i) {
        *val_end = *temp;
        ++val_end, ++temp;
    }
    *val_end = '\0';
    return *this;
}

String &String::operator=(String &&other) noexcept {
    Deallocate_n(val_begin);
    size_ = other.size_;
    val_begin = other.val_begin;
    val_end = other.val_end;
    store_end = other.store_end;
    other.size_ = 0;
    other.val_begin = other.val_end = other.store_end = nullptr;
    return *this;
}

String STD::operator+(String &left, const String &right) {
    left.append(right);
    return left;
}

String &STD::String::operator+=(const String &other) {
    append(other);
    return *this;
}

std::ostream &STD::operator<<(std::ostream &out, const String &target) {
    out << target.val_begin;
    return out;
}

std::istream &STD::operator>>(std::istream &in, String &target) {
    auto temp = Allocate_n<char>(256);
    in >> temp;
    target.append(temp);
    Deallocate_n(temp);
    return in;
}

bool STD::operator==(const String &left, const String &right) {
    if (left.size_ != right.size_) return false;
    auto l = left.val_begin, r = right.val_begin;
    for (int i = 0; i < left.size_; ++i) {
        if (*l != *r) return false;
        ++l, ++r;
    }
    return true;
}

bool STD::operator!=(const String &left, const String &right) {
    return !(left == right);
}

bool STD::operator<(const String &left, const String &right) {
    auto l = left.val_begin, r = right.val_begin;
    while (l < left.val_end && r < right.val_end) {
        if (*l == *r) ++l, ++r;
        else return *l < *r;
    }
    return l == left.val_end && r != right.val_end;
}

bool STD::operator<=(const String &left, const String &right) {
    auto l = left.val_begin, r = right.val_begin;
    while (l < left.val_end && r < right.val_end) {
        if (*l == *r) ++l, ++r;
        else return *l < *r;
    }
    return l == left.val_end;
}

bool STD::operator>(const String &left, const String &right) {
    auto l = left.val_begin, r = right.val_begin;
    while (l < left.val_end && r < right.val_end) {
        if (*l == *r) ++l, ++r;
        else return *l > *r;
    }
    return l != left.val_end && r == right.val_end;
}

bool STD::operator>=(const String &left, const String &right) {
    auto l = left.val_begin, r = right.val_begin;
    while (l < left.val_end && r < right.val_end) {
        if (*l == *r) ++l, ++r;
        else return *l > *r;
    }
    return r == right.val_end;
}

Size String::find(char t, Size pos) const {
    auto temp = val_begin + pos;
    while (temp != val_end) {
        if (*temp == t) return temp - val_begin;
        ++temp;
    }
    return String::Npos;
}

Size String::find(const char *target, Size pos) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::find' function");
    auto temp = Boyer_Moore(target, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::find(const char *target, Size pos, Size size) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::find' function");
    auto temp = Boyer_Moore(target, size, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::find(const String &target, Size pos) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::find' function");
    auto temp = Boyer_Moore(target.val_begin, target.size_, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::find(const String &target, Size pos, Size size) const {
    if (pos >= size_ || size > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find' function");
    auto temp = Boyer_Moore(target.val_begin, size, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::rfind(char t, Size pos) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = val_end - 1;
    while (temp >= val_begin) {
        if (*temp == t) return temp - val_begin;
        --temp;
    }
    return Npos;
}

Size String::rfind(const char *target, Size pos) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = rBoyer_Moore(target, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::rfind(const char *target, Size pos, Size size) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = rBoyer_Moore(target, size, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::rfind(const String &target, Size pos) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = rBoyer_Moore(target.val_begin, target.size_, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::rfind(const String &target, Size pos, Size size) const {
    if (pos >= size_ || size > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = rBoyer_Moore(target.val_begin, size, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::find_first_of(const char *target, Size pos, Size size) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::find_first_of' function");
    bool store[256];
    Memset(store, false, 256);
    for (int i = 0; i < size; ++i) store[target[i]] = true;
    auto temp = val_begin + pos;
    while (temp < val_end) {
        if (store[*temp]) return temp - val_begin;
        ++temp;
    }
    return Npos;
}

Size String::find_first_of(const char *target, Size pos) const {
    return find_first_of(target, pos, calculateLength(target));
}

Size String::find_first_of(const String &target, Size pos) const {
    return find_first_of(target.val_begin, pos, target.size_);
}

Size String::find_first_of(const String &target, Size pos, Size size) const {
    if (size > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find_first_of' function");
    return find_first_of(target.val_begin, pos, size);
}

Size String::find_last_of(const char *target, Size pos, Size size) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::find_last_of' function");
    bool store[256];
    Memset(store, false, 256);
    for (int i = 0; i < size; ++i) store[target[i]] = true;
    auto temp = val_end - 1;
    while (temp >= val_begin + pos) {
        if (store[*temp]) return temp - val_begin;
        --temp;
    }
    return Npos;
}

Size String::find_last_of(const char *target, Size pos) const {
    return find_last_of(target, pos, calculateLength(target));
}

Size String::find_last_of(const String &target, Size pos) const {
    return find_first_of(target.val_begin, pos, target.size_);
}

Size String::find_last_of(const String &target, Size pos, Size size) const {
    if (size > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find_last_of' function");
    return find_first_of(target.val_begin, pos, size);
}

Size String::find_first_not_of(const char *target, Size pos, Size size) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find_first_not_of' function");
    bool store[256];
    Memset(store, true, 256);
    for (int i = 0; i < size; ++i) store[target[i]] = false;
    auto temp = val_begin + pos;
    while (temp < val_end) {
        if (store[*temp]) return temp - val_begin;
        ++temp;
    }
    return Npos;
}

Size String::find_first_not_of(const char *target, Size pos) const {
    return find_first_not_of(target, pos, calculateLength(target));
}

Size String::find_first_not_of(const String &target, Size pos) const {
    return find_first_not_of(target.val_begin, pos, target.size_);
}

Size String::find_first_not_of(const String &target, Size pos, Size size) const {
    if (size > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find_first_not_of' function");
    return find_first_not_of(target.val_begin, pos, size);
}

Size String::find_last_not_of(const char *target, Size pos, Size size) const {
    if (pos >= size_) throw outOfRange("You selected an out-of-range value in the 'String::find_last_not_of' function");
    bool store[256];
    Memset(store, true, 256);
    for (int i = 0; i < size; ++i) store[target[i]] = false;
    auto temp = val_end - 1;
    while (temp >= val_begin + pos) {
        if (store[*temp]) return temp - val_begin;
        --temp;
    }
    return Npos;
}

Size String::find_last_not_of(const char *target, Size pos) const {
    return find_last_not_of(target, pos, calculateLength(target));
}

Size String::find_last_not_of(const String &target, Size pos) const {
    return find_last_not_of(target.val_begin, pos, target.size_);
}

Size String::find_last_not_of(const String &target, Size pos, Size size) const {
    if (size > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find_last_not_of' function");
    return find_last_not_of(target.val_begin, pos, size);
}

int String::compare(Size pos1, Size n1, const char *target, Size n2) const {
    if (pos1 + n1 > size_) throw outOfRange("You selected an out-of-range value in the 'String::compare' function");
    const char *temp = val_begin + pos1, *end1 = temp + n1, *end2 = target + n2;
    while (temp != end1 && target != end2) {
        if (*temp != *target) return *temp - *target;
        ++temp, ++target;
    }
    if (n1 == n2) return 0;
    return temp == end1 ? -(*target) : *temp;
}

int String::compare(Size pos1, Size n1, const char *target) const {
    return compare(pos1, n1, target, calculateLength(target));
}

int String::compare(const char *target) const {
    return compare(0, size_, target, calculateLength(target));
}

int String::compare(Size pos1, Size n1, const String &target, Size pos2, Size n2) const {
    if (pos2 + n2 > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::compare' function");
    return compare(pos1, n1, target.val_begin + pos2, n2);
}

int String::compare(Size pos1, Size n1, const String &target) const {
    return compare(pos1, n1, target.val_begin, target.size_);
}

int String::compare(const String &target) const {
    return compare(0, size_, target.val_begin, target.size_);
}

//----------------------------------------------------------------------------------------------------------------------

