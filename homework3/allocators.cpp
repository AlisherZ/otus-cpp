#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "allocator.hpp"

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
    return 0;
}

int factorial(int n)
{
    if(n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}