//
// Created by xyx on 2025/9/14.
//

#include "Stack.h"
#include <iostream>
#include <climits>

using std::cout;
using std::endl;

template<class T>
class LinkedListStack : public Stack<T> {
    struct Node {
        T value;
        Node *next;

        explicit Node(T value = T(), Node *next = nullptr) : value(value), next(next) {
        }
    };

    int capacity;
    int size = 0;
    Node *head = new Node;

public:
    explicit LinkedListStack(int capacity = INT_MAX) : capacity(capacity) {
    }

    ~LinkedListStack() override {
        Node *cur = head;
        while (cur != nullptr) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
    }

    bool push(T value) override {
        if (isFull()) {
            return false;
        }
        head->next = new Node(value, head->next);
        size++;
        return true;
    }

    T pop() override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node *node = head->next;
        head->next = node->next;
        size--;
        T value = node->value;
        delete node;
        return value;
    }

    T peek() override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return head->next->value;
    }

    bool isEmpty() override {
        return size == 0;
    }

    bool isFull() override {
        return size >= capacity;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = Node *;
        using reference = T &;

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
        return Iterator(nullptr);
    }
};

int main() {
    LinkedListStack<int> stack(3);
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    for (auto i: stack) {
        cout << i << " ";
    }
    cout << endl;

    cout << stack.pop() << endl;
    cout << stack.pop() << endl;
    cout << stack.pop() << endl;
    cout << stack.pop() << endl;
    return 0;
}
