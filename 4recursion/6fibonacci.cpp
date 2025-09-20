//
// Created by xyx on 25-7-25.
//
#include <iostream>
#include <vector>

using namespace std;

// 时间复杂度：Θ(1.618^n)
// 指数时间，是一个很差的算法
int fibonacci(int n) {
    if (n == 0 || n == 1) {
        return n;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}


// 记忆法优化时间复杂度(剪枝)
// 时间复杂度：O(n)
// 空间复杂度：O(n)
int fibonacci_memo(int n, vector<int> &cache) {
    if (n >= cache.size()) {
        cache.push_back(fibonacci_memo(n - 1, cache) + fibonacci_memo(n - 2, cache));
    }
    return cache[n];
}

int fibonacci_memo(int n) {
    vector<int> cache;
    cache.reserve(n);
    cache.push_back(0);
    cache.push_back(1);
    return fibonacci_memo(n, cache);
}

int main() {
    // cout << fibonacci(5) << endl;
    cout << fibonacci_memo(50) << endl;
    return 0;
}