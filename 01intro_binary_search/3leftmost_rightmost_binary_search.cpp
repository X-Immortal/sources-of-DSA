//
// Created by xyx on 25-7-20.
//
#include <iostream>
#include <cassert>

using namespace std;

// 重复元素返回最左边的索引
int leftmostBinarySearch(const int *arr, int len, int target) {
    int left = 0, right = len - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] >= target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    // 返回值表示数组中>=target的元素的最小索引
    return left;
}

// 重复元素返回最右边的索引
int rightmostBinarySearch(const int *arr, int len, int target) {
    int left = 0, right = len - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] > target) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    // 返回值表示数组中<=target的元素中的最大索引
    return right;
}


int main() {
    int arr[] = {1, 2, 4, 4, 4, 5, 6, 7};
    assert(leftmostBinarySearch(arr, 8, 1) == 0);
    assert(leftmostBinarySearch(arr, 8, 2) == 1);
    assert(leftmostBinarySearch(arr, 8, 4) == 2);
    assert(leftmostBinarySearch(arr, 8, 5) == 5);
    assert(leftmostBinarySearch(arr, 8, 6) == 6);
    assert(leftmostBinarySearch(arr, 8, 7) == 7);
    assert(leftmostBinarySearch(arr, 8, 0) == -1);
    assert(leftmostBinarySearch(arr, 8, 3) == -1);
    assert(leftmostBinarySearch(arr, 8, 8) == -1);

    assert(rightmostBinarySearch(arr, 8, 1) == 0);
    assert(rightmostBinarySearch(arr, 8, 2) == 1);
    assert(rightmostBinarySearch(arr, 8, 4) == 4);
    assert(rightmostBinarySearch(arr, 8, 5) == 5);
    assert(rightmostBinarySearch(arr, 8, 6) == 6);
    assert(rightmostBinarySearch(arr, 8, 7) == 7);
    assert(rightmostBinarySearch(arr, 8, 0) == -1);
    assert(rightmostBinarySearch(arr, 8, 3) == -1);
    assert(rightmostBinarySearch(arr, 8, 8) == -1);
    return 0;
}