//
// Created by xyx on 2025/10/1.
//
#include <iostream>
#include <cstring>
#include "MaxHeap.h"

#define DEBUG 0

using std::cout;
using std::endl;

MaxHeap::MaxHeap(int capacity) : size(0), capacity(capacity) {
    array = new int[capacity];
}

MaxHeap::MaxHeap(int *array, int size) : capacity(size), size(size) {
    this->array = new int[size];
    memcpy(this->array, array, size * sizeof(*array));
    heapify();
}

MaxHeap::~MaxHeap() {
    delete[] array;
}

// Floyd建堆算法：
// 先找到最后一个非叶子节点
// 然后从这个节点开始，从后往前依次执行下潜
// 时间复杂度为O(n)
// t = \sigma_{i=1}^h {2^{h-i}*(i-1)}
// = 2^h - h - 1
// = n - log(n) - 1
void MaxHeap::heapify() {
    for (int i = parent(size - 1); i >= 0; --i) {
        down(i);
    }
}

int MaxHeap::poll() {
    return poll(0);
}

int MaxHeap::poll(int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    swap(index, --size);
    down(index);
    return array[size];
}

int MaxHeap::peek() const {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty");
    }
    return array[0];
}

bool MaxHeap::offer(int value) {
    if (isFull()) {
        return false;
    }
    array[size++] = value;
    up(size - 1);
    return true;
}

void MaxHeap::replace(int value) {
    if (isEmpty()) {
        throw std::out_of_range("Heap is empty");
    }
    array[0] = value;
    down(0);
}

bool MaxHeap::isEmpty() const {
    return size == 0;
}

bool MaxHeap::isFull() const {
    return size == capacity;
}

void MaxHeap::up(int index) {
    for (int parent_i = parent(index);
         parent_i >= 0 && array[parent_i] < array[index];
         index = parent_i, parent_i = parent(index)) {
        swap(parent_i, index);
    }
}

void MaxHeap::down(int index) {
    int left_i, right_i;
    int max_i = index;
    while (true) {
        left_i = left(index);
        right_i = right(index);
        if (left_i >= 0 && array[left_i] > array[max_i]) {
            max_i = left_i;
        }
        if (right_i >= 0 && array[right_i] > array[max_i]) {
            max_i = right_i;
        }
        if (max_i == index) {
            break;
        }
        swap(index, max_i);
        index = max_i;
    }
}

void MaxHeap::swap(int i, int j) {
    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}

int MaxHeap::parent(int index) const {
    return index == 0 ? -1 : (index - 1) / 2;
}

int MaxHeap::left(int index) const {
    int ret = index * 2 + 1;
    return ret >= size ? -1 : ret;
}

int MaxHeap::right(int index) const {
    int ret = index * 2 + 2;
    return ret >= size ? -1 : ret;
}

#if DEBUG == 1
int main() {
    int arr[] = {4, 3, 2, 5, 1, 6, 0, 0, 0, 0};
    MaxHeap heap(arr, std::size(arr));

    heap.poll(9);
    heap.poll(8);
    heap.poll(7);
    heap.poll(6);

    heap.offer(9);
    heap.offer(8);
    heap.offer(7);
    heap.offer(10);

    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    cout << heap.poll() << endl;
    return 0;
}
#endif
