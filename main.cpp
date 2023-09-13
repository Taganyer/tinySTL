#include <vector>
#include <iostream>
#include "STD/String.hpp"
#include "STD/Vector.hpp"
#include "STD/List.hpp"

using namespace std;
using namespace STD;

class A {
public:
    virtual void show() const {
        cout << "class A" << endl;
    }
};

class B : public A {
public:
    void show() const {
        cout << "class B" << endl;
    }

    int test = 1;
};

int main() {
    List<int> test{1, 2, 3, 4, 5}, test2(10, 9);
    for (auto i : test) cout << i << ends;
    cout << endl;
    for (auto i : test2) cout << i << ends;
    cout << endl;
    test.insert(test.begin(), 1);
    for (auto i : test) cout << i << ends;
    cout << endl;
    return 0;
}
