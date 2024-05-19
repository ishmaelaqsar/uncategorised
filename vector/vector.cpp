#include "vector.hpp"
#include <iostream>

using namespace std;

template<typename T>
void print(string s, local::vector<T>);

int main()
{
    auto v1 = local::vector<int>{5};
    print("v1"s, v1);
    for (int i = 0; i < v1.capacity(); ++i) {
        v1.push_back(i);
    }
    print("v1"s, v1);
    v1.push_back(5);
    print("v1"s, v1);
    cout << "v1 capacity: " << v1.capacity() << endl;

    local::vector<int> v2{0, 1, 2, 3, 4};
    print("v2"s, v2);
    v2.push_back(5);
    v2.push_back(6);
    print("v2"s, v2);

    local::vector<int> v3{1, 2, 3};
    print("v3"s, v3);
    auto v4 = v3.map([](int x) { return x * 2; });
    print("v4"s, v4);
}

template<typename T>
void print(string s, local::vector<T> v)
{
    cout << s << ": ";
    for (auto& i : v) {
        cout << i << ", ";
    }
    cout << endl;
}
