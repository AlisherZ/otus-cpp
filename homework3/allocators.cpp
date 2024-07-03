#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "allocator.hpp"
#include "container.hpp"

int factorial(int n);

int main(int, char **)
{
    std::map<int, int> m;
    for(int i = 0;i < 10;i++) {
        m[i] = factorial(i);
    }
    std::map<int, int, std::less<int>, pool_allocator<std::pair<const int, int> > > m1;
    for(int i = 0;i < 10;i++) {
        m1[i] = factorial(i);
    }
    for(auto it : m1) {
        std::cout << it.first << ' ' << it.second << std::endl;
    }
    MyContainer<int> c;
    for(int i = 0;i < 10;i++) {
        c.push_back(i);
    }
    for(int i = 0;i < 10;i++) {
        std::cout << c[i] << std::endl;
    }
    MyContainer<int, pool_allocator<int> > c1;
    for(int i = 0;i < 10;i++) {
        c1.push_back(i);
    }
    for(int i = 0;i < 10;i++) {
        std::cout << c1[i] << std::endl;
    }
    return 0;
}

int factorial(int n)
{
    if(n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}