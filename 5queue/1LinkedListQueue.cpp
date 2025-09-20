//
// Created by xyx on 2025/9/13.
//

#include "Queue.h"
#include <iostream>
#include <climits>

using std::cout;
using std::endl;

template<class T>
class LinkedListQueue : public Queue<T> {
    class Node {
        friend class LinkedListQueue;

        T value;
        Node *next;

    public:
        explicit Node(T value = T(), Node *next = nullptr) : value(value), next(next) {}
    };

    Node *head;
    Node *tail;
    int size;
    const int capacity;

public:
    explicit LinkedListQueue(int capacity = INT_MAX) : size(0), capacity(capacity) {
        tail->next = head = tail = new Node;
    }

    ~LinkedListQueue() override {
        Node *cur = head;
        while (cur != tail) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
        delete tail;
    }

    bool offer(T value) override {
        if (isFull()) {
            return false;
        }
        tail->next = new Node(value, head);
        tail = tail->next;
        size++;
        return true;
    }

    T poll() override {
        if (isEmpty()) {
            throw std::runtime_error("queue is empty!");
        }
        Node *node = head->next;
        head->next = node->next;
        T ret = node->value;
        if (node == tail) {
            tail = head;
        }
        delete node;
        size--;
        return ret;
    }

    T peek() override {
        if (isEmpty()) {
            throw std::runtime_error("queue is empty!");
        }
        return head->next->value;
    }

    bool isEmpty() override {
        return head == tail;
    }

    bool isFull() override {
        return size >= capacity;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = Node *;
        using reference = T&;

        explicit Iterator(pointer cur) : cur(cur) {
        }

        reference operator*() const {
            return cur->value;
        }

        Iterator &operator++() {
            cur = cur->next;
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
    };

    Iterator begin() const {
        return Iterator(head->next);
    }

    Iterator end() const {
        return Iterator(head);
    }
};

int main() {
    LinkedListQueue<int> queue(3);
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