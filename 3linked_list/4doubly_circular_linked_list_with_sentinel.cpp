//
// Created by xyx on 25-7-25.
//
#include <iostream>
#include <functional>

using std::cout;
using std::endl;

class DoublyCircularLinkedListWithSentinel {
    class Node {
        friend class DoublyCircularLinkedListWithSentinel;

        Node *prev;
        int value;
        Node *next;

        explicit Node(int value, Node *prev = nullptr, Node *next = nullptr) : prev(prev), value(value), next(next) {
        }
    };

    Node *head = new Node(114514);
    int size = 0;

public:
    DoublyCircularLinkedListWithSentinel() {
        head->next = head->prev = head;
    }

    DoublyCircularLinkedListWithSentinel(const DoublyCircularLinkedListWithSentinel &list) : size(list.size) {
        head->next = head->prev = head;
        for (Node *cur = list.head->next; cur != list.head; cur = cur->next) {
            head->prev = new Node(cur->value, head->prev, head);
            head->prev->prev->next = head->prev;
        }
    }

    ~DoublyCircularLinkedListWithSentinel() {
        Node *cur = head->next;
        while (cur != head) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
        delete head;
    }

    void addFirst(int value) {
        Node *first = head->next;
        first->prev = head->next = new Node(value, head, first);
        size++;
    }

    void addLast(int value) {
        Node *last = head->prev;
        last->next = head->prev = new Node(value, last, head);
        size++;
    }

    int removeFirst() {
        if (size == 0) {
            throw std::out_of_range("list is empty");
        }
        Node *cur = head->next;
        Node *next = cur->next;
        head->next = next;
        next->prev = head;
        int ret = cur->value;
        delete cur;
        size--;
        return ret;
    }

    int removeLast() {
        if (size == 0) {
            throw std::out_of_range("list is empty");
        }
        Node *cur = head->prev;
        Node *prev = cur->prev;
        head->prev = prev;
        prev->next = head;
        int ret = cur->value;
        delete cur;
        size--;
        return ret;
    }

    bool removeByValue(int value) {
        Node *cur = findByValue(value);
        if (cur == nullptr) {
            return false;
        }
        Node *prev = cur->prev;
        Node *next = cur->next;
        prev->next = next;
        next->prev = prev;
        delete cur;
        size--;
        return true;
    }

    class Iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = int;
        using pointer = Node *;
        using reference = int &;

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
        return Iterator(head->next);
    }

    Iterator end() const {
        return Iterator(head);
    }

private:
    Node *findByValue(int value) {
        for (Node *cur = head->next; cur != head; cur = cur->next) {
            if (cur->value == value) {
                return cur;
            }
        }
        return nullptr;
    }
};

using List = DoublyCircularLinkedListWithSentinel;

void test1() {
    List list;
    list.addFirst(1);
    list.addFirst(2);
    list.addFirst(3);
    list.addFirst(4);
    list.addFirst(5);
    list.addLast(6);
    list.addLast(7);
    list.addLast(8);
    list.addLast(9);
    list.addLast(10);

    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.removeFirst() << endl;
    cout << list.removeFirst() << endl;
    cout << list.removeFirst() << endl;
    cout << list.removeFirst() << endl;
    cout << list.removeFirst() << endl;
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.removeLast() << endl;
    cout << list.removeLast() << endl;
    cout << list.removeLast() << endl;
    cout << list.removeLast() << endl;
    cout << list.removeLast() << endl;
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;
    cout << list.removeLast() << endl;
}

void test2() {
    List list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);
    list.addLast(5);

    cout << list.removeByValue(3) << endl;
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;
}

int main() {
    // test1();
    test2();
    return 0;
}
