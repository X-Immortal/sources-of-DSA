//
// Created by xyx on 25-7-25.
//
#include <iostream>

using namespace std;


// Master Theorem(主定理)：
// T(n) = aT(n/b) + f(n)
// 其中a为子问题的数量，b为子问题中数据规模的缩减比例，f(n)为非递归的时间成本。
// 设f(n) = n^c，比较n^c和n^log_b(a)，取较大者作为时间复杂度。
// 若两者相等，则时间复杂度为O((n^c)log(n))。

// a = 1, b = 2, c = log_2(a) = 0
// 时间复杂度：O(log(n))
int binarySearch(const int *arr, int target, int low, int high) {
    if (low > high) {
        return -1;
    }

    int mid = low + (high - low) / 2;
    if (arr[mid] > target) {
        return binarySearch(arr, target, low, mid - 1);
    }
    if (arr[mid] < target) {
        return binarySearch(arr, target, mid + 1, high);
    }
    return mid;
}

int main() {
    int arr[] = {7, 13, 21, 30, 38, 44, 52, 53};
    cout << binarySearch(arr, 30, 0, 7) << endl;
    return 0;
}