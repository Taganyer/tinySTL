//
// Created by 86152 on 2023/9/30.
//

#ifndef TINYSTL_HASHCODE_HPP
#define TINYSTL_HASHCODE_HPP

#include "../Type_traits.hpp"
#include "../Config.hpp"
#include "../String.hpp"

namespace STD {

    template<typename Arg>
    class Vector;

    template<typename Arg>
    class List;

    template<typename Arg>
    class Deque;

    template<typename Arg>
    class Forward_List;

    template<typename Arg, Size size_>
    class Array;

    template<typename Arg>
    struct Hash;

//----------------------------------------------------------------------------------------------------------------------

    //MurmurHash是一种经过广泛测试且速度很快的非加密哈希函数，这里用的是64A的版本。
    //key : 对象的地址
    //len : 对象的字节长度
    //seed : 随机种子，可减少冲突
    Size MurmurHash64A(const void *key, Size len, unsigned int seed) {
        const Size m = 0xc6a4a7935bd1e995;
        const int r = 47;

        Size hashcode = seed ^ (len * m);
        const auto *data = (const Size *) key;
        const Size *end = data + (len / 8);
        //将指针强转为Size(unsigned long long)指针，从底层对其进行哈希操作。
        while (data != end) {
            Size k = *data++;
            k *= m;
            k ^= k >> r;
            k *= m;
            hashcode ^= k;
            hashcode *= m;
        }

        const auto *data2 = (const unsigned char *) data;
        //处理剩余下的字节（也就是len % 8个字节）。
        switch (len & 7) {
            case 7:
                hashcode ^= Size(data2[6]) << 48;
            case 6:
                hashcode ^= Size(data2[5]) << 40;
            case 5:
                hashcode ^= Size(data2[4]) << 32;
            case 4:
                hashcode ^= Size(data2[3]) << 24;
            case 3:
                hashcode ^= Size(data2[2]) << 16;
            case 2:
                hashcode ^= Size(data2[1]) << 8;
            case 1:
                hashcode ^= Size(data2[0]);
                hashcode *= m;
        }

        hashcode ^= hashcode >> r;
        hashcode *= m;
        hashcode ^= hashcode >> r;
        return hashcode;
    }

    //常见的哈希算法，但看了几篇文章了解到数据量过大时函数计算hashcode的自然溢出会导致最终的
    //哈希值大量冲突，效果不如MurmurHash好。
    Size BKDRHash(const void *key, Size len) {
        int hashcode = 0;
        const auto *data = static_cast<const char *>(key);
        if (len > 0) {
            for (int i = 0; i < len; ++i) {
                hashcode = 31 * hashcode + data[i];
            }
        }
        return hashcode;
    }

//----------------------------------------------------------------------------------------------------------------------

    //针对迭代器的hashcode的计算方法
    template<typename Input_iterator>
    struct Iterator_Hash {
        Size operator()(Input_iterator begin, const Input_iterator &end, unsigned int seed = 1) const {
            Size hashcode = 0;
            Hash<typename Iterator_traits<Input_iterator>::Value_type> hasher;
            while (begin != end) {
                hashcode ^= hasher(*begin, seed) + 0x9e3779b9 + (hashcode << 6) + (hashcode >> 2);
                ++begin;
            }
            return hashcode;
        }
    };

    //这是一个为没有提供Hash函数的类的通用版本的Hash函数，但会存在一定隐患，自定义类型如果不能
    //在其成员中直接反应其类型的实例对象的异同性，Hash函数就会造成不同对象之间的hashcode相同的
    //冲突，或相同对象出现不同的hashcode的现象。
    //key : 对象的地址
    //seed : 随机种子，可减少冲突，默认为1
    template<typename Arg, typename Range>
    struct Hash_Helper {
        Size operator()(const Arg &target, unsigned int seed = 1) const {
            return MurmurHash64A(&target, sizeof(target), seed);
        }
    };

    //内置类型的偏特化版本
    template<typename Arg>
    struct Hash_Helper<Arg, True_type> {
        Size operator()(const Arg &target, unsigned int = 1) const {
            return target;
        }
    };

    template<typename Arg>
    struct Hash : Hash_Helper<Arg, typename Is_integral_Helper<Arg>::Type> {
        using Hash_Helper<Arg, typename Is_integral_Helper<Arg>::Type>::operator();
    };

    //STD容器的偏特化版本
    template<>
    struct Hash<String> {
        Size operator()(const String &target, unsigned int seed = 1) const {
            return MurmurHash64A(target.c_str(), target.size(), seed);
        };
    };

    template<typename Arg>
    struct Hash<Vector<Arg>> {
        Size operator()(const Vector<Arg> &target, unsigned int seed = 1) const {
            Iterator_Hash<typename Vector<Arg>::Iterator> hasher;
            return hasher(target.begin(), target.end(), seed);
        };
    };

    template<typename Arg>
    struct Hash<List<Arg>> {
        Size operator()(const List<Arg> &target, unsigned int seed = 1) const {
            Iterator_Hash<typename List<Arg>::Iterator> hasher;
            return hasher(target.begin(), target.end(), seed);
        };
    };

    template<typename Arg>
    struct Hash<Deque<Arg>> {
        Size operator()(const Deque<Arg> &target, unsigned int seed = 1) const {
            Iterator_Hash<typename Deque<Arg>::Iterator> hasher;
            return hasher(target.begin(), target.end(), seed);
        };
    };

    template<typename Arg>
    struct Hash<Forward_List<Arg>> {
        Size operator()(const Forward_List<Arg> &target, unsigned int seed = 1) const {
            Iterator_Hash<typename Forward_List<Arg>::Iterator> hasher;
            return hasher(target.begin(), target.end(), seed);
        };
    };

    template<typename Arg, Size size_>
    struct Hash<Array<Arg, size_>> {
        Size operator()(const Array<Arg, size_> &target, unsigned int seed = 1) const {
            return MurmurHash64A(target.data(), size_ * sizeof(Arg), seed);
        };
    };


//----------------------------------------------------------------------------------------------------------------------

}

#endif //TINYSTL_HASHCODE_HPP
