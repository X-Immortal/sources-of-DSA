//
// Created by xyx on 2025/9/26.
//
#include <iostream>
#include "Deque.h"

using std::cout;
using std::endl;

template<class T>
class ArrayDeque : public Deque<T> {
    T *array;
    int capacity;
    int head;
    int tail;

public:
    explicit ArrayDeque(int capacity) : capacity(capacity) {
        array = new T[capacity + 1];
        head = tail = 0;
    }

    ~ArrayDeque() override {
        delete[] array;
    }

    bool offerFirst(T e) override {
        if (isFull()) {
            return false;
        }
        array[head = dec(head)] = e;
        return true;
    }

    bool offerLast(T e) override {
        if (isFull()) {
            return false;
        }
        array[tail] = e;
        tail = inc(tail);
        return true;
    }

    T pollFirst() override {
        if (isEmpty()) {
            throw std::runtime_error("deque is empty!");
        }
        T ret = array[head];
        head = inc(head);
        return ret;
    }

    T pollLast() override {
        if (isEmpty()) {
            throw std::runtime_error("deque is empty!");
        }
        return array[tail = dec(tail)];
    }

    T peekFirst() override {
        if (isEmpty()) {
            throw std::runtime_error("deque is empty!");
        }
        return array[head];
    }

    T peekLast() override {
        if (isEmpty()) {
            throw std::runtime_error("deque is empty!");
        }
        return array[dec(tail)];
    }

    bool isEmpty() override {
        return head == tail;
    }

    bool isFull() override {
        return tail - head == capacity || head - tail == 1;
    }

    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = int;
        using reference = T&;

        explicit Iterator(pointer cur, const ArrayDeque *queue) : cur(cur) , deque(queue) {
        }

        reference operator*() const {
            return deque->array[cur];
        }

        Iterator &operator++() {
            cur = deque->inc(cur);
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator & operator--() {
            cur = deque->dec(cur);
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
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
        const ArrayDeque *deque;
    };

    Iterator begin() const {
        return Iterator(head, this);
    }

    Iterator end() const {
        return Iterator(tail, this);
    }

private:
    int inc(int i) const {
        return ++i > capacity ? 0 : i;
    }

    int dec(int i) const {
        return --i < 0 ? capacity : i;
    }
};

int main() {
    ArrayDeque<int> deque(3);
    deque.offerFirst(1);
    deque.offerFirst(2);
    deque.offerLast(3);
    deque.offerLast(4);
    for (auto i : deque) {
        cout << i << " ";
    }
    cout << endl;

    cout << deque.peekFirst() << endl;
    cout << deque.peekLast() << endl;
    cout << deque.pollFirst() << endl;
    cout << deque.pollLast() << endl;
    cout << deque.pollFirst() << endl;
    cout << deque.pollLast() << endl;
    return 0;
}