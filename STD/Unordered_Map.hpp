//
// Created by 86152 on 2023/10/1.
//

#ifndef TINYSTL_UNORDERED_MAP_HPP
#define TINYSTL_UNORDERED_MAP_HPP

#include "Detail/Hashtable.hpp"

namespace STD {

    namespace Detail {
        template<typename Key, typename Val, typename Hash>
        struct Unordered_Map_Hash_Helper {
            Hash calculate;

            explicit Unordered_Map_Hash_Helper(Hash calculate = Hash()) : calculate(calculate) {};

            Size operator()(const Pair<const Key, Val> &target) const {
                return calculate(target.first);
            }
        };

        template<typename Key, typename Val, typename Equal_>
        struct Unordered_Map_equal_Helper {
            Equal_ equal;

            explicit Unordered_Map_equal_Helper(Equal_ equal = Equal_()) : equal(equal) {};

            bool operator()(const Pair<const Key, Val> &left,
                            const Pair<const Key, Val> &right) const {
                return equal(left.first, right.first);
            }
        };
    }


    template<typename Key, typename Val, typename Hash_Code = Hash<Key>, typename Equal_ = Equal<Key>>
    class Unordered_Map : public Detail::Hashtable<Pair<const Key, Val>,
            Detail::Unordered_Map_Hash_Helper<Key, Val, Hash_Code>,
            Detail::Unordered_Map_equal_Helper<Key, Val, Equal_>> {
    public:
        using Hash = Detail::Unordered_Map_Hash_Helper<Key, Val, Hash_Code>;

        using Eql = Detail::Unordered_Map_equal_Helper<Key, Val, Equal_>;

        using Map_Type = Pair<const Key, Val>;

        using Key_Type = Key;

        using Value_Type = Val;

        using Basic = Detail::Hashtable<Map_Type, Hash, Eql>;

        using Self = Unordered_Map<Key, Val, Hash_Code, Equal_>;

        using Iterator = typename Basic::Iterator;

        using cIterator = typename Basic::cIterator;

        using rIterator = typename Basic::rIterator;

        using crIterator = typename Basic::crIterator;

        explicit Unordered_Map(Hash_Code hash = Hash_Code(), Equal_ equal = Equal_()) :
                Basic(Hash(hash), Eql(equal)), hasher(hash), equal(equal) {};

        Unordered_Map(const std::initializer_list<Map_Type> &list, Hash_Code hash = Hash_Code(),
                      Equal_ equal = Equal_());

        template<typename Key_Input, typename Val_Input>
        Unordered_Map(Key_Input key_begin, const Key_Input &key_end, Val_Input val_begin,
                      Hash_Code hash = Hash_Code(), Equal_ equal = Equal_());

        Unordered_Map(const Self &other) : Basic(other), hasher(other.hasher), equal(other.equal) {};

        Unordered_Map(Self &&other) : Basic(move(other)), hasher(other.hasher), equal(other.equal) {};

        ~Unordered_Map() = default;

        Pair<Iterator, bool> insert(const Key &key, const Val &val);

        Pair<Iterator, bool> insert(const Key &key, Val &&val);

        Pair<Iterator, bool> insert(Key &&key, const Val &val);

        Pair<Iterator, bool> insert(Key &&key, Val &&val);

        bool erase(const Key &key);

        Iterator find(const Key &target) const;

        Val &operator[](const Key &target) const;

        Self &operator=(const Self &other) {
            Basic::operator=(other);
            return *this;
        };

        Self &operator=(Self &&other) noexcept {
            Basic::operator=(move(other));
            return *this;
        };

        void swap(Self &other) noexcept {
            Basic::swap(other);
        };

        friend void swap(Self &left, Self &right) noexcept {
            left.Basic::swap(right);
        };

    private:
        using Node = typename Basic::Node;

        using Basic::size_;

        using Basic::buckets;

        using Basic::array;

        Hash_Code hasher;

        Equal_ equal;

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Unordered_Map<Key, Val, Hash_Code, Equal_>::Unordered_Map
            (const std::initializer_list<Map_Type> &list, Hash_Code hash, Equal_ equal) :
            Basic(list, Hash(hash), Eql(equal)), hasher(hash), equal(equal) {}

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    template<typename Key_Input, typename Val_Input>
    Unordered_Map<Key, Val, Hash_Code, Equal_>::Unordered_Map
            (Key_Input key_begin, const Key_Input &key_end, Val_Input val_begin, Hash_Code hash, Equal_ equal) :
            Basic(Hash(hash), Eql(equal)), hasher(hash), equal(equal) {
        Deallocate_n(array);
        Size temp = calculate_Length(key_begin, key_end);
        temp += temp / 5;
        if (temp < 8) temp = 8;
        buckets = temp;
        array = Allocate_n<Node *>(buckets);
        Memset<Node *>(array, nullptr, buckets);
        while (key_begin != key_end) {
            auto ptr = Allocate<Node>(*key_begin, *val_begin);
            Basic::insert(ptr);
            ++key_begin, ++val_begin;
        }
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Pair<typename Unordered_Map<Key, Val, Hash_Code, Equal_>::Iterator, bool>
    Unordered_Map<Key, Val, Hash_Code, Equal_>::insert(const Key &key, const Val &val) {
        Size record = size_;
        Node *ptr = Allocate<Node>(key, val);
        Size index = Basic::insert(ptr);
        return {Basic::get_Iterator(index, ptr), record != size_};
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Pair<typename Unordered_Map<Key, Val, Hash_Code, Equal_>::Iterator, bool>
    Unordered_Map<Key, Val, Hash_Code, Equal_>::insert(const Key &key, Val &&val) {
        Size record = size_;
        Node *ptr = Allocate<Node>(key, move(val));
        Size index = Basic::insert(ptr);
        return {Basic::get_Iterator(index, ptr), record != size_};
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Pair<typename Unordered_Map<Key, Val, Hash_Code, Equal_>::Iterator, bool>
    Unordered_Map<Key, Val, Hash_Code, Equal_>::insert(Key &&key, const Val &val) {
        Size record = size_;
        Node *ptr = Allocate<Node>(move(key), val);
        Size index = Basic::insert(ptr);
        return {Basic::get_Iterator(index, ptr), record != size_};
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Pair<typename Unordered_Map<Key, Val, Hash_Code, Equal_>::Iterator, bool>
    Unordered_Map<Key, Val, Hash_Code, Equal_>::insert(Key &&key, Val &&val) {
        Size record = size_;
        Node *ptr = Allocate<Node>(move(key), move(val));
        Size index = Basic::insert(ptr);
        return {Basic::get_Iterator(index, ptr), record != size_};
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    bool Unordered_Map<Key, Val, Hash_Code, Equal_>::erase(const Key &key) {
        auto index = hasher(key) % buckets;
        Node *ptr = array[index], *last = nullptr;
        while (ptr) {
            if (equal(ptr->value.first, key)) {
                if (last) last->next = ptr->next;
                else array[index] = ptr->next;
                Deallocate(ptr);
                --size_;
                return true;
            }
            last = ptr;
            ptr = ptr->next;
        }
        return false;
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_Map<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_Map<Key, Val, Hash_Code, Equal_>::find(const Key &target) const {
        Size index = hasher(target) % buckets;
        Node *ptr = array[index];
        while (ptr) {
            if (equal(target, ptr->value.first)) {
                return Basic::get_Iterator(index, ptr);
            }
            ptr = ptr->next;
        }
        return Basic::end();
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Val &Unordered_Map<Key, Val, Hash_Code, Equal_>::operator[](const Key &target) const {
        Size index = hasher(target) % buckets;
        Node *ptr = array[index];
        while (ptr) {
            if (equal(target, ptr->value.first)) {
                return Basic::get_Iterator(index, ptr);
            }
            ptr = ptr->next;
        }
        return (*insert(target, Val()).first).second;
    }

}

#endif //TINYSTL_UNORDERED_MAP_HPP
