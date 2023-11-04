//
// Created by 86152 on 2023/9/27.
//

#ifndef TINYSTL_TEST_HPP
#define TINYSTL_TEST_HPP

#include "STD/Vector.hpp"
#include "STD/String.hpp"
#include "STD/Forward_List.hpp"
#include "STD/Deque.hpp"
#include "STD/Algorithms.hpp"
#include "STD/Stack.hpp"
#include "STD/Queue.hpp"
#include "STD/List.hpp"
#include "STD/Array.hpp"
#include "STD/Detail/Hashcode.hpp"
#include "STD/Detail/Red_Black_Tree.hpp"
#include "STD/Detail/AVL_Tree.hpp"
#include "STD/Map.hpp"
#include "STD/Set.hpp"
#include "STD/Detail/Hashtable.hpp"
#include "STD/Unordered_Map.hpp"
#include "STD/Unordered_Set.hpp"
#include "STD/Detail/MultiRed_Black_Tree.hpp"
#include "STD/Memory.hpp"
#include <iostream>
#include "STD/Tuple.hpp"
#include "STD/BitSet.hpp"
#include <algorithm>
#include <vector>
#include <memory>
#include <chrono>
#include <fstream>
#include <bitset>

namespace STD {
    using std::cout;
    using std::ends;
    using std::endl;

    void Vector_test();

    void List_test();

    void Forward_List_test();

    void Deque_test();

    void String_test();

    void Priority_queue_test();

    void Hash_test();

    void Red_Black_Tree_test();

    void AVL_Tree_Test();

    void Map_test();

    void Set_test();

    void Hashtable_Test();

    void Unordered_Map_Test();

    void MultiRed_Black_Tree_test();

    void MultiMap_test();

    void MultiSet_test();

    void Ptr_test();

    void Tuple_test();

    void BitSet_test();

    void Speed_Test();

    void Algorithms_test();


    void Vector_test() {
        Vector<int> test(10, 0),
                test1{6, 7, 8, 9, 10},
                test2(test1.begin(), test1.end()),
                test3(test);

        //11010
        cout << (test == test3) << (test < test1) << (test > test1) << (test <= test1) << (test >= test1) << endl;

        for (int i = 0; i < 100; ++i) test.push_back(i);

        for (auto i: test) {
            cout << i << ends;
        }
        cout << "size: " << test.size() << endl;

        for (int i = 0; i < test1.size(); ++i) {
            cout << test1[i] << ends;
        }
        cout << "capacity: " << test1.capacity() << endl;
        test.push_back(4, 5);
        auto iter1 = test.insert(test.crbegin(), 4, 7);
        auto iter2 = test.cbegin();
        auto iter3 = test1.rbegin();
        auto iter4 = test1.crend();
        auto iter5 = test1.rend();

        while (iter1 != test.crend()) {
            cout << *iter1++ << ends;
        }
        cout << endl;

        while (iter3 != iter5) {
            cout << *iter3++ << ends;
        }
        cout << endl;

        while (iter4 != test1.crbegin()) {
            cout << *--iter4 << ends;
        }
        cout << endl;

        test.erase(test.begin(), test.end());

        for (auto i: test) {
            cout << i << ends;
        }
    }

    void List_test() {
        List<int> test(10, 0), test1 = {1, 2, 3, 4, 5, 6};
        auto iter = test.insert(test.rbegin(), {10, 9, 8});
        swap(test, test1);
        test = test1;
        cout << *iter << endl;
        for (const auto &i: test) {
            cout << i << ends;
        }
        cout << endl;
        test1.erase(test1.begin(), --test1.end());
        for (const auto &i: test1) {
            cout << i << ends;
        }
        cout << endl;
    }

    void Forward_List_test() {
        Forward_List<int> test(10, 0), test1 = {1, 2, 3, 4, 5, 6};
        auto iter = test.insert_after(test.before_begin(), {10, 9, 8});
        swap(test, test1);
        test = test1;
        cout << *iter << endl;
        for (const auto &i: test) {
            cout << i << ends;
        }
        cout << endl;
        test1.erase_after(test1.before_begin(), test1.end());
        for (const auto &i: test1) {
            cout << i << ends;
        }
        cout << endl;
        iter = test.before_begin();
        for (int i = 0; i < 100; ++i) {
            if (iter == test.before_begin()) ++iter;
            cout << *iter << ends;
            ++iter;
        }
    }

    void Deque_test() {
        Deque<int> test(10, 0),
                test1{6, 7, 8, 9, 10},
                test2(test1.begin(), test1.end()),
                test3(test);

        //11010
        cout << (test == test3) << (test < test1) << (test > test1) << (test <= test1) << (test >= test1) << endl;

        for (auto i: test) {
            cout << i << ends;
        }
        cout << "size: " << test.size() << endl;

        for (int i = 0; i < test1.size(); ++i) {
            cout << test1[i] << ends;
        }
        cout << "capacity: " << test1.capacity() << endl;
        test.push_back(4, 5);
        auto iter1 = test.insert(test.cbegin(), 4, 7);
        auto iter2 = test.cbegin();
        auto iter3 = test1.rbegin();
        auto iter4 = test1.crend();
        auto iter5 = test1.rend();

        while (iter1 != test.cend()) {
            cout << *iter1++ << ends;
        }
        cout << endl;

        while (iter3 != iter5) {
            cout << *iter3++ << ends;
        }
        cout << endl;

        while (iter4 != test1.crbegin()) {
            cout << *--iter4 << ends;
        }
        cout << endl;

        test.erase(test.begin(), test.end());

        for (auto i: test) {
            cout << i << ends;
        }

        cout << (test == test1) << endl;
    }

    void String_test() {
        String test("012345678"), test1(10, 'a'),
                test2{'b', 'c', 'd', 'e', 'f'}, test3(test.rbegin(), test.rend());
        test.replace(test.crbegin(), test.crbegin() + 5, "asdfghjkl");

        test *= 1000;
        cout << *test3.insert(test3.cend(), test) << endl;
        Sort(test3.begin(), test3.end());
        cout << *Binary_Search(test3.begin(), test3.end(), '9') << endl;
        cout << (Hash<String>()(test)) << endl;
        cout << test1 << endl;
        cout << test2 << endl;
        cout << test3 << endl;
    }

    void Priority_queue_test() {
        Deque<int> container{7, 34, 5, 1, 0, -1, 45, 69, 34};
        Priority_queue<int> test(container.begin(), container.end());
        for (int i = 0; i < 100; ++i) {
            if (i % 2) test.push(i);
            else test.push(-i);
        }
        while (!test.empty()) {
            cout << test.front() << ends;
            test.pop();
        }
        cout << endl << test.size() << endl;
    }

    void Hash_test() {
        Hash<int> int_test;

        Hash<String> String_test;

        Hash<int *> p_test;

        Hash<const int *> cp_test;

        Hash<Vector<int>> v_test;

        int d1 = 999;

        String d2("the data........ \n");

        Vector<int> d3{1, 2, 3, 4}, d4(d3);

        int *p1 = &d1;

        const int *p2 = &d1;

        cout << int_test(d1) << endl;

        cout << String_test(d2) << endl;

        cout << p_test(p1) << endl;

        cout << cp_test(p2) << endl;

        cout << v_test(d3) << endl;

        cout << v_test(d4) << endl;

        auto ptr = new char[1000];

        ptr[0] = 1;

        ptr[1] = 1;

        cout << int(ptr[1000]) << endl;

        const auto *tp = (const unsigned long long *) ptr - 1;

        cout << *(tp) << endl;

        delete[] ptr;
    }

    void Red_Black_Tree_test() {
        Detail::Red_Black_Tree<int> test,
                test1{88, 77, 66};
        for (int i = 0; i < 400; ++i) {
            if (i % 2)
                test.insert(i);
            else
                test.insert(-i);
        }
//        for (auto &t: test) {
//            cout << t << ends;
//        }
        for (int i = 0; i < 200; ++i) {
            if (i == 129)
                cout << "test" << endl;
            if (i % 2)
                test.erase(i);
            else
                test.erase(-i);
            auto iter = test.begin(), end = test.end();
            int j = 0;
            while (iter != end) {
                ++iter;
                ++j;
                if (j > 399 - i) {
                    cout << i << endl;
                    break;
                }
            }
        }
        for (auto &t: test) {
            cout << t << ends;
        }
        cout << endl << test.size() << endl;
        swap(test, test1);
        test1 = test;
        cout << (test == test1);
    }

    void AVL_Tree_Test() {
        Detail::AVL_Tree<const int> test;
        for (int i = 0; i < 400; ++i) {
            if (i % 2)
                test.insert(i);
            else
                test.insert(-i);
        }
        for (auto &t: test) {
            cout << t << ends;
        }
        cout << endl;
        for (int i = 0; i < 200; ++i) {
            bool judge = false;
            if (i % 2)
                judge = test.erase(i);
            else
                judge = test.erase(-i);
            if (!judge) cout << i << ends;
        }
        cout << endl;
        int i = 0;
        for (auto &t: test) {
            cout << t << ends;
            ++i;
        }
        cout << endl << test.size() << endl << i << endl;
    }

    void Map_test() {
        Vector<int> d1{1, 2, 3, 4, 5, 6, 7, 8, 9, 0}, d2(10, 6);
        Map<int, int> test,
                test2(d1.crbegin(), d1.crend(), d2.begin()),
                test3 = Map<int, int>({{99, 2},
                                       {98, 3}});
        for (int i = 0; i < 400; ++i) {
            if (i % 2)
                test.insert(i, i);
            else
                test.insert(-i, i);
        }
        for (auto &t: test) {
            cout << t.first << ends;
        }
        cout << endl;
        for (int i = 0; i < 200; ++i) {
            if (i % 2)
                test.erase(i);
            else
                test.erase(-i);
        }
        test = move(Map<int, int>{{1, 2},
                                  {2, 3}});
        swap(test2, test3);
        int i = 0;
        for (auto &t: test) {
            cout << t.first << ends;
            ++i;
        }
        cout << endl << test.size() << endl << i << endl;
    }

    void Set_test() {
        auto comp = [](const int &lhs, const int &rhs) { return lhs > rhs; };
        Set<int, Greater<int>, Equal<int>> test,
                test1(test);
        for (int i = 0; i < 400; ++i) {
            if (i % 2)
                test.insert(i);
            else
                test.insert(-i);
        }
//        for (auto &t: test) {
//            cout << t << ends;
//        }
        for (int i = 0; i < 200; ++i) {
            if (i % 2)
                test.erase(i);
            else
                test.erase(-i);
            auto iter = test.begin(), end = test.end();
            int j = 0;
            while (iter != end) {
                ++iter;
                ++j;
                if (j > 399 - i) {
                    cout << i << endl;
                    break;
                }
            }
        }
        for (auto &t: test) {
            cout << t << ends;
        }
        cout << endl << test.size() << endl;
        swap(test, test1);
        test = move(test1);
        cout << (test == test1) << endl;
        auto iter = test.crbegin();
        while (iter != test.crend()) {
            cout << *iter << ends;
            ++iter;
        }
    }

    void Hashtable_Test() {
        Detail::Hashtable<int, Hash<int>, Equal<int>> test,
                test1(test);
        int arr[400] = {0};
        for (int i = 0; i < 400; ++i) {
            if (i % 2)
                test.insert(i);
            else
                test.insert(-i);
        }
        int i = 0;
        for (auto t: test) {
            if (t < 0) t = -t;
            if (t >= 400 || arr[t])
                cout << "error:" << t << ends;
            arr[t] = 1;
            ++i;
        }
        cout << endl << i << endl;
        for (int i = 0; i < 200; ++i) {
            bool judge = false;
            if (i % 2)
                judge = test.erase(i);
            else
                judge = test.erase(-i);
            if (!judge) cout << i << ends;
        }
        cout << endl;
        i = 0;
        auto iter = test.rend();
        while (iter != test.rbegin()) {
            cout << *--iter << ends;
            ++i;
        }
        cout << endl << test.size() << endl << i << endl;
    }

    void Unordered_Map_Test() {
        Vector<int> d1{1, 2, 3, 4, 5, 6, 7, 8, 9, 0}, d2(10, 6);
        Unordered_Map<int, int> test,
                test2(d1.crbegin(), d1.crend(), d2.begin()),
                test3{{99, 2},
                      {98, 3}};
        for (int i = 0; i < 400; ++i) {
            if (i % 2)
                test.insert(i, i);
            else
                test.insert(-i, i);
        }
        for (auto &t: test) {
            cout << t.first << ends;
        }
        cout << endl;
        for (int i = 0; i < 200; ++i) {
            if (i % 2)
                test.erase(i);
            else
                test.erase(-i);
        }
        swap(test, test2);
        int i = 0;
        for (auto &t: test) {
            cout << t.first << ends;
            ++i;
        }
        cout << endl << test.size() << endl << i << endl;
    }

    void MultiRed_Black_Tree_test() {
        Detail::MultiRed_Black_Tree<int> test;
        for (int i = 0; i < 100; ++i) {
            test.insert(i);
            test.insert(i);
        }
        for (int i = 0; i < 50; ++i) {
            test.erase(-- -- ++ ++test.find(i));
            test.erase(-- -- ++ ++test.find(i));
        }
        for (auto i: test) {
            cout << i << ends;
        }
    }

    void MultiMap_test() {
        MultiMap<int, String> test;
        for (int i = 0; i < 100; ++i) {
            test.insert(i, " - 1");
            test.insert(i, " - 2");
        }
        for (int i = 0; i < 50; ++i) {
            auto pair = test.equal_range(i);
            while (pair.first != pair.second) {
                auto temp = pair.first;
                ++pair.first;
                test.erase(temp);
                test.erase({i, nullptr});
            }
        }
        for (auto i: test) {
            cout << i.first << i.second << ends;
        }
    }

    void MultiSet_test() {
        Unordered_MultiSet<int> test{1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7}, test2(test);
        test.clear();
        for (int i = 0; i < 40; ++i) {
            test.insert(i);
            test.insert(i);
            test.insert(i);
        }
        for (int i = 0; i < 40; ++i) {
            test.erase(i);
        }
        for (auto i: test) {
            cout << i << ends;
        }
        cout << endl;
        test = move(test2);
        swap(test, test2);
    }

    void Ptr_test() {
        Shared_ptr<int> test(new int(1));
        Shared_ptr<int> test2(move(test));
        *test2;
        cout << *test << endl;
        cout << test.use_count() << endl;
        cout << test2.use_count() << endl;
        test2 = test;
        test == test2;
        struct A {
            using Type = int;

            virtual void show() {};
        };

        struct B : A {
            const int i = 99;

            void show() override {
                cout << i << endl;
            }
        };

        auto *temp = new B();
        {
            Shared_ptr<A> t1(temp), t2(new A());
            {
                Shared_ptr<void> t3(t1);
                {
                    Shared_ptr<B> t4 = dynamic_pointer_cast<B>(t1);
                    { Shared_ptr<A> t5(t4); }
                }
            }
        }
        Unique_ptr<A> ta = Unique_ptr<B>(nullptr);
    }

    void Tuple_test() {
        String a = "0000", b = "1111", c = "2222";
        Tuple<String, String, String> test{"0000", "1111", "2222"},
                test2(move(a), b, c);
        cout << get<0>(test) << get<1>(test) << get<2>(test) << sizeof(String) << endl;
        cout << endl;
    }

    void BitSet_test() {
        BitSet<64> test(-1);
        String temp = test.to_String();
        test = BitSet<64>(temp);
        cout << test.to_String() << endl;
    }

    void Speed_Test() {
        std::ofstream out("../Speed_Test.txt", std::ofstream::app);
        out << "总测试次数： 100 * 9\n" << endl;
        out << "----------------------------------------------------------------------------------------" << endl;
        for (int count = 0; count < 9; ++count) {
            int size = 10;
            for (int i = 0; i < count; ++i) size *= 10;
            Size time1 = 0, time2 = 0, time3 = 0;
            for (int t = 0; t < 100; ++t) {
                std::vector<int> test1;
                Vector<int> test2;
                List<int> test3;
                for (int i = 0; i < size; ++i) {
                    if (i % 2) {
                        test1.push_back(i);
                        test2.push_back(i);
                        test3.push_back(i);
                    } else {
                        test1.push_back(-i);
                        test2.push_back(-i);
                        test3.push_back(-i);
                    }
                }

                auto begin1 = std::chrono::high_resolution_clock::now();
                std::sort(test1.begin(), test1.end());
                auto end1 = std::chrono::high_resolution_clock::now();
                auto elapsed1 = std::chrono::duration_cast<std::chrono::nanoseconds>(end1 - begin1);

                auto begin2 = std::chrono::high_resolution_clock::now();
                Sort(test2.begin(), test2.end(), Less<int>());
                auto end2 = std::chrono::high_resolution_clock::now();
                auto elapsed2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end2 - begin2);

                auto begin3 = std::chrono::high_resolution_clock::now();
                Sort(test3.begin(), test3.end(), Less<int>());
                auto end3 = std::chrono::high_resolution_clock::now();
                auto elapsed3 = std::chrono::duration_cast<std::chrono::nanoseconds>(end3 - begin3);

                time1 += elapsed1.count();
                time2 += elapsed2.count();
                time3 += elapsed3.count();
            }

            out << "数据量： " << size << endl;
            out << "时间： " << time1 << " | " << time2 << " | " << time3 << endl;
            out << "比率： " << 1.0 << " | " << (double) time2 / (double) time1 << " | "
                << (double) time3 / (double) time1 << endl;
            out << "----------------------------------------------------------------------------------------" << endl;
            out.close();
            out.open("../Speed_Test.txt", std::ofstream::app);
        }
    }

    void Algorithms_test() {
        double t1 = 123456.789;
        float t2 = 11111.990001f;
        cout << Floor(t1) << endl << Floor(-t2) << endl;
        cout << To_String(-t1) << endl << To_String(t2) << endl;
        String target(To_String(MAX_DOUBLE));
        cout << Stod(target) << endl;
    }

}

#endif //TINYSTL_TEST_HPP
