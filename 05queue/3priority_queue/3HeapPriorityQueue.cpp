//
// Created by xyx on 2025/9/30.
//
#include <iostream>
#include "Priority.h"
#include "../Queue.h"
#include <string>

using std::cout;
using std::endl;
using std::string;

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

    // 时间复杂度: O(log n)
    bool offer(const T &value) override {
        if (isFull()) {
            return false;
        }
        int child_index = size++;
        int parent_index = parent(child_index);
        while (parent_index >= 0 && array[parent_index]->priority() < value.priority()) {
            array[child_index] = array[parent_index];
            child_index = parent_index;
            parent_index = parent(child_index);
        }
        array[child_index] = new T(value);
        return true;
    }

    // 时间复杂度: O(log n)
    T poll() override {
        if (isEmpty()) {
            throw std::out_of_range("PriorityQueue is empty");
        }

        T *head = array[0];
        T *tail = array[--size];
        int tail_index = 0;
        int left_index = left(tail_index);
        int right_index = right(tail_index);

        while ((left_index >= 0 && tail->priority() < array[left_index]->priority()) ||
                (right_index >= 0 && tail->priority() < array[right_index]->priority())) {
            if (array[left_index]->priority() > tail->priority() &&
                (right_index < 0 || array[left_index]->priority() > array[right_index]->priority())) {
                array[tail_index] = array[left_index];
                tail_index = left_index;
            } else {
                array[tail_index] = array[right_index];
                tail_index = right_index;
            }
            left_index = left(tail_index);
            right_index = right(tail_index);
        }
        array[tail_index] = tail;

        T ret = *head;
        delete head;
        return ret;
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::out_of_range("PriorityQueue is empty");
        }
        return *array[0];
    }

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

template<class T>
using PriorityQueue = HeapPriorityQueue<T>;

class Entry : public Priority {
    string value;
    int m_priority;

public:
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