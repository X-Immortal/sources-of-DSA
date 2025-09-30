//
// Created by xyx on 2025/9/30.
//
#include <iostream>
#include "Priority.h"
#include "../Queue.h"

using std::cout;
using std::endl;

template<class T>
class HeapPriorityQueue : public Queue<T> {
    static_assert(std::is_base_of_v<Priority, T>, "T must inherit from Priority");
    static_assert(std::is_copy_constructible_v<T>, "T must have copy constructor");
    static_assert(std::is_copy_assignable_v<T>, "T must be copy assignable");

    // 用数组实现大顶堆
    T **array;
    int capacity;
    int size;

public:
    explicit HeapPriorityQueue(int capacity) : capacity(capacity), size(0) {
        array = new T *[capacity];
    }

    ~HeapPriorityQueue() override {
        for (int i = 0; i < size; i++) {
            delete array[i];
        }
        delete[] array;
    }

    bool offer(const T &value) override {
        if (isFull()) {
            return false;
        }
        int new_elem = size++;
        int parent_elem = parent(new_elem);
        T *temp;
        while (parent_elem >= 0 && array[parent_elem]->priority() < value.priority()) {
            temp = array[parent_elem];
            array[parent_elem] = array[new_elem];
            array[new_elem] = temp;
            new_elem = parent_elem;
            parent_elem = parent(new_elem);
        }
        array[new_elem] = new T(value);
        return true;
    }

    T poll() override;

    T peek() const override;

    bool isEmpty() const override {
        return size == 0;
    }

    bool isFull() const override {
        return size == capacity;
    }

private:
    int parent(int i) {
        return i == 0 ? -1 : (i - 1) /2;
    }

    int left(int i) {
        int ret = i * 2 + 1;
        return ret >= size ? -1 : ret;
    }

    int right(int i) {
        int ret = i * 2 + 2;
        return ret >= size ? -1 : ret;
    }
};