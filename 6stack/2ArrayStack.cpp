//
// Created by xyx on 2025/9/14.
//

#include "Stack.h"
#include <iostream>

using std::cout;
using std::endl;

template<class T>
class ArrayStack : public Stack<T> {
    static_assert(std::is_copy_constructible_v<T>, "T must have copy constructor");

    T **array;
    int capacity;
    int size = 0;

public:
    explicit ArrayStack(int capacity) : capacity(capacity) {
        array = new T *[capacity];
    }

    ~ArrayStack() override {
        for (int i = 0; i < size; i++) {
            delete array[i];
        }
        delete[] array;
    }

    bool push(const T &value) override {
        if (isFull()) {
            return false;
        }
        array[size++] = new T(value);
        return true;
    }

    T pop() override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        T ret = *array[--size];
        delete array[size];
        return ret;
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return *array[size - 1];
    }

    bool isEmpty() const override {
        return size == 0;
    }

    bool isFull() const override {
        return size >= capacity;
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using pointer = int;
        using reference = T&;

        explicit Iterator(pointer cur, const ArrayStack *queue) : cur(cur) , queue(queue) {
        }

        reference operator*() const {
            return *queue->array[cur];
        }

        Iterator &operator++() {
            cur--;
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
        const ArrayStack *queue;
    };

    Iterator begin() const {
        return Iterator(size - 1, this);
    }

    Iterator end() const {
        return Iterator(-1, this);
    }
};

int main() {
    ArrayStack<int> stack(3);
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