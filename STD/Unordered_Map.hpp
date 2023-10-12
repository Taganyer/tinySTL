//
// Created by 86152 on 2023/10/1.
//

#ifndef TINYSTL_UNORDERED_MAP_HPP
#define TINYSTL_UNORDERED_MAP_HPP

#include "Detail/Hashtable.hpp"
#include "Detail/MultiHashtable.hpp"

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

        ~Unordered_Map() override = default;

        using Basic::insert;

        Pair<Iterator, bool> insert(const Key &key, const Val &val);

        Pair<Iterator, bool> insert(const Key &key, Val &&val);

        Pair<Iterator, bool> insert(Key &&key, const Val &val);

        Pair<Iterator, bool> insert(Key &&key, Val &&val);

        using Basic::erase;

        bool erase(const Key &key);

        Iterator find(const Key &key);

        cIterator find(const Key &key) const;

        Iterator at(const Key &key);

        cIterator at(const Key &key) const;

        Val &operator[](const Key &key);

        Size count(const Key &key) const;

        bool contains(const Key &key) const;

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

        Iterator get(const Key &key) const;

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_Map<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_Map<Key, Val, Hash_Code, Equal_>::get(const Key &key) const {
        Size index = hasher(key) % buckets;
        Node *ptr = array[index];
        while (ptr) {
            if (equal(key, ptr->value.first)) {
                return Basic::get_Iterator(index, ptr);
            }
            ptr = ptr->next;
        }
        return Basic::end();
    }

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
    Unordered_Map<Key, Val, Hash_Code, Equal_>::find(const Key &key) {
        return get(key);
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_Map<Key, Val, Hash_Code, Equal_>::cIterator
    Unordered_Map<Key, Val, Hash_Code, Equal_>::find(const Key &key) const {
        return cIterator(get(key));
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_Map<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_Map<Key, Val, Hash_Code, Equal_>::at(const Key &key) {
        Iterator iter = get(key);
        if (iter == Basic::end())
            throw outOfRange("You passed an out-of-range basic in Unordered_Map::at()");
        return iter;
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_Map<Key, Val, Hash_Code, Equal_>::cIterator
    Unordered_Map<Key, Val, Hash_Code, Equal_>::at(const Key &key) const {
        Iterator iter = get(key);
        if (iter == Basic::end())
            throw outOfRange("You passed an out-of-range basic in Unordered_Map::at()");
        return cIterator(iter);
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Val &Unordered_Map<Key, Val, Hash_Code, Equal_>::operator[](const Key &key) {
        Iterator iter = get(key);
        if (iter != Basic::end()) return iter;
        return (*insert(key, Val()).first).second;
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Size Unordered_Map<Key, Val, Hash_Code, Equal_>::count(const Key &key) const {
        return contains(key);
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    bool Unordered_Map<Key, Val, Hash_Code, Equal_>::contains(const Key &key) const {
        return get(key) != Basic::end();
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Hash_Code = Hash<Key>, typename Equal_ = Equal<Key>>
    class Unordered_MultiMap : public Detail::MultiHashtable<Pair<const Key, Val>,
            Detail::Unordered_Map_Hash_Helper<Key, Val, Hash_Code>,
            Detail::Unordered_Map_equal_Helper<Key, Val, Equal_>> {
    public:
        using Hash = Detail::Unordered_Map_Hash_Helper<Key, Val, Hash_Code>;

        using Eql = Detail::Unordered_Map_equal_Helper<Key, Val, Equal_>;

        using Map_Type = Pair<const Key, Val>;

        using Key_Type = Key;

        using Value_Type = Val;

        using Basic = Detail::MultiHashtable<Map_Type, Hash, Eql>;

        using Self = Unordered_MultiMap<Key, Val, Hash_Code, Equal_>;

        using Iterator = typename Basic::Iterator;

        using cIterator = typename Basic::cIterator;

        using rIterator = typename Basic::rIterator;

        using crIterator = typename Basic::crIterator;
        explicit Unordered_MultiMap(Hash_Code hash = Hash_Code(), Equal_ equal = Equal_()) :
                Basic(Hash(hash), Eql(equal)), hasher(hash), equal(equal) {};

        Unordered_MultiMap(const std::initializer_list<Map_Type> &list, Hash_Code hash = Hash_Code(),
                      Equal_ equal = Equal_());

        template<typename Key_Input, typename Val_Input>
        Unordered_MultiMap(Key_Input key_begin, const Key_Input &key_end, Val_Input val_begin,
        Hash_Code hash = Hash_Code(), Equal_ equal = Equal_());

        Unordered_MultiMap(const Self &other) : Basic(other), hasher(other.hasher), equal(other.equal) {};

        Unordered_MultiMap(Self &&other) : Basic(move(other)), hasher(other.hasher), equal(other.equal) {};

        ~Unordered_MultiMap() override = default;

        using Basic::insert;

        Iterator insert(const Key &key, const Val &val);

        Iterator insert(const Key &key, Val &&val);

        Iterator insert(Key &&key, const Val &val);

        Iterator insert(Key &&key, Val &&val);

        using Basic::erase;

        bool erase(const Key &key);

        Iterator find(const Key &key);

        cIterator find(const Key &key) const;

        Size count(const Key &key) const;

        bool contains(const Key &key) const;

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

        Iterator get(const Key &key) const;

    };

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::get(const Key &key) const {
        Size index = hasher(key) % buckets;
        Node *ptr = array[index];
        while (ptr) {
            if (equal(key, ptr->value.first)) {
                return Basic::get_Iterator(index, ptr);
            }
            ptr = ptr->next;
        }
        return Basic::end();
    }

//----------------------------------------------------------------------------------------------------------------------

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::Unordered_MultiMap
            (const std::initializer_list<Map_Type> &list, Hash_Code hash, Equal_ equal) :
            Basic(list, Hash(hash), Eql(equal)), hasher(hash), equal(equal) {}

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    template<typename Key_Input, typename Val_Input>
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::Unordered_MultiMap
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
    typename Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::insert(const Key &key, const Val &val) {
        Node *ptr = Allocate<Node>(key, val);
        Size index = Basic::insert(ptr);
        return Basic::get_Iterator(index, ptr);
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::insert(const Key &key, Val &&val) {
        Node *ptr = Allocate<Node>(key, move(val));
        Size index = Basic::insert(ptr);
        return Basic::get_Iterator(index, ptr);
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::insert(Key &&key, const Val &val) {
        Node *ptr = Allocate<Node>(move(key), val);
        Size index = Basic::insert(ptr);
        return Basic::get_Iterator(index, ptr);
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::insert(Key &&key, Val &&val) {
        Node *ptr = Allocate<Node>(move(key), move(val));
        Size index = Basic::insert(ptr);
        return Basic::get_Iterator(index, ptr);
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    bool Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::erase(const Key &key) {
        auto index = hasher(key) % buckets;
        Node *ptr = array[index], *last = nullptr;
        bool flag = false;
        while (ptr && !equal(ptr->value.first, key)) {
            last = ptr;
            ptr = ptr->next;
        }
        while (ptr && equal(ptr->value.first, key)) {
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

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::Iterator
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::find(const Key &key) {
        return get(key);
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    typename Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::cIterator
    Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::find(const Key &key) const {
        return cIterator(get(key));
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    Size Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::count(const Key &key) const {
        Iterator temp = get(key);
        if (temp == Basic::end()) return 0;
        Node *ptr = temp.target;
        Size count = 0;
        while (ptr && equal(key, ptr->value.first)) {
            ++count;
            ptr = ptr->next;
        }
        return count;
    }

    template<typename Key, typename Val, typename Hash_Code, typename Equal_>
    bool Unordered_MultiMap<Key, Val, Hash_Code, Equal_>::contains(const Key &key) const {
        return get(key) != Basic::end();
    }

}

#endif //TINYSTL_UNORDERED_MAP_HPP
