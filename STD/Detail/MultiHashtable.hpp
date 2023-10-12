//
// Created by 86152 on 2023/10/10.
//

#ifndef TINYSTL_MULTIHASHTABLE_HPP
#define TINYSTL_MULTIHASHTABLE_HPP

#include "../Function.hpp"

namespace STD {

    namespace Detail {

        template<typename Key, typename Hash_Code, typename Equal_>
        class MultiHashtable;

        template<typename Key, typename Hash_Code, typename Equal_>
        struct MultiHash_table_Iterator : public Iterator<Bidirectional_iterator_tag, Key> {
        public:
            using Basic = Iterator <Bidirectional_iterator_tag, Key>;

            using Self = MultiHash_table_Iterator<Key, Hash_Code, Equal_>;

            using Container = MultiHashtable<Key, Hash_Code, Equal_>;

            using Reference = typename Basic::Reference;

            using Pointer = typename Basic::Pointer;

            using Node = typename Container::Node;

            friend class MultiHashtable<Key, Hash_Code, Equal_>;

            friend class rIterator<Self, Bidirectional_iterator_tag>;

            friend class cIterator<Self, Bidirectional_iterator_tag>;

            friend class crIterator<Self, Bidirectional_iterator_tag>;

        private:
            Size bucket_location = 0;

            Node *target;

            Container *container;

            explicit MultiHash_table_Iterator(Size index, Node *target, Container *container) :
                    bucket_location(index), target(target), container(container) {};

        public:
            Reference operator*() const {
                return target->value;
            };

            Pointer operator->() const {
                return &(target->value);
            };

            Self &operator++();

            Self operator++(int);

            Self &operator--();

            Self operator--(int);

            friend bool operator==(const Self &left, const Self &right) {
                return left.target == right.target && left.container == right.container;
            };

            friend bool operator!=(const Self &left, const Self &right) {
                return left.target != right.target || left.container != right.container;
            };

        };

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHash_table_Iterator<Key, Hash_Code, Equal_>::Self
        &MultiHash_table_Iterator<Key, Hash_Code, Equal_>::operator++() {
            if (bucket_location == container->buckets) return *this;
            if (target && target->next) target = target->next;
            else {
                while (++bucket_location < container->buckets) {
                    if (container->array[bucket_location]) {
                        target = container->array[bucket_location];
                        return *this;
                    }
                }
                target = nullptr;
            }
            return *this;
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHash_table_Iterator<Key, Hash_Code, Equal_>::Self
        MultiHash_table_Iterator<Key, Hash_Code, Equal_>::operator++(int) {
            Self temp = Self(bucket_location, target, container);
            ++(*this);
            return temp;
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHash_table_Iterator<Key, Hash_Code, Equal_>::Self
        &MultiHash_table_Iterator<Key, Hash_Code, Equal_>::operator--() {
            if (bucket_location == -1) return *this;
            if (!target || container->array[bucket_location] == target) {
                while (--bucket_location != -1) {
                    if ((container->array)[bucket_location]) {
                        target = container->array[bucket_location];
                        while (target->next) target = target->next;
                        return *this;
                    }
                }
                target = nullptr;
            } else {
                Node *last = container->array[bucket_location];
                while (last->next != target) last = last->next;
                target = last;
            }
            return *this;
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHash_table_Iterator<Key, Hash_Code, Equal_>::Self
        MultiHash_table_Iterator<Key, Hash_Code, Equal_>::operator--(int) {
            Self temp = Self(bucket_location, target, container);
            --(*this);
            return temp;
        }

//----------------------------------------------------------------------------------------------------------------------

        template<typename Key, typename Hash_Code = Hash<Key>, typename Equal_ = Equal<Key>>
        class MultiHashtable {
        public:
            friend class MultiHash_table_Iterator<Key, Hash_Code, Equal_>;

            using Iterator = MultiHash_table_Iterator<Key, Hash_Code, Equal_>;

            using cIterator = STD::cIterator<Iterator, Bidirectional_iterator_tag>;

            using rIterator = STD::rIterator<Iterator, Bidirectional_iterator_tag>;

            using crIterator = STD::crIterator<Iterator, Bidirectional_iterator_tag>;

            using Self = MultiHashtable<Key, Hash_Code, Equal_>;

        protected:
            static constexpr int Load_Factor = 1;

            struct Node {
                Key value;

                Node *next = nullptr;

                template<typename ...Args>
                explicit Node(Args &&...args) : value(forward<Args>(args)...) {};
            };

            Size size_ = 0, buckets = 8;

            Node **array = nullptr;

            Size insert(Node *target);

            Iterator get_Iterator(Size index, Node *target) const {
                return Iterator(index, target, const_cast<Self *>(this));
            }

        private:
            Hash_Code hasher;

            Equal_ equal;

            Iterator *get(const Key &key) const;

        public:
            explicit MultiHashtable(Hash_Code hasher = Hash_Code(), Equal_ equal = Equal_()) :
                    array(Allocate_n<Node *>(8)), hasher(hasher), equal(equal) {
                Memset<Node *>(array, nullptr, 8);
            };

            MultiHashtable(const std::initializer_list<Key> &list,
                           Hash_Code hasher = Hash_Code(), Equal_ equal = Equal_());

            template<typename Input_Key>
            MultiHashtable(Input_Key begin, const Input_Key &end,
                           Hash_Code hasher = Hash_Code(), Equal_ equal = Equal_());

            MultiHashtable(const Self &other);

            MultiHashtable(Self &&other) noexcept;

            virtual ~MultiHashtable();

            void Rehash();

            void clear();

            template<typename ...Input_Key>
            Iterator emplace(Input_Key &&...args);

            Iterator insert(const Key &key);

            bool erase(const Key &key);

            void erase(const Iterator &iter);

            void erase(const cIterator &iter);

            void erase(const rIterator &iter);

            void erase(const crIterator &iter);

            Size size() const { return size_; };

            Size bucket_count() const { return buckets; };

            bool empty() const { return !size_; };

            void swap(Self &other) noexcept {
                Size temp1 = size_, temp2 = buckets;
                Node **temp3 = array;
                size_ = other.size_;
                buckets = other.buckets;
                array = other.array;
                other.size_ = temp1;
                other.buckets = temp2;
                other.array = temp3;
            }

            Iterator find(const Key &key);

            cIterator find(const Key &key) const;

            Size count(const Key &key) const;

            bool contains(const Key &key) const;

            // 这两个函数不会复制Compare和Equal_对象。
            Self &operator=(const Self &other);

            Self &operator=(Self &&other) noexcept;

            Iterator begin() const {
                if (!size_) return get_Iterator(0, nullptr);
                Size index = 0;
                while (index < buckets) {
                    if (array[index]) break;
                    ++index;
                }
                return get_Iterator(index, array[index]);
            }

            Iterator end() const { return get_Iterator(buckets, nullptr); };

            cIterator cbegin() const { return cIterator(begin()); };

            cIterator cend() const { return cIterator(end()); };

            rIterator rbegin() const {
                if (!size_) return rIterator(get_Iterator(buckets - 1, nullptr));
                Size index = buckets;
                Node *ptr = nullptr;
                while (index) {
                    if (array[--index]) {
                        ptr = array[index];
                        while (ptr->next) ptr = ptr->next;
                        break;
                    }
                }
                return rIterator(get_Iterator(index, ptr));
            };

            rIterator rend() const { return rIterator(get_Iterator(-1, nullptr)); };

            crIterator crbegin() const {
                if (!size_) return crIterator(get_Iterator(buckets - 1, nullptr));
                Size index = buckets;
                Node *ptr = nullptr;
                while (index) {
                    if (array[--index]) {
                        ptr = array[index];
                        while (ptr->next) ptr = ptr->next;
                        break;
                    }
                }
                return crIterator(get_Iterator(index, ptr));
            };

            crIterator crend() const { return crIterator(get_Iterator(-1, nullptr)); };

            friend bool operator==(const Self &left, const Self &right) {
                if (left.size_ != right.size_) return false;
                auto l = left.begin(), r = right.begin();
                Size size = left.size_;
                while (size) {
                    if (*l != *r) return false;
                    --size;
                }
                return true;
            }

            friend bool operator!=(const Self &left, const Self &right) {
                return !(left == right);
            }

            friend bool operator<(const Self &left, const Self &right) {
                auto l = left.begin(), r = right.begin(), l_end = left.end(), r_end = right.end();
                while (l != l_end && r != r_end) {
                    if (*l != *r) return *l < *r;
                    ++l, ++r;
                }
                return l == l_end && r != r_end;
            }

            friend bool operator<=(const Self &left, const Self &right) {
                return !(left > right);
            }

            friend bool operator>(const Self &left, const Self &right) {
                auto l = left.begin(), r = right.begin(), l_end = left.end(), r_end = right.end();
                while (l != l_end && r != r_end) {
                    if (*l != *r) return *l > *r;
                    ++l, ++r;
                }
                return l != l_end && r == r_end;
            }

            friend bool operator>=(const Self &left, const Self &right) {
                return !(left < right);
            }

            friend inline void swap(Self &left, Self &right) noexcept {
                left.swap(right);
            }

        };

//----------------------------------------------------------------------------------------------------------------------

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHashtable<Key, Hash_Code, Equal_>::Iterator
        *MultiHashtable<Key, Hash_Code, Equal_>::get(const Key &key) const {
            Size index = hasher(key) % buckets;
            Node *ptr = array[index];
            while (ptr) {
                if (equal(key, ptr->value)) return get_Iterator(index, ptr);
                ptr = ptr->next;
            }
            return end();
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        Size MultiHashtable<Key, Hash_Code, Equal_>::insert(Node *target) {
            Rehash();
            ++size_;
            auto index = hasher(target->value) % buckets;
            Node *ptr = array[index], *last = nullptr;
            while (ptr) {
                if (equal(ptr->value, target->value)) {
                    target->next = ptr->next;
                    ptr->next = target;
                    return index;
                }
                last = ptr;
                ptr = ptr->next;
            }
            if (last) last->next = target;
            else array[index] = target;
            return index;
        }

//----------------------------------------------------------------------------------------------------------------------

        template<typename Key, typename Hash_Code, typename Equal_>
        MultiHashtable<Key, Hash_Code, Equal_>::MultiHashtable
                (const std::initializer_list<Key> &list, Hash_Code hasher, Equal_ equal)
                : buckets(list.size() + list.size() / 5), array(Allocate_n<Node *>(buckets)),
                  hasher(hasher), equal(equal) {
            Memset<Node *>(array, nullptr, buckets);
            for (const Key &t: list) {
                auto ptr = Allocate<Node>(t);
                insert(ptr);
            }
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        template<typename Input_Key>
        MultiHashtable<Key, Hash_Code, Equal_>::MultiHashtable
                (Input_Key begin, const Input_Key &end, Hash_Code hasher, Equal_ equal)
                : hasher(hasher), equal(equal) {
            Size temp = calculate_Length(begin, end);
            temp += temp / 5;
            if (temp < 8) temp = 8;
            buckets = temp;
            array = Allocate_n<Node *>(buckets);
            Memset<Node *>(array, nullptr, buckets);
            while (begin != end) {
                auto ptr = Allocate<Node>(*begin);
                insert(ptr);
                ++begin;
            }
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        MultiHashtable<Key, Hash_Code, Equal_>::MultiHashtable(const MultiHashtable::Self &other) :
                MultiHashtable(other.begin(), other.end(), other.hasher, other.equal) {}

        template<typename Key, typename Hash_Code, typename Equal_>
        MultiHashtable<Key, Hash_Code, Equal_>::MultiHashtable(MultiHashtable::Self &&other) noexcept :
                size_(other.size_), buckets(other.buckets), array(other.array),
                hasher(other.hasher), equal(other.equal) {
            other.size_ = 0;
            other.buckets = 8;
            other.array = Allocate_n<Node *>(8);
            Memset<Node *>(other.array, nullptr, 8);
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        MultiHashtable<Key, Hash_Code, Equal_>::~MultiHashtable() {
            clear();
            Deallocate_n(array);
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        void MultiHashtable<Key, Hash_Code, Equal_>::Rehash() {
            if (static_cast<double>(size_) / static_cast<double>(buckets) >= Load_Factor) {
                Node **old = array;
                Size old_buckets = buckets;
                buckets = size_ + size_ / 5;
                array = Allocate_n<Node *>(buckets);
                Memset<Node *>(array, nullptr, buckets);
                while (old_buckets) {
                    Node *ptr = old[--old_buckets];
                    while (ptr) {
                        Size destination = hasher(ptr->value) % buckets;
                        Node *now = array[destination];
                        while (now && now->next) now = now->next;
                        if (now) now->next = ptr;
                        else array[destination] = ptr;
                        now = ptr;
                        ptr = ptr->next;
                        while (ptr && equal(ptr->value, now->value)) {
                            now = ptr;
                            ptr = ptr->next;
                        }
                        now->next = nullptr;
                    }
                }
                Deallocate_n(old);
            }
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        void MultiHashtable<Key, Hash_Code, Equal_>::clear() {
            while (buckets) {
                Node *ptr = array[--buckets];
                while (ptr) {
                    Node *del = ptr;
                    ptr = ptr->next;
                    Deallocate(del);
                }
                array[buckets] = nullptr;
            }
            size_ = 0;
            array = Allocate_n<Node *>(8);
            Memset<Node *>(array, nullptr, 8);
            buckets = 8;
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        template<typename ...Input_Key>
        typename MultiHashtable<Key, Hash_Code, Equal_>::Iterator
        MultiHashtable<Key, Hash_Code, Equal_>::emplace(Input_Key &&...args) {
            Node *ptr = Allocate<Node>(forward<args>(args)...);
            Size index = insert(ptr);
            return get_Iterator(index, ptr);
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHashtable<Key, Hash_Code, Equal_>::Iterator
        MultiHashtable<Key, Hash_Code, Equal_>::insert(const Key &key) {
            Node *ptr = Allocate<Node>(key);
            Size index = insert(ptr);
            return get_Iterator(index, ptr);
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        bool MultiHashtable<Key, Hash_Code, Equal_>::erase(const Key &key) {
            auto index = hasher(key) % buckets;
            Node *ptr = array[index], *last = nullptr;
            bool flag = false;
            while (ptr && !equal(ptr->value, key)) {
                last = ptr;
                ptr = ptr->next;
            }
            while (ptr && equal(ptr->value, key)) {
                --size_;
                flag = true;
                Node *temp = ptr;
                ptr = ptr->next;
                Deallocate(temp);
            }
            if (flag) {
                if (last) last->next = ptr;
                else array[index] = ptr;
            }
            return flag;
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        void MultiHashtable<Key, Hash_Code, Equal_>::erase(const Iterator &iter) {
            if (!iter.target) return;
            auto index = hasher(iter.target->value) % buckets;
            Node *ptr = array[index], *last = nullptr;
            while (ptr && ptr != iter.target) {
                last = ptr;
                ptr = ptr->next;
            }
            if (last) last->next = iter.target->next;
            else array[index] = iter.target->next;
            Deallocate(iter.target);
            --size_;
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        void MultiHashtable<Key, Hash_Code, Equal_>::erase(const cIterator &iter) {
            erase(iter.target);
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        void MultiHashtable<Key, Hash_Code, Equal_>::erase(const rIterator &iter) {
            erase(iter.target);
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        void MultiHashtable<Key, Hash_Code, Equal_>::erase(const crIterator &iter) {
            erase(iter.target);
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHashtable<Key, Hash_Code, Equal_>::Iterator
        MultiHashtable<Key, Hash_Code, Equal_>::find(const Key &key) {
            return get(key);
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHashtable<Key, Hash_Code, Equal_>::cIterator
        MultiHashtable<Key, Hash_Code, Equal_>::find(const Key &key) const {
            return cIterator(get(key));
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        Size MultiHashtable<Key, Hash_Code, Equal_>::count(const Key &key) const {
            Iterator temp = get(key);
            if (temp == end()) return 0;
            Node *ptr = temp.target;
            Size count = 0;
            while (ptr && equal(key, ptr->value)) {
                ++count;
                ptr = ptr->next;
            }
            return count;
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        bool MultiHashtable<Key, Hash_Code, Equal_>::contains(const Key &key) const {
            return get(key) != end();
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHashtable<Key, Hash_Code, Equal_>::Self
        &MultiHashtable<Key, Hash_Code, Equal_>::operator=(const Self &other) {
            if (this == &other) return *this;
            clear();
            Deallocate(array);
            buckets = other.buckets;
            array = Allocate_n<Node *>(buckets);
            Memset<Node *>(array, nullptr, buckets);
            Iterator begin = other.begin(), end = other.end();
            while (begin != end) {
                insert(*begin);
                ++begin;
            }
            return *this;
        }

        template<typename Key, typename Hash_Code, typename Equal_>
        typename MultiHashtable<Key, Hash_Code, Equal_>::Self
        &MultiHashtable<Key, Hash_Code, Equal_>::operator=(Self &&other) noexcept {
            if (this == &other) return *this;
            clear();
            Deallocate(array);
            size_ = other.size_;
            buckets = other.buckets;
            array = other.array;
            other.size_ = 0;
            other.buckets = 8;
            other.array = Allocate_n<Node *>(8);
            Memset<Node *>(other.array, nullptr, 8);
            return *this;
        }

    }

}

#endif //TINYSTL_MULTIHASHTABLE_HPP
