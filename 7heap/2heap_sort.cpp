//
// Created by xyx on 2025/10/1.
//
#include <iostream>
#include "MaxHeap.h"

using namespace std;

// 利用heapify和down实现排序
// 时间复杂度O(nlog(n))
void head_sort(int *arr, int len) {
    MaxHeap heap(arr, len);
    for (int i = len - 1; i >= 0; --i) {
        arr[i] = heap.poll();
    }
}

int main() {
    int arr[] = {4, 3, 2, 5, 1, 6};
    head_sort(arr, std::size(arr));
    for (int i = 0; i < std::size(arr); ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}