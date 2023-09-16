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

void String::forward(Size pos_from, Size pos_to) {
    auto temp1 = val_begin + pos_to, temp2 = val_begin + pos_from;
    while (temp1 < val_end) {
        *temp1 = *temp2;
        ++temp1, ++temp2;
    }
    size_ -= pos_from - pos_to;
    val_end = temp2;
    *val_end = '\0';
}

char *String::backward(Size pos_from, Size pos_to) {
    Size new_size = size_ + pos_to - pos_from + 1;
    if (new_size > capacity()) {
        auto the_new = Allocate_n<char>(new_size), temp = the_new;
        store_end = the_new + new_size;
        for (int i = 0; i < pos_from; ++i) {
            *temp = *val_begin;
            ++temp, ++val_begin;
        }
        temp = the_new + pos_to;
        while (val_begin < val_end) {
            *temp = *val_begin;
            ++temp, ++val_begin;
        }
        val_begin = the_new;
        val_end = temp;
    } else {
        auto temp1 = val_end - 1, temp2 = val_end - 1 + pos_to - pos_from, target_end = val_begin + pos_from;
        val_end = temp2 + 1;
        while (temp1 >= target_end) {
            *temp2 = *temp1;
            --temp1, --temp2;
        }
    }
    size_ += pos_to - pos_from;
    *val_end = '\0';
    return val_begin + pos_from;
}

void String::fill_with(char *pos, char target, Size size) {
    for (int i = 0; i < size; ++i) {
        *pos = target;
        ++pos;
    }
}

void String::fill_with(char *pos, const char *target, Size size) {
    for (int i = 0; i < size; ++i) {
        *pos = *target;
        ++pos, ++target;
    }
}

void String::fill_with(char *pos, const std::initializer_list<char> &list) {
    for (auto &t: list) {
        *pos = t;
        ++pos;
    }
}

void String::fill_with(char *pos, const cIter<char> &begin, Size size) {
    auto temp = begin.deep_copy();
    for (int i = 0; i < size; ++i) {
        *pos = **temp;
        ++pos, ++(*temp);
    }
}

void String::rfill_with(char *pos, const char *target, Size size) {
    pos += size - 1;
    for (int i = 0; i < size; ++i) {
        *pos = *target;
        --pos, ++target;
    }
}

void String::rfill_with(char *pos, const std::initializer_list<char> &list) {
    pos += list.size() - 1;
    for (auto &t: list) {
        *pos = t;
        --pos;
    }
}

void String::rfill_with(char *pos, const cIter<char> &begin, Size size) {
    pos += size - 1;
    auto temp = begin.deep_copy();
    for (int i = 0; i < size; ++i) {
        *pos = **temp;
        --pos, ++(*temp);
    }
}

String::String() : val_begin(Allocate_n<char>(1)), val_end(val_begin), store_end(val_begin + 1) {
    *val_begin = '\0';
}

String::String(Size size, char target) : size_(size), val_begin(Allocate_n<char>(size + 1)) {
    val_end = val_begin + size;
    store_end = val_end + 1;
    fill_with(val_begin, target, size);
    *val_end = '\0';
}

String::String(const char *target, Size len) : size_(len), val_begin(Allocate_n<char>(len + 1)),
                                               store_end(val_begin + len + 1) {
    val_end = val_begin + len;
    fill_with(val_begin, target, len);
    *val_end = '\0';
}

String::String(const std::initializer_list<char> &list) : size_(list.size()), val_begin(Allocate_n<char>(size_ + 1)),
                                                          val_end(val_begin + list.size()),
                                                          store_end(val_begin + size_ + 1) {
    fill_with(val_begin, list);
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

String::String(const Iter<char> &begin, const Iter<char> &end) : String(*begin.to_const(), *end.to_const()) {}

String::String(const cIter<char> &begin, const cIter<char> &end) : size_(STD::calculateLength(begin, end)) {
    val_begin = Allocate_n<char>(size_ + 1);
    store_end = val_begin + size_ + 1;
    val_end = val_begin + size_;
    fill_with(val_begin, begin, size_);
    *val_end = '\0';
}

String String::sub_str(Size begin, Size len) {
    return {val_begin + begin, len};
}

String &String::assign(const Iter<char> &begin, const Iter<char> &end) {
    return assign(*begin.to_const(), *end.to_const());
}

String &String::assign(const cIter<char> &begin, const cIter<char> &end) {
    Deallocate_n(val_begin);
    size_ = STD::calculateLength(begin, end);
    val_begin = Allocate_n<char>(size_ + 1);
    val_end = val_begin + size_;
    store_end = val_end + 1;
    fill_with(val_begin, begin, size_);
    *val_end = '\0';
    return *this;
}

void String::clear(bool whether_to_release) {
    if (whether_to_release) {
        Deallocate_n(val_begin);
        val_begin = Allocate_n<char>(1);
        val_end = val_begin;
        store_end = val_end + 1;
    } else {
        val_end = val_begin;
    }
    size_ = 0;
    *val_end = '\0';
}

void String::append(char t, Size len) {
    if (capacity() <= size_ + len)
        reallocate(size_ + len > size_ + size_ / 5 ? size_ + len + 1 : size_ + size_ / 5 + 1);
    fill_with(val_end, t, len);
    size_ += len;
    val_end += len;
    *val_end = '\0';
}

void String::append(const char *target, Size len) {
    auto temp = target;
    if (capacity() - size_ - 1 < len) reallocate(capacity() + len + 1);
    fill_with(val_end, target, len);
    size_ += len;
    val_end += len;
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
    append(*begin.to_const(), *end.to_const());
}

void String::append(const cIter<char> &begin, const cIter<char> &end) {
    Size count = STD::calculateLength(begin, end);
    if (capacity() - size_ - 1 < count) reallocate(capacity() + count + 1);
    fill_with(val_end, begin, count);
    size_ += count;
    val_end += count;
    *val_end = '\0';
}

void String::push_back(char t, Size len) {
    append(t, len);
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
    append(*begin.to_const(), *end.to_const());
}

void String::push_back(const cIter<char> &begin, const cIter<char> &end) {
    append(begin, end);
}

String::Iterator String::insert(Size pos, char t, Size size) {
    if (!size) return String::Iterator(val_begin + pos);
    if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    fill_with(backward(pos, pos + size), t, size);
    return String::Iterator(val_begin + pos);
}

String::Iterator String::insert(Size pos, const std::initializer_list<char> &list) {
    if (!list.size()) return String::Iterator(val_begin + pos);
    if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    fill_with(backward(pos, pos + list.size()), list);
    return String::Iterator(val_begin + pos);
}

String::Iterator String::insert(Size pos, const char *target) {
    return insert(pos, target, calculateLength(target));
}

String::Iterator String::insert(Size pos, const char *target, Size target_len) {
    if (!target_len) return String::Iterator(val_begin + pos);
    if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    fill_with(backward(pos, pos + target_len), target, target_len);
    return String::Iterator(val_begin + pos);
}

String::Iterator String::insert(Size pos, const char *target, Size target_pos, Size target_len) {
    return insert(pos, target + target_pos, target_len);
}

String::Iterator String::insert(Size pos, const String &target, Size target_pos, Size target_len) {
    return insert(pos, target.c_str() + target_pos, target_len);
}

String::Iterator String::insert(Size pos, const Iter<char> &begin, const Iter<char> &end) {
    return insert(pos, *begin.to_const(), *end.to_const());
}

String::Iterator String::insert(Size pos, const cIter<char> &begin, const cIter<char> &end) {
    if (begin == end) return String::Iterator(val_begin + pos);
    if (pos > size_) throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size target_len = STD::calculateLength(begin, end);
    fill_with(backward(pos, pos + target_len), begin, target_len);
    return String::Iterator(val_begin + pos);
}

String::Iterator String::insert(const String::Iterator &iter, char t, Size size) {
    return insert(iter.target - val_begin, t, size);
}

String::Iterator String::insert(const String::Iterator &iter, const std::initializer_list<char> &list) {
    return insert(iter.target - val_begin, list);
}

String::Iterator String::insert(const String::Iterator &iter, const char *target) {
    return insert(iter.target - val_begin, target, STD::calculateLength(target));
}

String::Iterator String::insert(const String::Iterator &iter, const char *target, Size target_len) {
    return insert(iter.target - val_begin, target, target_len);
}

String::Iterator String::insert(const String::Iterator &iter, const char *target, Size target_pos, Size target_len) {
    return insert(iter.target - val_begin, target + target_pos, target_len);
}

String::Iterator String::insert(const String::Iterator &iter, const String &target, Size target_pos, Size target_len) {
    return insert(iter.target - val_begin, target.c_str() + target_pos, target_len);
}

String::Iterator String::insert(const String::Iterator &iter, const Iter<char> &begin, const Iter<char> &end) {
    return insert(iter.target - val_begin, *begin.to_const(), *end.to_const());
}

String::Iterator String::insert(const String::Iterator &iter, const cIter<char> &begin, const cIter<char> &end) {
    return insert(iter.target - val_begin, begin, end);
}

String::cIterator String::insert(const String::cIterator &iter, char t, Size size) {
    return String::cIterator(insert(iter.target - val_begin, t, size).target);
}

String::cIterator String::insert(const String::cIterator &iter, const std::initializer_list<char> &list) {
    return cIterator(insert(iter.target - val_begin, list).target);
}


String::cIterator String::insert(const String::cIterator &iter, const char *target) {
    return String::cIterator(insert(iter.target - val_begin, target, STD::calculateLength(target)).target);
}

String::cIterator String::insert(const String::cIterator &iter, const char *target, Size target_len) {
    return String::cIterator(insert(iter.target - val_begin, target, target_len).target);
}

String::cIterator String::insert(const String::cIterator &iter, const char *target, Size target_pos, Size target_len) {
    return String::cIterator(insert(iter.target - val_begin, target + target_pos, target_len).target);
}

String::cIterator
String::insert(const String::cIterator &iter, const String &target, Size target_pos, Size target_len) {
    return String::cIterator(insert(iter.target - val_begin, target.c_str() + target_pos, target_len).target);
}

String::cIterator String::insert(const String::cIterator &iter, const Iter<char> &begin, const Iter<char> &end) {
    return String::cIterator(insert(iter.target - val_begin, *begin.to_const(), *end.to_const()).target);
}

String::cIterator String::insert(const String::cIterator &iter, const cIter<char> &begin, const cIter<char> &end) {
    return String::cIterator(insert(iter.target - val_begin, begin, end).target);
}

String::rIterator String::insert(const String::rIterator &iter, char t, Size size) {
    return String::rIterator(insert(iter.target - val_begin + 1, t, size).target + size - 1);
}

String::rIterator String::insert(const String::rIterator &iter, const std::initializer_list<char> &list) {
    if (!list.size()) return iter;
    if (iter.target < val_begin - 1 || iter.target >= val_end)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size temp = iter.target - val_begin + 1;
    rfill_with(backward(temp, temp + list.size()), list);
    return String::rIterator(val_begin + temp + list.size() - 1);
}


String::rIterator String::insert(const String::rIterator &iter, const char *target, Size target_len) {
    if (!target_len) return iter;
    if (iter.target >= val_end || iter.target < val_begin - 1)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    auto temp = backward(iter.target - val_begin + 1, iter.target - val_begin + 1 + target_len);
    rfill_with(temp, target, target_len);
    return String::rIterator(temp + target_len - 1);
}

String::rIterator String::insert(const String::rIterator &iter, const char *target) {
    return insert(iter, target, STD::calculateLength(target));
}

String::rIterator String::insert(const String::rIterator &iter, const char *target, Size target_pos, Size target_len) {
    return insert(iter, target + target_pos, target_len);
}

String::rIterator
String::insert(const String::rIterator &iter, const String &target, Size target_pos, Size target_len) {
    if (target_len == Npos) target_len = target.size_;
    return insert(iter, target.c_str() + target_pos, target_len);
}

String::rIterator String::insert(const String::rIterator &iter, const Iter<char> &begin, const Iter<char> &end) {
    return insert(iter, *begin.to_const(), *end.to_const());
}

String::rIterator String::insert(const String::rIterator &iter, const cIter<char> &begin, const cIter<char> &end) {
    if (begin == end) return iter;
    if (iter.target >= val_end || iter.target < val_begin - 1)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size target_len = STD::calculateLength(begin, end);
    auto temp = backward(iter.target - val_begin + 1, iter.target - val_begin + 1 + target_len);
    rfill_with(temp, begin, target_len);
    return String::rIterator(temp + target_len - 1);
}

String::crIterator String::insert(const String::crIterator &iter, char t, Size size) {
    return String::crIterator(insert(iter.target - val_begin + 1, t, size).target + size - 1);
}

String::crIterator String::insert(const String::crIterator &iter, const std::initializer_list<char> &list) {
    return String::crIterator(insert(rIterator(iter.target), list).target);
}

String::crIterator String::insert(const String::crIterator &iter, const char *target) {
    return String::crIterator(insert(rIterator(iter.target), target, STD::calculateLength(target)).target);
}

String::crIterator String::insert(const String::crIterator &iter, const char *target, Size target_len) {
    return String::crIterator(insert(rIterator(iter.target), target, target_len).target);
}

String::crIterator
String::insert(const String::crIterator &iter, const char *target, Size target_pos, Size target_len) {
    return String::crIterator(insert(rIterator(iter.target), target + target_pos, target_len).target);
}

String::crIterator
String::insert(const String::crIterator &iter, const String &target, Size target_pos, Size target_len) {
    if (target_len == Npos) target_len = target.size_;
    return String::crIterator(insert(rIterator(iter.target), target.c_str() + target_pos, target_len).target);
}

String::crIterator String::insert(const String::crIterator &iter, const Iter<char> &begin, const Iter<char> &end) {
    return String::crIterator(insert(rIterator(iter.target), *begin.to_const(), *end.to_const()).target);
}

String::crIterator String::insert(const String::crIterator &iter, const cIter<char> &begin, const cIter<char> &end) {
    return String::crIterator(insert(rIterator(iter.target), begin, end).target);
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

String::rIterator String::erase(const String::rIterator &iter) {
    return String::rIterator(erase(iter.target - val_begin, 1).target);
}

String::crIterator String::erase(const String::crIterator &iter) {
    return String::crIterator(erase(iter.target - val_begin, 1).target);
}

String::rIterator String::erase(const String::rIterator &begin, const String::rIterator &end) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin - 1 || end.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::erase' function");
    return String::rIterator(erase(end.target - val_begin + 1, end - begin).target);
}

String::crIterator String::erase(const String::crIterator &begin, const String::crIterator &end) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin - 1 || end.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::erase' function");
    return String::crIterator(erase(end.target - val_begin + 1, end - begin).target);
}

String &String::replace(Size pos, Size len, Size n, char t) {
    if (pos + len > size_) throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    if (len < n) backward(pos + len, pos + n);
    else if (len > n) forward(pos + len, pos + n);
    fill_with(val_begin + pos, t, n);
    return *this;
}

String &String::replace(Size pos, Size len, const std::initializer_list<char> &list) {
    if (pos + len > size_) throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    if (len < list.size()) backward(pos + len, pos + list.size());
    else if (len > list.size()) forward(pos + len, pos + list.size());
    fill_with(val_begin + pos, list);
    return *this;
}

String &String::replace(Size pos, Size len, const char *target, Size target_len) {
    if (pos + len > size_) throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    if (len < target_len) backward(pos + len, pos + target_len);
    else if (len > target_len) forward(pos + len, pos + target_len);
    fill_with(val_begin + pos, target, target_len);
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

String &String::replace(Size pos, Size len, const Iter<char> &begin, const Iter<char> &end) {
    return replace(pos, len, *begin.to_const(), *end.to_const());
}

String &String::replace(Size pos, Size len, const cIter<char> &begin, const cIter<char> &end) {
    if (pos + len > size_) throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    Size target_len = calculateLength(begin, end);
    if (len < target_len) backward(pos + len, pos + target_len);
    else if (len > target_len) forward(pos + len, pos + target_len);
    fill_with(val_begin + pos, begin, target_len);
    return *this;
}

String &String::replace(const String::Iterator &begin, const String::Iterator &end, Size n, char t) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, n, t);
}

String &
String::replace(const String::Iterator &begin, const String::Iterator &end, const std::initializer_list<char> &list) {
    return replace(begin.target - val_begin, end - begin, list);
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

String &String::replace(const String::Iterator &begin, const String::Iterator &end, const Iter<char> &target_begin,
                        const Iter<char> &target_end) {
    return replace(begin.target - val_begin, end - begin, *target_begin.to_const(), *target_end.to_const());
}

String &String::replace(const String::Iterator &begin, const String::Iterator &end, const cIter<char> &target_begin,
                        const cIter<char> &target_end) {
    return replace(begin.target - val_begin, end - begin, target_begin, target_end);
}

String &String::replace(const String::cIterator &begin, const String::cIterator &end, Size n, char t) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin || end.target > val_end ||
        begin.target > end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin, end.target - begin.target, n, t);
}

String &
String::replace(const String::cIterator &begin, const String::cIterator &end, const std::initializer_list<char> &list) {
    return replace(begin.target - val_begin, end - begin, list);
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

String &String::replace(const String::cIterator &begin, const String::cIterator &end, const Iter<char> &target_begin,
                        const Iter<char> &target_end) {
    return replace(begin.target - val_begin, end - begin, *target_begin.to_const(), *target_end.to_const());
}

String &String::replace(const String::cIterator &begin, const String::cIterator &end, const cIter<char> &target_begin,
                        const cIter<char> &target_end) {
    return replace(begin.target - val_begin, end - begin, target_begin, target_end);
}

String &String::replace(const String::rIterator &begin, const String::rIterator &end, Size n, char t) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin - 1 || end.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin + 1, begin - end, n, t);
}

String &
String::replace(const String::rIterator &begin, const String::rIterator &end, const std::initializer_list<char> &list) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin - 1 || end.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    Size len = end - begin;
    if (len < list.size()) {
        auto temp = end.target - val_begin + 1;
        backward(begin.target - val_begin + 1, begin.target - val_begin + 1 + list.size() - len);
        rfill_with(val_begin + temp, list);
    } else if (len > list.size()) {
        forward(begin.target - val_begin + 1, begin.target - val_begin + 1 + list.size() - len);
        rfill_with(end.target + 1, list);
    } else rfill_with(end.target + 1, list);
    return *this;
}

String &
String::replace(const String::rIterator &begin, const String::rIterator &end, const char *target, Size target_len) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin - 1 || end.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    Size len = end - begin;
    if (len < target_len) {
        auto temp = end.target - val_begin + 1;
        backward(begin.target - val_begin + 1, begin.target - val_begin + 1 + target_len - len);
        rfill_with(val_begin + temp, target, target_len);
    } else if (len > target_len) {
        forward(begin.target - val_begin + 1, begin.target - val_begin + 1 + target_len - len);
        rfill_with(end.target + 1, target, target_len);
    } else rfill_with(end.target + 1, target, target_len);
    return *this;
}

String &String::replace(const String::rIterator &begin, const String::rIterator &end, const char *target) {
    return replace(begin, end, target, STD::calculateLength(target));
}

String &String::replace(const String::rIterator &begin, const String::rIterator &end, const String &target) {
    return replace(begin, end, target.c_str(), target.size_);
}

String &String::replace(const String::rIterator &begin, const String::rIterator &end, const String &target,
                        Size target_pos, Size target_len) {
    return replace(begin, end, target.c_str() + target_pos, target_len);
}

String &String::replace(const String::rIterator &begin, const String::rIterator &end, const Iter<char> &target_begin,
                        const Iter<char> &target_end) {
    return replace(begin, end, *target_begin.to_const(), *target_end.to_const());
}

String &String::replace(const String::rIterator &begin, const String::rIterator &end, const cIter<char> &target_begin,
                        const cIter<char> &target_end) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin - 1 || end.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    Size len = end - begin, target_len = calculateLength(target_begin, target_end);
    if (len < target_len) {
        auto temp = end.target - val_begin + 1;
        backward(begin.target - val_begin + 1, begin.target - val_begin + 1 + target_len - len);
        rfill_with(val_begin + temp, target_begin, target_len);
    } else if (len > target_len) {
        forward(begin.target - val_begin + 1, begin.target - val_begin + 1 + target_len - len);
        rfill_with(end.target + 1, target_begin, target_len);
    } else rfill_with(end.target + 1, target_begin, target_len);
    return *this;
}

String &String::replace(const String::crIterator &begin, const String::crIterator &end, Size n, char t) {
    if (begin.target >= val_end || begin.target < val_begin || end.target < val_begin - 1 || end.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    return replace(begin.target - val_begin + 1, begin - end, n, t);
}

String &
String::replace(const String::crIterator &begin, const String::crIterator &end,
                const std::initializer_list<char> &list) {
    return replace(rIterator(begin.target), rIterator(end.target), list);
}

String &
String::replace(const String::crIterator &begin, const String::crIterator &end, const char *target, Size target_len) {
    return replace(String::rIterator(begin.target), String::rIterator(end.target), target, target_len);
}

String &String::replace(const String::crIterator &begin, const String::crIterator &end, const char *target) {
    return replace(String::rIterator(begin.target), String::rIterator(end.target), target,
                   STD::calculateLength(target));
}

String &String::replace(const String::crIterator &begin, const String::crIterator &end, const String &target) {
    return replace(String::rIterator(begin.target), String::rIterator(end.target), target.c_str(), target.size_);
}

String &String::replace(const String::crIterator &begin, const String::crIterator &end, const String &target,
                        Size target_pos, Size target_len) {
    return replace(String::rIterator(begin.target), String::rIterator(end.target), target.c_str() + target_pos,
                   target_len);
}

String &String::replace(const String::crIterator &begin, const String::crIterator &end, const Iter<char> &target_begin,
                        const Iter<char> &target_end) {
    return replace(String::rIterator(begin.target), String::rIterator(end.target), target_begin, target_end);
}

String &String::replace(const String::crIterator &begin, const String::crIterator &end, const cIter<char> &target_begin,
                        const cIter<char> &target_end) {
    return replace(String::rIterator(begin.target), String::rIterator(end.target), target_begin, target_end);
}

String &String::operator=(const String &other) {
    if (this == &other) return *this;
    Deallocate_n(val_begin);
    val_begin = Allocate_n<char>(other.size_ + 1);
    size_ = other.size_;
    val_end = val_begin + other.size_;
    store_end = val_end + 1;
    fill_with(val_begin, other.val_begin, size_);
    *val_end = '\0';
    return *this;
}

String &String::operator=(String &&other) noexcept {
    if (this == &other) return *this;
    Deallocate_n(val_begin);
    size_ = other.size_;
    val_begin = other.val_begin;
    val_end = other.val_end;
    store_end = other.store_end;
    other.size_ = 0;
    other.val_begin = other.val_end = other.store_end = nullptr;
    return *this;
}

String &STD::String::operator+=(const String &other) {
    append(other);
    return *this;
}

String &String::operator*=(int size) {
    if (size < 0) throw logicError("You passed an illegal value in the function 'String::operator*'.");
    Size new_size = size_ * size;
    auto ptr = Allocate_n<char>(new_size + 1), temp = ptr;
    for (int i = 0; i < size; ++i) {
        String::fill_with(temp, val_begin, size_);
        temp += size_;
    }
    Deallocate_n(val_begin);
    val_begin = ptr;
    size_ = new_size;
    val_end = temp;
    store_end = temp + 1;
    *val_end = '\0';
    return *this;
}

String STD::operator+(const String &left, const String &right) {
    auto ptr = Allocate_n<char>(left.size_ + right.size_ + 1);
    String target;
    String::fill_with(ptr, left.val_begin, left.size_);
    String::fill_with(ptr + left.size_, right.val_begin, right.size_);
    target.size_ = left.size_ + right.size_;
    target.val_begin = ptr;
    target.val_end = ptr + target.size_;
    target.store_end = ptr + target.size_ + 1;
    *target.val_end = '\0';
    return target;
}

String STD::operator*(const String &target, int size) {
    if (size < 0) throw logicError("You passed an illegal value in the function 'String::operator*'.");
    String target_;
    target_.size_ = size * target.size_;
    auto ptr = Allocate_n<char>(target_.size_ + 1), temp = ptr;
    for (int i = 0; i < size; ++i) {
        String::fill_with(temp, target.val_begin, target.size_);
        temp += target.size_;
    }
    target_.val_begin = ptr;
    target_.val_end = temp;
    target_.store_end = temp + 1;
    *target_.val_end = '\0';
    return target_;
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
    return !(left > right);
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
    return !(left < right);
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

