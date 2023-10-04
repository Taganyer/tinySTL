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
//#include "STD/Stack.hpp"
#include "STD/Queue.hpp"
#include "STD/List.hpp"
#include "STD/Array.hpp"
#include "STD/Detail/Hashcode.hpp"
#include "STD/Detail/Red_Black_Tree.hpp"
#include "STD/Detail/AVL_Tree.hpp"
#include <iostream>

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


    void Vector_test() {
        Vector<int> test(10, 0),
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
    }

    void String_test() {
        String test("012345678"), test1(10, 'a'),
                test2{'b', 'c', 'd', 'e', 'f'}, test3(test.rbegin(), test.rend());
        test.replace(test.crbegin(), test.crbegin() + 5, "asdfghjkl");

        test *= 1000;
//        cout << *test3.insert(test3.cend(), test) << endl;
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
        Red_Black_Tree<int, int> test;
        for (int i = 0; i < 400; ++i) {
            if (i % 2)
                test.insert(i, i);
            else
                test.insert(-i, i);
        }
        for (auto &t: test) {
            cout << t.first << ends;
        }
        for (int i = 0; i < 200; ++i) {
            if (i % 2)
                test.erase(i);
            else
                test.erase(-i);
        }
        for (auto &t: test) {
            cout << t.first << ends;
        }
        cout << endl << test.size() << endl;
    }

    void AVL_Tree_Test() {
        AVL_Tree<int, int> test;
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
            bool judge = false;
            if (i % 2)
                judge = test.erase(i);
            else
                judge = test.erase(-i);
        }
        int i = 0;
        for (auto &t: test) {
            cout << t.first << ends;
            ++i;
        }
        cout << endl << test.size() << endl << i <<endl;
    }
}

#endif //TINYSTL_TEST_HPP
