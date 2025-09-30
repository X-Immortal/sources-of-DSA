//
// Created by xyx on 2025/9/27.
//
#include <iostream>
#include "../Queue.h"
#include "Priority.h"
#include <algorithm>
#include <string>
#include <cstring>

using std::cout;
using std::endl;
using std::string;

template<class T>
class DisorderedArrayPriorityQueue : public Queue<T> {
    static_assert(std::is_base_of_v<Priority, T>, "T must inherit from Priority");
    static_assert(std::is_copy_constructible_v<T>, "T must have copy constructor");
    static_assert(std::is_copy_assignable_v<T>, "T must be copy assignable");

    T **array;
    int capacity;
    int size;

public:
    explicit DisorderedArrayPriorityQueue(int capacity) : capacity(capacity), size(0) {
        array = new T *[capacity];
    }

    ~DisorderedArrayPriorityQueue() override {
        for (int i = 0; i < size; i++) {
            delete array[i];
        }
        delete[] array;
    }

    // 时间复杂度: O(1)
    bool offer(const T &value) override {
        if (isFull()) {
            return false;
        }
        array[size++] = new T(value);
        return true;
    }

    // 时间复杂度: O(n)
    T poll() override {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return remove(findMax());
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return *array[findMax()];
    }

    bool isEmpty() const override {
        return size == 0;
    }

    bool isFull() const override {
        return size == capacity;
    }

private:
    int findMax() const {
        if (isEmpty()) {
            return -1;
        }
        int max_index = 0;
        for (int i = 1; i < size; i++) {
            if (array[i]->priority() > array[max_index]->priority()) {
                max_index = i;
            }
        }
        return max_index;
    }

    T remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        T ret = *array[index];
        delete array[index];
        memmove(array + index, array + index + 1, (size - 1 - index) * sizeof(*array));
        size--;
        return ret;
    }
};

template<class T>
using PriorityQueue = DisorderedArrayPriorityQueue<T>;

class Entry : public Priority {
    string value;
    int m_priority;

public:
    Entry() {
        m_priority = -1;
    }

    Entry(const string &value, int priority) : value(value), m_priority(priority) {}

    int priority() const override {
        return m_priority;
    }
};

int main() {
    PriorityQueue<Entry> queue(5);
    queue.offer(Entry("task1", 4));
    queue.offer(Entry("task2", 3));
    queue.offer(Entry("task3", 2));
    queue.offer(Entry("task4", 5));
    queue.offer(Entry("task5", 1));
    queue.offer(Entry("task6", 6));

    cout << queue.poll().priority() << endl;
    cout << queue.poll().priority() << endl;
    cout << queue.poll().priority() << endl;
    cout << queue.poll().priority() << endl;
    cout << queue.poll().priority() << endl;

    return 0;
}