//
// Created by xyx on 25-7-27.
//
#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

// 时间复杂度：O(2^n)
int element(int i, int j) {
    if (j == 0 || j == i) {
        return 1;
    }
    return element(i - 1, j - 1) + element(i - 1, j);
}

// 优化：二维数组记忆化
int element_memo(int i, int j, vector<vector<int> > &cache) {
    if (cache[i][j] > 0) {
        return cache[i][j];
    }
    if (j == 0 || j == i) {
        return cache[i][j] = 1;
    }
    return cache[i][j] = element(i - 1, j - 1) + element(i - 1, j);
}

// 进一步优化：只用一维数组(动态规划)
void createRow(int *row, int i) {
    if (i == 0) {
        row[0] = 1;
    }
    for (int j = i; j > 0; --j) {
        row[j] += row[j - 1];
    }
}

void test1(int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            cout << "  ";
        }
        for (int j = 0; j <= i; ++j) {
            printf("%-4d", element(i, j));
        }
        cout << endl;
    }
}

void test2(int n) {
    vector<vector<int> > cache;
    cache.reserve(n);
    for (int i = 0; i < n; ++i) {
        cache.emplace_back(i + 1);
        for (int j = 0; j < n - i - 1; ++j) {
            cout << "  ";
        }
        for (int j = 0; j <= i; ++j) {
            printf("%-4d", element_memo(i, j, cache));
        }
        cout << endl;
    }
}

void test3(int n) {
    int cache[n] = {};
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            cout << "  ";
        }
        createRow(cache, i);
        for (int j = 0; j <= i; ++j) {
            printf("%-4d", cache[j]);
        }
        cout << endl;
    }
}

int main() {
    // test1(10);
    // test2(10);
    test3(10);

    return 0;
}
