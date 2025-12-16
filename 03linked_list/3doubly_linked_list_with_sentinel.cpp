//
// Created by xyx on 25-7-24.
//
#include <iostream>
#include <functional>

using std::cout;
using std::endl;


// 带哨兵的双向链表
class DoublyLinkedListWithSentinel {
    class Node {
    public:
        Node *prev;
        int value;
        Node *next;

        explicit Node(int value, Node *prev = nullptr, Node *next = nullptr) : value(value), prev(prev), next(next) {}
    };

    Node *head = new Node(114514);
    Node *tail = new Node(1919810);
    int size = 0;

public:
    DoublyLinkedListWithSentinel() {
        head->next = tail;
        tail->prev = head;
    }

    DoublyLinkedListWithSentinel(const DoublyLinkedListWithSentinel &list) : size(list.size) {
        head->next = tail;
        tail->prev = head;
        for (Node *cur = list.head->next; cur != list.tail; cur = cur->next) {
            tail->prev = new Node(cur->value, tail->prev, tail);
            tail->prev->prev->next = tail->prev;
        }
    }

    ~DoublyLinkedListWithSentinel() {
        Node *cur = head;
        while (cur != nullptr) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
    }

    void insert(int index, int value) {
        Node *prev = find(index - 1), *next = prev->next;
        Node *node = new Node(value, prev, next);
        prev->next = next->prev = node;
        size++;
    }

    void addFirst(int value) {
        insert(0, value);
    }

    void addLast(int value) {
        insert(size, value);
    }

    int remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("index out of range");
        }
        Node *cur = find(index);
        Node *prev = cur->prev, *next = cur->next;
        prev->next = next;
        next->prev = prev;
        int ret = cur->value;
        delete cur;
        size--;
        return ret;
    }

    int removeFirst() {
        return remove(0);
    }

    int removeLast() {
        return remove(size - 1);
    }

    int get(int index) const {
        return find(index)->value;
    }

    void forEach(const std::function<void(int)> &func) const {
        for (Node *cur = head->next; cur != tail; cur = cur->next) {
            func(cur->value);
        }
    }

    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = int;
        using pointer = Node *;
        using reference = int &;

        explicit Iterator(pointer cur) : cur(cur) {}

        reference operator*() const {
            return cur->value;
        }

        Iterator &operator++() {
            cur = cur->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            cur = cur->next;
            return temp;
        }

        Iterator &operator--() {
            cur = cur->prev;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            cur = cur->prev;
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
        return Iterator(tail);
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

private:
    Node *find(int index) const {
        if (index < -1 || index >= size) {
            throw std::out_of_range("index out of range");
        }
        if (index <= size / 2) {
            Node *cur = head;
            for (int i = -1; i < index; ++i) {
                cur = cur->next;
            }
            return cur;
        } else {
            Node *cur = tail;
            for (int i = size; i > index; --i) {
                cur = cur->prev;
            }
            return cur;
        }
    }
};

void test1() {
    DoublyLinkedListWithSentinel list;
    list.addFirst(1);
    list.addFirst(2);
    list.addFirst(3);
    list.addFirst(4);
    list.forEach([](int value) {cout << value << " ";});
    cout << endl;

    list.addLast(5);
    list.addLast(6);
    list.addLast(7);
    list.addLast(8);
    for (auto elem : list) {
        cout << elem << " ";
    }
    cout << endl;

    DoublyLinkedListWithSentinel list2(list);
    list2.insert(0, 9);
    list2.insert(list.getSize(), 10);
    list2.insert(5, 11);
    list.forEach([](int value) {cout << value << " ";});
    cout << endl;
    list2.forEach([](int value) {cout << value << " ";});
    cout << endl;
}

void test2() {
    DoublyLinkedListWithSentinel list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);
    list.addLast(5);

    for (auto elem : list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.removeLast() << endl;
    for (auto elem : list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.removeFirst() << endl;
    for (auto elem : list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.remove(1) << endl;
    for (auto elem : list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.removeLast() << endl;
    cout << list.removeLast() << endl;
    cout << list.remove(10) << endl;
}

int main() {
    // test1();
    test2();
    return 0;
}