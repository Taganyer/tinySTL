//
// Created by 86152 on 2023/9/9.
//

#include "../String.hpp"

using namespace STD;

//----------------------------------------------------------------------------------------------------------------------

void String::reallocate(Size size) {
    auto the_new = Allocate_n<char>(size), the_old = val_begin;
    store_end = the_new + size;
    for (int i = 0; i < size_; ++i)
        *the_new++ = *the_old++;
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
        auto temp1 = val_end - 1, temp2 = val_end - 1 + pos_to - pos_from,
                target_end = val_begin + pos_from;
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

String::String() : val_begin(Allocate_n<char>(1)),
                   val_end(val_begin), store_end(val_begin + 1) {
    *val_begin = '\0';
}

String::String(Size size, char target)
        : size_(size), val_begin(Allocate_n<char>(size + 1)) {
    val_end = val_begin + size;
    store_end = val_end + 1;
    fill_with(val_begin, size, target);
    *val_end = '\0';
}

String::String(const char *target, Size len) :
        size_(len), val_begin(Allocate_n<char>(len + 1)),
        store_end(val_begin + len + 1) {
    val_end = val_begin + len;
    fill_with(val_begin, target, target + len);
    *val_end = '\0';
}

String::String(const std::initializer_list<char> &list)
        : size_(list.size()), val_begin(Allocate_n<char>(size_ + 1)),
          val_end(val_begin + list.size()), store_end(val_begin + size_ + 1) {
    fill_with(val_begin, list);
    *val_end = '\0';
}

String::String(const char *target) : String(target, STD::calculate_Length(target)) {}

String::String(const String &other, Size pos) : String(other.c_str() + pos) {}

String::String(const String &other, Size pos, Size len) : String(other.c_str() + pos, len) {}

String::String(const String &other) : String(other.c_str(), other.size_) {}

String::String(String &&other) noexcept
        : size_(other.size_), val_begin(other.val_begin), val_end(other.val_end),
          store_end(other.store_end) {
    other.size_ = 0;
    other.val_begin = other.val_end = other.store_end = nullptr;
}

template<typename Input_iterator>
String::String(const Input_iterator &begin, const Input_iterator &end)
        : size_(get_size(begin, end)) {
    val_begin = Allocate_n<char>(size_ + 1);
    store_end = val_begin + size_ + 1;
    val_end = val_begin + size_;
    fill_with(val_begin, begin, end);
    *val_end = '\0';
}

String String::sub_str(Size begin, Size len) const {
    return {val_begin + begin, len};
}

template<typename Input_iterator>
String &String::assign(const Input_iterator &begin, const Input_iterator &end) {
    Deallocate_n(val_begin);
    size_ = get_size(begin, end);
    val_begin = Allocate_n<char>(size_ + 1);
    val_end = val_begin + size_;
    store_end = val_end + 1;
    fill_with(val_begin, begin, end);
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

void String::append(char t) {
    if (capacity() <= size_ + 1)
        reallocate(size_ + 1 > size_ + size_ / 5 ?
                   size_ + 1 + 1 : size_ + size_ / 5 + 1);
    ++size_;
    *val_end = t;
    ++val_end;
    *val_end = '\0';
}

void String::append(Size len, char t) {
    if (capacity() <= size_ + len)
        reallocate(size_ + len > size_ + size_ / 5 ?
                   size_ + len + 1 : size_ + size_ / 5 + 1);
    fill_with(val_end, len, t);
    size_ += len;
    val_end += len;
    *val_end = '\0';
}

void String::append(const char *target, Size len) {
    if (capacity() - size_ - 1 < len)
        reallocate(capacity() + len + 1);
    fill_with(val_end, target, target + len);
    size_ += len;
    val_end += len;
    *val_end = '\0';
}

void String::append(const char *target) {
    append(target, 0, STD::calculate_Length(target));
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

template<typename Input_iterator>
void String::append(const Input_iterator &begin, const Input_iterator &end) {
    Size count = get_size(begin, end);
    if (capacity() - size_ - 1 < count)
        reallocate(capacity() + count + 1);
    fill_with(val_end, begin, end);
    size_ += count;
    val_end += count;
    *val_end = '\0';
}

void String::push_back(char t) { append(t); }

void String::push_back(Size len, char t) { append(len, t); }

void String::push_back(const char *target) { append(target); }

void String::push_back(const char *target, Size len) { append(target, len); }

void String::push_back(const char *target, Size pos, Size len) {
    append(target, pos, len);
}

void String::push_back(const String &target) { append(target); }

void String::push_back(const String &target, Size pos) { append(target, pos); }

void String::push_back(const String &target, Size pos, Size len) {
    append(target, pos, len);
}

template<typename Input_iterator>
void String::push_back(const Input_iterator &begin, const Input_iterator &end) {
    append(begin, end);
}

String::Iterator
String::insert(Size pos, char t) {
    if (pos > size_)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    *backward(pos, pos + 1) = t;
    return Iterator(val_begin + pos);
}

String::Iterator
String::insert(Size pos, Size size, char t) {
    if (!size)
        return Iterator(val_begin + pos);
    if (pos > size_)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    fill_with(backward(pos, pos + size), size, t);
    return Iterator(val_begin + pos);
}

String::Iterator
String::insert(Size pos, const std::initializer_list<char> &list) {
    if (!list.size())
        return Iterator(val_begin + pos);
    if (pos > size_)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    fill_with(backward(pos, pos + list.size()), list);
    return Iterator(val_begin + pos);
}

String::Iterator
String::insert(Size pos, const char *target) {
    return insert(pos, target, STD::calculate_Length(target));
}

String::Iterator
String::insert(Size pos, const char *target, Size target_len) {
    if (!target_len)
        return Iterator(val_begin + pos);
    if (pos > size_)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    fill_with(backward(pos, pos + target_len), target, target + target_len);
    return Iterator(val_begin + pos);
}

String::Iterator
String::insert(Size pos, const char *target, Size target_pos, Size target_len) {
    return insert(pos, target + target_pos, target_len);
}

String::Iterator
String::insert(Size pos, const String &target, Size target_pos, Size target_len) {
    return insert(pos, target.c_str() + target_pos, target_len);
}

template<typename Input_iterator>
String::Iterator
String::insert(Size pos, const Input_iterator &begin, const Input_iterator &end) {
    if (begin == end)
        return Iterator(val_begin + pos);
    if (pos > size_)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size target_len = get_size(begin, end);
    fill_with(backward(pos, pos + target_len), begin, end);
    return Iterator(val_begin + pos);
}

String::Iterator
String::insert(const Iterator &iter, char t) {
    return insert(iter.target - val_begin, t);
}

String::Iterator
String::insert(const Iterator &iter, Size size, char t) {
    return insert(iter.target - val_begin, size, t);
}

String::Iterator
String::insert(const Iterator &iter, const std::initializer_list<char> &list) {
    return insert(iter.target - val_begin, list);
}

String::Iterator
String::insert(const Iterator &iter, const char *target) {
    return insert(iter.target - val_begin, target, STD::calculate_Length(target));
}

String::Iterator
String::insert(const Iterator &iter, const char *target, Size target_len) {
    return insert(iter.target - val_begin, target, target_len);
}

String::Iterator
String::insert(const Iterator &iter, const char *target,
               Size target_pos, Size target_len) {
    return insert(iter.target - val_begin, target + target_pos, target_len);
}

String::Iterator
String::insert(const Iterator &iter, const String &target,
               Size target_pos, Size target_len) {
    return insert(iter.target - val_begin,
                  target.c_str() + target_pos, target_len);
}

template<typename Input_iterator>
String::Iterator
String::insert(const Iterator &iter, const Input_iterator &begin,
               const Input_iterator &end) {
    return insert(iter.target - val_begin, begin, end);
}

String::cIterator
String::insert(const cIterator &iter, char t) {
    return cIterator(insert(iter.target.target - val_begin, t));
}

String::cIterator
String::insert(const cIterator &iter, Size size, char t) {
    return cIterator(insert(iter.target.target - val_begin, size, t));
}

String::cIterator
String::insert(const cIterator &iter, const std::initializer_list<char> &list) {
    return cIterator(insert(iter.target.target - val_begin, list));
}

String::cIterator
String::insert(const cIterator &iter, const char *target) {
    return cIterator(insert(iter.target.target - val_begin,
                            target, STD::calculate_Length(target)));
}

String::cIterator
String::insert(const cIterator &iter, const char *target, Size target_len) {
    return cIterator(insert(iter.target.target - val_begin, target, target_len));
}

String::cIterator
String::insert(const cIterator &iter, const char *target,
               Size target_pos, Size target_len) {
    return cIterator(insert(iter.target.target - val_begin,
                            target + target_pos, target_len));
}

String::cIterator
String::insert(const cIterator &iter, const String &target,
               Size target_pos, Size target_len) {
    return cIterator(insert(iter.target.target - val_begin,
                            target.c_str() + target_pos, target_len));
}

template<typename Input_iterator>
String::cIterator
String::insert(const cIterator &iter, const Input_iterator &begin,
               const Input_iterator &end) {
    return cIterator(insert(iter.target.target - val_begin, begin, end));
}

String::rIterator
String::insert(const rIterator &iter, char t) {
    if (iter.target.target < val_begin - 1 || iter.target.target >= val_end)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size pos_from = iter.target.target - val_begin + 1;
    auto ptr = backward(pos_from, pos_from + 1);
    *ptr = t;
    return rIterator(Iterator(ptr));
}

String::rIterator
String::insert(const rIterator &iter, Size size, char t) {
    if (!size)
        return iter;
    if (iter.target.target < val_begin - 1 || iter.target.target >= val_end)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size pos_from = iter.target.target - val_begin + 1;
    auto ptr = backward(pos_from, pos_from + size) + size - 1;
    rfill_with(ptr, size, t);
    return rIterator(Iterator(ptr));
}

String::rIterator
String::insert(const rIterator &iter, const std::initializer_list<char> &list) {
    if (!list.size())
        return iter;
    if (iter.target.target < val_begin - 1 || iter.target.target >= val_end)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size pos_from = iter.target.target - val_begin + 1;
    auto ptr = backward(pos_from, pos_from + list.size()) + list.size() - 1;
    rfill_with(ptr, list);
    return rIterator(Iterator(ptr));
}

String::rIterator
String::insert(const rIterator &iter, const char *target, Size target_len) {
    if (!target_len)
        return iter;
    if (iter.target.target >= val_end || iter.target.target < val_begin - 1)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size pos_from = iter.target.target - val_begin + 1;
    auto ptr = backward(pos_from, pos_from + target_len) + target_len - 1;
    rfill_with(ptr, target, target + target_len);
    return rIterator(Iterator(ptr));
}

String::rIterator
String::insert(const rIterator &iter, const char *target) {
    return insert(iter, target, STD::calculate_Length(target));
}

String::rIterator
String::insert(const rIterator &iter, const char *target,
               Size target_pos, Size target_len) {
    return insert(iter, target + target_pos, target_len);
}

String::rIterator
String::insert(const rIterator &iter, const String &target,
               Size target_pos, Size target_len) {
    if (target_len == Npos)
        target_len = target.size_;
    return insert(iter, target.c_str() + target_pos, target_len);
}

template<typename Input_iterator>
String::rIterator
String::insert(const rIterator &iter, const Input_iterator &begin,
               const Input_iterator &end) {
    if (begin == end)
        return iter;
    if (iter.target.target >= val_end || iter.target.target < val_begin - 1)
        throw outOfRange("You passed an out-of-range value in the 'String::insert' function");
    Size target_len = get_size(begin, end);
    Size pos_from = iter.target.target - val_begin + 1;
    auto ptr = backward(pos_from, pos_from + target_len) + target_len - 1;
    rfill_with(ptr, begin, end);
    return rIterator(Iterator(ptr));
}

String::crIterator
String::insert(const crIterator &iter, char t) {
    return crIterator(insert(rIterator(iter.target), t).target);
}

String::crIterator
String::insert(const crIterator &iter, Size size, char t) {
    return crIterator(insert(rIterator(iter.target), size, t).target);
}

String::crIterator
String::insert(const crIterator &iter, const std::initializer_list<char> &list) {
    return crIterator(insert(rIterator(iter.target), list).target);
}

String::crIterator
String::insert(const crIterator &iter, const char *target) {
    return crIterator(insert(rIterator(iter.target), target,
                             STD::calculate_Length(target)).target);
}

String::crIterator
String::insert(const crIterator &iter, const char *target, Size target_len) {
    return crIterator(insert(rIterator(iter.target), target, target_len).target);
}

String::crIterator
String::insert(const crIterator &iter, const char *target,
               Size target_pos, Size target_len) {
    return crIterator(insert(rIterator(iter.target), target + target_pos, target_len).target);
}

String::crIterator
String::insert(const crIterator &iter, const String &target,
               Size target_pos, Size target_len) {
    if (target_len == Npos) target_len = target.size_;
    return crIterator(insert(rIterator(iter.target),
                             target.c_str() + target_pos, target_len).target);
}

template<typename Input_iterator>
String::crIterator
String::insert(const crIterator &iter, const Input_iterator &begin,
               const Input_iterator &end) {
    return crIterator(insert(rIterator(iter.target), begin, end).target);
}

String::Iterator String::erase(Size pos, Size size) {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::erase' function");
    if (!size)
        return Iterator(val_begin + pos);
    size = pos + size > size_ ? size_ - pos : size;
    auto temp1 = val_begin + pos, temp2 = val_begin + size;
    while (temp2 < val_end) {
        *temp1 = *temp2;
        ++temp1, ++temp2;
    }
    size_ -= size;
    val_end -= size;
    *val_end = '\0';
    return Iterator(val_begin + pos);
}

String::Iterator String::erase(const Iterator &iter) {
    return erase(iter.target - val_begin, 1);
}

String::cIterator String::erase(const cIterator &iter) {
    return cIterator(erase(iter.target.target - val_begin, 1));
}

String::Iterator String::erase(const Iterator &begin, const Iterator &end) {
    return erase(begin.target - val_begin, end - begin);
}

String::cIterator String::erase(const cIterator &begin, const cIterator &end) {
    return cIterator(erase(begin.target, end.target));
}

String::rIterator String::erase(const rIterator &iter) {
    return rIterator(erase(iter.target.target - val_begin, 1));
}

String::crIterator String::erase(const crIterator &iter) {
    return crIterator(erase(iter.target.target - val_begin, 1));
}

String::rIterator String::erase(const rIterator &begin, const rIterator &end) {
    return rIterator(--erase(end.target.target - val_begin + 1, end - begin));
}

String::crIterator String::erase(const crIterator &begin, const crIterator &end) {
    return crIterator(--erase(end.target.target - val_begin + 1, end - begin));
}

String &String::replace(Size pos, Size len, Size n, char t) {
    if (pos + len > size_)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    if (len < n)
        backward(pos + len, pos + n);
    else if (len > n)
        forward(pos + len, pos + n);
    fill_with(val_begin + pos, n, t);
    return *this;
}

String &String::replace(Size pos, Size len, const std::initializer_list<char> &list) {
    if (pos + len > size_)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    if (len < list.size())
        backward(pos + len, pos + list.size());
    else if (len > list.size())
        forward(pos + len, pos + list.size());
    fill_with(val_begin + pos, list);
    return *this;
}

String &String::replace(Size pos, Size len, const char *target, Size target_len) {
    if (pos + len > size_)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    if (len < target_len)
        backward(pos + len, pos + target_len);
    else if (len > target_len)
        forward(pos + len, pos + target_len);
    fill_with(val_begin + pos, target, target + target_len);
    return *this;
}

String &String::replace(Size pos, Size len, const char *target) {
    return replace(pos, len, target, STD::calculate_Length(target));
}

String &String::replace(Size pos, Size len, const String &target) {
    return replace(pos, len, target.c_str(), STD::calculate_Length(target.c_str()));
}

String &String::replace(Size pos, Size len, const String &target,
                        Size target_pos, Size target_len) {
    return replace(pos, len, target.c_str() + target_pos, target_len);
}

template<typename Input_iterator>
String &String::replace(Size pos, Size len, const Input_iterator &begin, const Input_iterator &end) {
    if (pos + len > size_)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    Size target_len = get_size(begin, end);
    if (len < target_len)
        backward(pos + len, pos + target_len);
    else if (len > target_len)
        forward(pos + len, pos + target_len);
    fill_with(val_begin + pos, begin, end);
    return *this;
}

String &String::replace(const Iterator &begin, const Iterator &end, Size n, char t) {
    return replace(begin.target - val_begin, end.target - begin.target, n, t);
}

String &String::replace(const Iterator &begin, const Iterator &end,
                        const std::initializer_list<char> &list) {
    return replace(begin.target - val_begin, end - begin, list);
}

String &String::replace(const Iterator &begin, const Iterator &end,
                        const char *target, Size target_len) {
    return replace(begin.target - val_begin, end - begin, target, target_len);
}

String &String::replace(const Iterator &begin, const Iterator &end, const char *target) {
    return replace(begin.target - val_begin, end - begin,
                   target, STD::calculate_Length(target));
}

String &String::replace(const Iterator &begin, const Iterator &end, const String &target) {
    return replace(begin.target - val_begin, end - begin,
                   target.c_str(), target.size_);
}

String &String::replace(const Iterator &begin, const Iterator &end,
                        const String &target, Size target_pos, Size target_len) {
    return replace(begin.target - val_begin, end - begin,
                   target.c_str() + target_pos, target_len);
}

template<typename Input_iterator>
String &String::replace(const Iterator &begin, const Iterator &end,
                        const Input_iterator &target_begin, const Input_iterator &target_end) {
    return replace(begin.target - val_begin, end - begin, target_begin, target_end);
}

String &String::replace(const cIterator &begin, const cIterator &end, Size n, char t) {
    return replace(begin.target.target - val_begin, end - begin, n, t);
}

String &String::replace(const cIterator &begin, const cIterator &end,
                        const std::initializer_list<char> &list) {
    return replace(begin.target.target - val_begin, end - begin, list);
}

String &String::replace(const cIterator &begin, const cIterator &end,
                        const char *target, Size target_len) {
    return replace(begin.target.target - val_begin, end - begin, target, target_len);
}

String &String::replace(const cIterator &begin, const cIterator &end, const char *target) {
    return replace(begin.target.target - val_begin, end - begin,
                   target, STD::calculate_Length(target));
}

String &String::replace(const cIterator &begin, const cIterator &end, const String &target) {
    return replace(begin.target.target - val_begin, end - begin,
                   target.c_str(), target.size_);
}

String &String::replace(const cIterator &begin, const cIterator &end,
                        const String &target, Size target_pos, Size target_len) {
    return replace(begin.target.target - val_begin, end - begin,
                   target.c_str() + target_pos, target_len);
}

template<typename Input_iterator>
String &String::replace(const cIterator &begin, const cIterator &end,
                        const Input_iterator &target_begin, const Input_iterator &target_end) {
    return replace(begin.target.target - val_begin, end - begin, target_begin, target_end);
}

String &String::replace(const rIterator &begin, const rIterator &end, Size n, char t) {
    return replace(end.target.target - val_begin + 1, end - begin, n, t);
}

String &String::replace(const rIterator &begin, const rIterator &end,
                        const std::initializer_list<char> &list) {
    if (begin.target.target >= val_end || begin.target.target < val_begin ||
        end.target.target < val_begin - 1 || end.target.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    Size len = end - begin;
    if (len < list.size()) {
        Size pos_from = end.target.target - val_begin + len + 1;
        Size pos_to = pos_from + list.size() - len;
        backward(pos_from, pos_to);
        rfill_with(val_begin + pos_to - 1, list);
    } else if (len > list.size()) {
        Size pos_from = begin.target.target - val_begin + 1;
        Size pos_to = pos_from + list.size() - len;
        forward(pos_from, pos_to);
        rfill_with(val_begin + pos_to - 1, list);
    } else
        rfill_with(begin.target.target, list);
    return *this;
}

String &String::replace(const rIterator &begin, const rIterator &end, const char *target, Size target_len) {
    if (begin.target.target >= val_end || begin.target.target < val_begin ||
        end.target.target < val_begin - 1 || end.target.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    Size len = end - begin;
    if (len < target_len) {
        Size pos_from = end.target.target - val_begin + len + 1;
        Size pos_to = pos_from + target_len - len;
        backward(pos_from, pos_to);
        rfill_with(val_begin + pos_to - 1, target, target + target_len);
    } else if (len > target_len) {
        Size pos_from = begin.target.target - val_begin + 1;
        Size pos_to = pos_from + target_len - len;
        forward(pos_from, pos_to);
        rfill_with(val_begin + pos_to - 1, target, target + target_len);
    } else
        rfill_with(begin.target.target, target, target + target_len);
    return *this;
}

String &String::replace(const rIterator &begin, const rIterator &end, const char *target) {
    return replace(begin, end, target, STD::calculate_Length(target));
}

String &String::replace(const rIterator &begin, const rIterator &end, const String &target) {
    return replace(begin, end, target.c_str(), target.size_);
}

String &String::replace(const rIterator &begin, const rIterator &end,
                        const String &target, Size target_pos, Size target_len) {
    return replace(begin, end, target.c_str() + target_pos, target_len);
}

template<typename Input_iterator>
String &String::replace(const rIterator &begin, const rIterator &end,
                        const Input_iterator &target_begin, const Input_iterator &target_end) {
    if (begin.target.target >= val_end || begin.target.target < val_begin ||
        end.target.target < val_begin - 1 || end.target.target >= val_end ||
        begin.target < end.target)
        throw outOfRange("You selected an out-of-range value in the 'String::replace' function");
    Size len = end - begin;
    Size target_len = get_size(target_begin, target_end);
    if (len < target_len) {
        Size pos_from = end.target.target - val_begin + len + 1;
        Size pos_to = pos_from + target_len - len;
        backward(pos_from, pos_to);
        rfill_with(val_begin + pos_to - 1, target_begin, target_end);
    } else if (len > target_len) {
        Size pos_from = begin.target.target - val_begin + 1;
        Size pos_to = pos_from + target_len - len;
        forward(pos_from, pos_to);
        rfill_with(val_begin + pos_to - 1, target_begin, target_end);
    } else
        rfill_with(begin.target.target, target_begin, target_end);
    return *this;
}

String &String::replace(const crIterator &begin, const crIterator &end, Size n, char t) {
    return replace(end.target.target - val_begin + 1, end - begin, n, t);
}

String &String::replace(const crIterator &begin, const crIterator &end,
                        const std::initializer_list<char> &list) {
    return replace(rIterator(begin.target), rIterator(end.target), list);
}

String &String::replace(const crIterator &begin, const crIterator &end,
                        const char *target, Size target_len) {
    return replace(rIterator(begin.target), rIterator(end.target),
                   target, target_len);
}

String &String::replace(const crIterator &begin, const crIterator &end, const char *target) {
    return replace(rIterator(begin.target), rIterator(end.target),
                   target, STD::calculate_Length(target));
}

String &String::replace(const crIterator &begin, const crIterator &end, const String &target) {
    return replace(rIterator(begin.target), rIterator(end.target),
                   target.c_str(), target.size_);
}

String &String::replace(const crIterator &begin, const crIterator &end,
                        const String &target, Size target_pos, Size target_len) {
    return replace(rIterator(begin.target), rIterator(end.target),
                   target.c_str() + target_pos, target_len);
}

template<typename Input_iterator>
String &String::replace(const crIterator &begin, const crIterator &end,
                        const Input_iterator &target_begin, const Input_iterator &target_end) {
    return replace(rIterator(begin.target), rIterator(end.target), target_begin, target_end);
}

String &String::operator=(const String &other) {
    if (this == &other)
        return *this;
    Deallocate_n(val_begin);
    val_begin = Allocate_n<char>(other.size_ + 1);
    size_ = other.size_;
    val_end = val_begin + other.size_;
    store_end = val_end + 1;
    fill_with(val_begin, other.val_begin, other.val_end);
    *val_end = '\0';
    return *this;
}

String &String::operator=(String &&other) noexcept {
    if (this == &other)
        return *this;
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
    if (size < 0)
        throw logicError("You passed an illegal value in the function 'String::operator*'.");
    Size new_size = size_ * size;
    auto ptr = Allocate_n<char>(new_size + 1), temp = ptr;
    for (int i = 0; i < size; ++i) {
        fill_with(temp, val_begin, val_end);
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
    fill_with(ptr, left.val_begin, left.val_end);
    fill_with(ptr + left.size_, right.val_begin, right.val_end);
    target.size_ = left.size_ + right.size_;
    target.val_begin = ptr;
    target.val_end = ptr + target.size_;
    target.store_end = ptr + target.size_ + 1;
    *target.val_end = '\0';
    return target;
}

String STD::operator*(const String &target, int size) {
    if (size < 0)
        throw logicError("You passed an illegal value in the function 'String::operator*'.");
    String target_;
    target_.size_ = size * target.size_;
    auto ptr = Allocate_n<char>(target_.size_ + 1), temp = ptr;
    for (int i = 0; i < size; ++i) {
        fill_with(temp, target.val_begin, target.val_end);
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
    if (left.size_ != right.size_)
        return false;
    auto l = left.val_begin, r = right.val_begin;
    for (int i = 0; i < left.size_; ++i) {
        if (*l != *r)
            return false;
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
        if (*l == *r)
            ++l, ++r;
        else
            return *l < *r;
    }
    return l == left.val_end && r != right.val_end;
}

bool STD::operator<=(const String &left, const String &right) {
    return !(left > right);
}

bool STD::operator>(const String &left, const String &right) {
    auto l = left.val_begin, r = right.val_begin;
    while (l < left.val_end && r < right.val_end) {
        if (*l == *r)
            ++l, ++r;
        else
            return *l > *r;
    }
    return l != left.val_end && r == right.val_end;
}

bool STD::operator>=(const String &left, const String &right) {
    return !(left < right);
}

Size String::find(char t, Size pos) const {
    auto temp = val_begin + pos;
    while (temp != val_end) {
        if (*temp == t)
            return temp - val_begin;
        ++temp;
    }
    return Npos;
}

Size String::find(const char *target, Size pos) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find' function");
    auto temp = Boyer_Moore(target, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::find(const char *target, Size pos, Size size) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find' function");
    auto temp = Boyer_Moore(target, size, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::find(const String &target, Size pos) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find' function");
    auto temp = Boyer_Moore(target.val_begin, target.size_, val_begin + pos,
                            size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::find(const String &target, Size pos, Size size) const {
    if (pos >= size_ || size > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find' function");
    auto temp =
            Boyer_Moore(target.val_begin, size, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::rfind(char t, Size pos) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = val_end - 1;
    while (temp >= val_begin) {
        if (*temp == t)
            return temp - val_begin;
        --temp;
    }
    return Npos;
}

Size String::rfind(const char *target, Size pos) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = rBoyer_Moore(target, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::rfind(const char *target, Size pos, Size size) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = rBoyer_Moore(target, size, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::rfind(const String &target, Size pos) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp = rBoyer_Moore(target.val_begin, target.size_, val_begin + pos,
                             size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::rfind(const String &target, Size pos, Size size) const {
    if (pos >= size_ || size > target.size_)
        throw outOfRange("You selected an out-of-range value in the 'String::rfind' function");
    auto temp =
            rBoyer_Moore(target.val_begin, size, val_begin + pos, size_ - pos);
    return temp ? temp - val_begin : Npos;
}

Size String::find_first_of(const char *target, Size pos, Size size) const {
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find_first_of' function");
    bool store[256];
    Memset(store, false, 256);
    for (int i = 0; i < size; ++i)
        store[target[i]] = true;
    auto temp = val_begin + pos;
    while (temp < val_end) {
        if (store[*temp])
            return temp - val_begin;
        ++temp;
    }
    return Npos;
}

Size String::find_first_of(const char *target, Size pos) const {
    return find_first_of(target, pos, STD::calculate_Length(target));
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
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find_last_of' function");
    bool store[256];
    Memset(store, false, 256);
    for (int i = 0; i < size; ++i)
        store[target[i]] = true;
    auto temp = val_end - 1;
    while (temp >= val_begin + pos) {
        if (store[*temp])
            return temp - val_begin;
        --temp;
    }
    return Npos;
}

Size String::find_last_of(const char *target, Size pos) const {
    return find_last_of(target, pos, STD::calculate_Length(target));
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
    for (int i = 0; i < size; ++i)
        store[target[i]] = false;
    auto temp = val_begin + pos;
    while (temp < val_end) {
        if (store[*temp])
            return temp - val_begin;
        ++temp;
    }
    return Npos;
}

Size String::find_first_not_of(const char *target, Size pos) const {
    return find_first_not_of(target, pos, STD::calculate_Length(target));
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
    if (pos >= size_)
        throw outOfRange("You selected an out-of-range value in the 'String::find_last_not_of' function");
    bool store[256];
    Memset(store, true, 256);
    for (int i = 0; i < size; ++i)
        store[target[i]] = false;
    auto temp = val_end - 1;
    while (temp >= val_begin + pos) {
        if (store[*temp])
            return temp - val_begin;
        --temp;
    }
    return Npos;
}

Size String::find_last_not_of(const char *target, Size pos) const {
    return find_last_not_of(target, pos, calculate_Length(target));
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
    if (pos1 + n1 > size_)
        throw outOfRange("You selected an out-of-range value in the 'String::compare' function");
    const char *temp = val_begin + pos1, *end1 = temp + n1, *end2 = target + n2;
    while (temp != end1 && target != end2) {
        if (*temp != *target)
            return *temp - *target;
        ++temp, ++target;
    }
    if (n1 == n2)
        return 0;
    return temp == end1 ? -(*target) : *temp;
}

int String::compare(Size pos1, Size n1, const char *target) const {
    return compare(pos1, n1, target, STD::calculate_Length(target));
}

int String::compare(const char *target) const {
    return compare(0, size_, target, STD::calculate_Length(target));
}

int String::compare(Size pos1, Size n1, const String &target, Size pos2,
                    Size n2) const {
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
