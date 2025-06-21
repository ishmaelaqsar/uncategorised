#include "vector.hpp"
#include <iostream>

using namespace std;

template<typename T>
void print(const string& s, const local::vector<T> &v);

int main()
{
        auto v1 = local::vector{5};
        print("v1"s, v1);
        for (int i = 0; i < 5; ++i) {
                v1.push_back(i);
        }
        print("v1"s, v1);
        v1.push_back(5);
        print("v1"s, v1);
        cout << "v1 capacity: " << v1.capacity() << endl;

        local::vector v2{0, 1, 2, 3, 4};
        print("v2"s, v2);
        v2.push_back(5);
        v2.push_back(6);
        print("v2"s, v2);

        local::vector v3{1, 2, 3};
        print("v3"s, v3);
        const auto v4 = v3.map([](const int x) {
                return x * 2;
        });
        print("v4"s, v4);
}

template<typename T>
void print(const string& s, const local::vector<T> &v)
{
        cout << s << ": ";
        for (auto &i: v) {
                cout << i << ", ";
        }
        cout << endl;
}
