//
// Created by xyx on 25-7-19.
//
#include <iostream>
#include <cassert>

using namespace std;

// 二分查找算法：
// 时间复杂度：O(log(n))
// 空间复杂度：O(1)

int binarySearch1(const int *arr, int len, int target) {
    // 左右均为闭
    int left = 0, right = len - 1;
    // 此处如果不取等，就无法搜索到两端的元素
    while (left <= right) {
        // 如果直接用 (left + right) / 2 会溢出
        int mid = left + (right - left) / 2;
        // 此处if-else的书写顺序决定了它是平衡的
        // 即不管目标在左边还是右边，判断次数都相同
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}

int binarySearch2(const int *arr, int len, int target) {
    // 左闭右开
    int left = 0, right = len;
    // 此处如果取等，当目标不存在时会陷入死循环
    while (left < right) {
        // 如果直接用 (left + right) / 2 会溢出
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) {
            return mid;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return -1;
}

int main() {
    int arr[] = {7, 13, 21, 30, 38, 44, 52, 53};
    assert(binarySearch1(arr, 8, 7) == 0);
    assert(binarySearch1(arr, 8, 13) == 1);
    assert(binarySearch1(arr, 8, 21) == 2);
    assert(binarySearch1(arr, 8, 30) == 3);
    assert(binarySearch1(arr, 8, 38) == 4);
    assert(binarySearch1(arr, 8, 44) == 5);
    assert(binarySearch1(arr, 8, 52) == 6);
    assert(binarySearch1(arr, 8, 53) == 7);
    assert(binarySearch1(arr, 8, 0) == -1);
    assert(binarySearch1(arr, 8, 15) == -1);
    assert(binarySearch1(arr, 8, 60) == -1);
    return 0;
}