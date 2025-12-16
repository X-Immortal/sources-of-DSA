//
// Created by xyx on 2025/9/28.
//
#include <iostream>
#include "Priority.h"
#include "../Queue.h"
#include <string>

using std::cout;
using std::endl;
using std::string;

template<class T>
class OrderedArrayPriorityQueue : public Queue<T> {
    static_assert(std::is_base_of_v<Priority, T>, "T must inherit from Priority");
    static_assert(std::is_copy_constructible_v<T>, "T must have copy constructor");
    static_assert(std::is_copy_assignable_v<T>, "T must be copy assignable");

    T **array;
    int capacity;
    int size;

public:
    explicit OrderedArrayPriorityQueue(int capacity) : capacity(capacity), size(0) {
        array = new T *[capacity];
    }

    ~OrderedArrayPriorityQueue() override {
        for (int i = 0; i < size; i++) {
            delete array[i];
        }
        delete[] array;
    }

    // 时间复杂度: O(n)
    bool offer(const T &value) override {
        if (isFull()) {
            return false;
        }
        insert(value);
        return true;
    }

    // 时间复杂度: O(1)
    T poll() override {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        T ret = *array[--size];
        delete array[size];
        return ret;
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return *array[size - 1];
    }

    bool isEmpty() const override {
        return size == 0;
    }

    bool isFull() const override {
        return size == capacity;
    }

private:
    void insert(const T &elem) {
        int i;
        for (i = size - 1; i >= 0 && array[i]->priority() > elem.priority(); i--) {
            array[i + 1] = array[i];
        }
        array[i + 1] = new T(elem);
        size++;
    }
};

template<class T>
using PriorityQueue = OrderedArrayPriorityQueue<T>;

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
