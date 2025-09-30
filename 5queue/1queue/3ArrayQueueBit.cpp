//
// Created by xyx on 2025/9/13.
//

#include "../Queue.h"
#include <iostream>

using std::cout;
using std::endl;

template<class T>
class ArrayQueueBit : public Queue<T> {
    static_assert(std::is_copy_constructible_v<T>, "T must have copy constructor");

    T **array;
    int head = 0;
    int tail = 0;
    int real_capacity;
    int capacity;

public:
    explicit ArrayQueueBit(int capacity) {
        this->capacity = capacity;
        real_capacity = log2_ceil(capacity);
        array = new T *[real_capacity];
    }

    ~ArrayQueueBit() override {
        for (int i = head; i < tail; i++) {
            delete array[i & real_capacity - 1];
        }
        delete[] array;
    }

    bool offer(const T &value) override {
        if (isFull()) {
            return false;
        }
        // & 相当于 %
        array[tail++ & real_capacity - 1] = new T(value);
        return true;
    }

    T poll() override {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        T *elem = array[head & real_capacity - 1];
        head++;
        T ret = *elem;
        delete elem;
        return ret;
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::runtime_error("Queue is empty");
        }
        return *array[head & real_capacity - 1];
    }

    bool isEmpty() const override {
        return head == tail;
    }

    bool isFull() const override {
        return tail - head == capacity;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = int;
        using reference = T &;

        explicit Iterator(pointer cur, const ArrayQueueBit *queue) : cur(cur), queue(queue) {
        }

        reference operator*() const {
            return *queue->array[cur & queue->real_capacity - 1];
        }

        Iterator &operator++() {
            cur++;
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
        const ArrayQueueBit *queue;
    };

    Iterator begin() const {
        return Iterator(head, this);
    }

    Iterator end() const {
        return Iterator(tail, this);
    }

private:
    static int log2_ceil(int num) {
        num--;
        num |= num >> 1; // 10 -> 11
        num |= num >> 2; // 1100 -> 1111
        num |= num >> 4; // 0xF0 -> 0xFF
        num |= num >> 8; // 0xFF00 -> 0xFFFF
        num |= num >> 16; // 0xFFFF0000 -> 0xFFFFFFFF
        return num + 1;
    }
};

int main() {
    ArrayQueueBit<int> queue(3);
    queue.offer(1);
    queue.offer(2);
    queue.offer(3);
    queue.offer(4);
    queue.offer(5);
    for (auto i: queue) {
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