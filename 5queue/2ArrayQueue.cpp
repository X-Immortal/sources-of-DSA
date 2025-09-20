//
// Created by xyx on 2025/9/13.
//

#include "Queue.h"
#include <iostream>

using std::cout;
using std::endl;

template<class T>
class ArrayQueue : public Queue<T> {
    T *array;
    int head = 0;
    int tail = 0;
    int capacity;
    int size = 0;

public:
    explicit ArrayQueue(int capacity) : capacity(capacity) {
        array = new T[capacity + 1];
    }

    ~ArrayQueue() override {
        delete[] array;
    }

    bool offer(T value) override {
        if (isFull()) {
            return false;
        }
        array[tail] = value;
        tail = (tail + 1) % (capacity + 1);
        size++;
        return true;
    }

    T poll() override {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        int index = head;
        head = (head + 1) % (capacity + 1);
        size--;
        return array[index];
    }

    T peek() override {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return array[head];
    }

    bool isEmpty() override {
        return head == tail;
    }

    bool isFull() override {
        return (tail + 1) % (capacity + 1) == head;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = int;
        using reference = T&;

        explicit Iterator(pointer cur, const ArrayQueue *queue) : cur(cur) , queue(queue) {
        }

        reference operator*() const {
            return queue->array[cur];
        }

        Iterator &operator++() {
            cur = (cur + 1) % (queue->capacity + 1);
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator &other) const {
            return cur == other.cur;
        }

        bool operator!=(const Iterator &other) const {
            return !(*this == other);
        }

    private:
        pointer cur;
        const ArrayQueue *queue;
    };

    Iterator begin() const {
        return Iterator(head, this);
    }

    Iterator end() const {
        return Iterator(tail, this);
    }
};

int main() {
    ArrayQueue<int> queue(3);
    queue.offer(1);
    queue.offer(2);
    queue.offer(3);
    queue.offer(4);
    queue.offer(5);
    for (auto i : queue) {
        cout << i << " ";
    }
    cout << endl;

    cout << queue.peek() << endl;

    cout << queue.poll() << endl;
    cout << queue.poll() << endl;
    cout << queue.poll() << endl;
    cout << queue.poll() << endl;
    cout << queue.poll() << endl;

    return 0;
}