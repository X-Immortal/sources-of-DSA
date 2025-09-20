//
// Created by xyx on 25-7-25.
//
#include <iostream>

using namespace std;

// 递归过深时容易爆栈(StackOverFlow)
// T(n) = T(n - 1) + c，不适用主定理，应用直接展开法
// T(n) = cn
// 时间复杂度：O(n)
int sum(int n) {
    if (n == 1) {
        return 1;
    }
    return n + sum(n - 1);
}

// 改为尾递归写法，某些编译器可以自动优化，在调用完外层函数后会提前释放栈空间
int sum_tailrec(int n, int accumulator = 0) {
    if (n == 1) {
        return accumulator + 1;
    }
    return sum_tailrec(n - 1, accumulator + n);
}

// 要从根本上避免爆栈，应该避免递归，改用迭代(循环)
int sum_iter(int n) {
    int sum = 0;
    while (n > 0) {
        sum += n;
        n--;
    }
    return sum;
}

int main() {
    cout << sum(30000) << endl;
    return 0;
}