//
// Created by xyx on 25-7-19.
//
#include <iostream>
#include <cstring>

using namespace std;

int javaBinarySearch(const int *arr, int len, int target) {
    int low = 0, high = len - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] < target) {
            low = mid + 1;
        } else if (arr[mid] > target) {
            high = mid - 1;
        } else {
            return mid;
        }
    }
    // 如果搜索不到，返回：-插入点 - 1
    return -(low + 1);
}

int main() {
    int arr[] = {2, 5, 8};
    int ret = javaBinarySearch(arr, 3, 4);
    if (ret < 0) {
        int insertPoint = -ret - 1;
        int newArr[4];
        memcpy(newArr, arr, insertPoint * sizeof(int));
        newArr[insertPoint] = 4;
        memcpy(newArr + insertPoint + 1, arr + insertPoint, (3 - insertPoint) * sizeof(int));
        for (int i : newArr) {
            cout << i << " ";
        }
        cout << endl;
    } else {
        cout << "Found " << arr[ret] << " at " << ret << endl;
    }
    return 0;
}