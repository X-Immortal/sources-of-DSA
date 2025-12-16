//
// Created by xyx on 25-7-25.
//
#include <iostream>

using namespace std;

// T(n) = T(n - 1) + n，T(1) = c
// T(n) = n(n+1)/2 + c - 1
// 时间复杂度：O(n^2)
void bubbleSort(int *arr, int len) {
    if (len <= 1) {
        return;
    }
    for (int i = 1; i < len; i++) {
        if (arr[i] < arr[i - 1]) {
            int temp = arr[i];
            arr[i] = arr[i - 1];
            arr[i - 1] = temp;
        }
    }
    bubbleSort(arr, len - 1);
}

// 优化版冒泡排序
void bubbleSort2(int *arr, int len) {
    if (len <= 1) {
        return;
    }
    // 发生最后一次交换时的索引，代表有序区域和无序区域的分界线
    int last_swap = 1;
    for (int i = 1; i < len; ++i) {
        if (arr[i] < arr[i - 1]) {
            int temp = arr[i];
            arr[i] = arr[i - 1];
            arr[i - 1] = temp;
            last_swap = i;
        }
    }
    bubbleSort2(arr, last_swap);
}

int main() {
    int arr[] = {5, 4, 3, 2, 1};
    bubbleSort(arr, 5);
    for (int i : arr) {
        cout << i << " ";
    }
    cout << endl;

    int arr2[] = {10, 8, 9, 7, 6, 5, 4, 3, 2, 1};
    bubbleSort2(arr2, 10);
    for (int i : arr2) {
        cout << i << " ";
    }
    cout << endl;
    return 0;
}