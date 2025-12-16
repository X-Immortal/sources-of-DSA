//
// Created by xyx on 2025/9/26.
//
#include <iostream>
#include "Deque.h"
#include <climits>

using std::cout;
using std::endl;

template<class T>
class LinkedListDeque : public Deque<T> {
    static_assert(std::is_copy_constructible_v<T>, "T must have copy constructor");

    class Node {
    public:
        Node *prev;
        T *value;
        Node *next;

        explicit Node(const T *value = nullptr, Node *prev = nullptr, Node *next = nullptr) : prev(prev), next(next) {
            if (value == nullptr) {
                this->value = nullptr;
            } else {
                this->value = new T(*value);
            }
        }

        ~Node() {
            delete value;
        }
    };

    int capacity;
    int size;
    Node *sentinel;

public:
    explicit LinkedListDeque(int capacity = INT_MAX) : size(0), capacity(capacity) {
        sentinel = new Node;
        sentinel->next = sentinel->prev = sentinel;
    }

    ~LinkedListDeque() override {
        Node *cur = sentinel->next;
        while (cur != sentinel) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
        delete sentinel;
    }

    bool offerFirst(const T &e) override {
        if (isFull()) {
            return false;
        }
        Node *node = new Node(&e, sentinel, sentinel->next);
        node->prev->next = node->next->prev = node;
        size++;
        return true;
    }

    bool offerLast(const T &e) override {
        if (isFull()) {
            return false;
        }
        Node *node = new Node(&e, sentinel->prev, sentinel);
        node->prev->next = node->next->prev = node;
        size++;
        return true;
    }

    T pollFirst() override {
        if (isEmpty()) {
            throw std::runtime_error("deque is empty!");
        }
        Node *node = sentinel->next;
        node->next->prev = sentinel;
        sentinel->next = node->next;
        T ret = *node->value;
        delete node;
        size--;
        return ret;
    }

    T pollLast() override {
        if (isEmpty()) {
            throw std::runtime_error("deque is empty!");
        }
        Node *node = sentinel->prev;
        node->prev->next = sentinel;
        sentinel->prev = node->prev;
        T ret = *node->value;
        delete node;
        size--;
        return ret;
    }

    T peekFirst() const override {
        if (isEmpty()) {
            throw std::runtime_error("deque is empty!");
        }
        return *sentinel->next->value;
    }

    T peekLast() const override {
        if (isEmpty()) {
            throw std::runtime_error("deque is empty!");
        }
        return *sentinel->prev->value;
    }

    bool isEmpty() const override {
        return size == 0;
    }

    bool isFull() const override {
        return size == capacity;
    }

    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = Node *;
        using reference = T&;

        explicit Iterator(pointer cur) : cur(cur) {
        }

        reference operator*() const {
            return *cur->value;
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

        Iterator &operator--() {
            cur = cur->prev;
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
    };

    Iterator begin() const {
        return Iterator(sentinel->next);
    }

    Iterator end() const {
        return Iterator(sentinel);
    }
};

int main() {
    LinkedListDeque<int> deque(5);
    deque.offerFirst(1);
    deque.offerFirst(2);
    deque.offerFirst(3);
    deque.offerLast(4);
    deque.offerLast(5);
    deque.offerLast(6);
    for (auto i : deque) {
        cout << i << " ";
    }
    cout << endl;

    cout << deque.pollFirst() << endl;
    cout << deque.pollFirst() << endl;
    cout << deque.pollLast() << endl;
    cout << deque.pollLast() << endl;
    cout << deque.pollLast() << endl;
    cout << deque.pollLast() << endl;
    cout << deque.isEmpty() << endl;
    return 0;
}