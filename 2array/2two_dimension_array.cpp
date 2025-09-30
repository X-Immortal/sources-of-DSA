//
// Created by xyx on 25-7-22.
//
#include <iostream>

using std::cout;
using std::endl;

// cpu读取内存时，会将读取到的数据及其邻近的数据存储到缓存中
// 缓存的读写速度比内存更快
// 先行后列的遍历能充分利用缓存，从而提高效率

int main() {
    int arr[][5] = {
        {1, 2, 3, 4, 5},
        {10, 20, 30, 40, 50},
        {100, 200, 300, 400, 500}
    };
    // 先行后列的遍历，效率更高
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    // 先列后行的遍历，效率较低
    for (int j = 0; j < 5; j++) {
        for (int i = 0; i < 3; i++) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}