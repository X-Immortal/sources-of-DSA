//
// Created by xyx on 25-7-25.
//
#include <iostream>

using namespace std;

void insertionSort(int *arr, int len) {
    if (len <= 1) {
        return;
    }
    insertionSort(arr, len - 1);
    int temp = arr[len - 1];
    int i;
    for (i = len - 1; i > 0 && arr[i - 1] > temp; --i) {
        arr[i] = arr[i - 1];
    }
    arr[i] = temp;
}

int main() {
    int arr[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    insertionSort(arr, 10);
    for (int i = 0; i < 10; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
    return 0;
}