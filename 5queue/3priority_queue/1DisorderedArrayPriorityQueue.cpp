//
// Created by xyx on 2025/9/27.
//
#include <iostream>
#include "../1queue/Queue.h"
#include "Priority.h"
#include <algorithm>
#include <string>

using std::cout;
using std::endl;
using std::string;

template<class T>
class DisorderedArrayPriorityQueue : public Queue<T> {
    static_assert(std::is_base_of_v<Priority, T>, "T must inherit from Priority");

    T *array;
    int capacity;
    int size;

public:
    explicit DisorderedArrayPriorityQueue(int capacity) : capacity(capacity), size(0) {
        array = new T[capacity];
    }

    ~DisorderedArrayPriorityQueue() override {
        delete[] array;
    }

    bool offer(T value) override {
        if (isFull()) {
            return false;
        }
        array[size++] = value;
        return true;
    }

    T poll() override {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return remove(findMax());
    }

    T peek() override {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return array[findMax()];
    }

    bool isEmpty() override {
        return size == 0;
    }

    bool isFull() override {
        return size == capacity;
    }

private:
    int findMax() {
        if (isEmpty()) {
            return -1;
        }
        int max_index = 0;
        for (int i = 1; i < size; i++) {
            if (array[i].priority() > array[max_index].priority()) {
                max_index = i;
            }
        }
        return max_index;
    }

    T remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        T ret = array[index];
        for (int i = index; i < size - 1; i++) {
            array[i] = array[i + 1];
        }
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

    int priority() override {
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