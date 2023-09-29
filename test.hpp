//
// Created by 86152 on 2023/9/27.
//

#ifndef TINYSTL_TEST_HPP
#define TINYSTL_TEST_HPP

#include "STD/Vector.hpp"
//#include "STD/String.hpp"
//#include "STD/Forward_list.hpp"
#include "STD/Deque.hpp"
//#include "STD/Stack.hpp"
//#include "STD/Queue.hpp"
#include "STD/List.hpp"
#include "STD/Array.hpp"
#include <iostream>

namespace STD {
    using std::cout;
    using std::ends;
    using std::endl;

    void Vector_test();

    void List_test();

    void Deque_test();


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
        auto iter = test.insert(test.end(), {10, 10, 10});
        swap(test, test1);
        test = test1;
        cout << *iter << endl;
        for (const auto &i : test) {
            cout << i << ends;
        }
        cout << endl;
        test1.erase(test1.begin(), advance(test1.begin(), 7));
        for (const auto &i : test1) {
            cout << i << ends;
        }
        cout << endl;
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
}

#endif //TINYSTL_TEST_HPP
