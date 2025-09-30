//
// Created by xyx on 25-7-24.
//
#include <iostream>
#include <functional>

using std::cout;
using std::endl;


// 带哨兵的单向链表
class SinglyLinkedListWithSentinel {
    // 链表与节点为组合关系，应将节点定义为内部类
    class Node {
    public:
        int value;
        Node *next;

        explicit Node(int value, Node *next = nullptr) : value(value), next(next) {}
    };

    // 哨兵节点，免去了对头指针的维护
    Node *head = new Node(114514);
    int size = 0;

public:
    SinglyLinkedListWithSentinel() = default;

    SinglyLinkedListWithSentinel(const SinglyLinkedListWithSentinel &list) : size(list.size) {
        for (Node *prev = head, *cur = list.head->next; cur != nullptr; cur = cur->next, prev = prev->next) {
            prev->next = new Node(cur->value);
        }
    }

    ~SinglyLinkedListWithSentinel() {
        for (Node *cur = head; cur != nullptr;) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
    }

    void addFirst(int value) {
        insert(0, value);
    }

    void forEach(const std::function<void(int)> &func) const {
        for (Node *cur = head->next; cur != nullptr; cur = cur->next) {
            func(cur->value);
        }
    }

    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = int;
        using pointer = Node *;
        using reference = int &;

        explicit Iterator(pointer node) : node(node) {
        }

        reference operator*() const {
            if (node == nullptr) {
                throw std::out_of_range("iterator out of range");
            }
            return node->value;
        }

        Iterator &operator++() {
            if (node == nullptr) {
                throw std::out_of_range("iterator out of range");
            }
            node = node->next;
            return *this;
        }

        Iterator operator++(int) {
            if (node == nullptr) {
                throw std::out_of_range("iterator out of range");
            }
            Iterator temp = *this;
            node = node->next;
            return temp;
        }

        bool operator==(const Iterator &it) const {
            return node == it.node;
        }

        bool operator!=(const Iterator &it) const {
            return !(*this == it);
        }

    private:
        pointer node;
    };

    Iterator begin() const {
        return Iterator(head->next);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }

    void addLast(int value) {
        insert(size, value);
    }

    int get(int index) const {
        return find(index)->value;
    }

    void insert(int index, int value) {
        Node *prev = find(index - 1);
        prev->next = new Node(value, prev->next);
        size++;
    }

    int removeFirst() {
        return remove(0);
    }

    int remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("index out of range");
        }
        Node *prev = find(index - 1), *cur = prev->next;
        prev->next = cur->next;
        int ret = cur->value;
        delete cur;
        size--;
        return ret;
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
        Node *cur = head;
        for (int i = -1; i < index; i++) {
            cur = cur->next;
        }
        return cur;
    }
};


void test1() {
    SinglyLinkedListWithSentinel list;
    list.addFirst(1);
    list.addFirst(2);
    list.addFirst(3);
    list.addFirst(4);
    list.forEach([](int value) {
        cout << value << " ";
    });
    cout << endl;

    SinglyLinkedListWithSentinel list2(list);
    list2.forEach([](int value) { cout << value << " "; });
    cout << endl;
    list.addFirst(5);
    list2.forEach([](int value) { cout << value << " "; });
    cout << endl;
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;
}

void test2() {
    SinglyLinkedListWithSentinel list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.get(2) << endl;

    list.insert(2, 5);
    list.insert(5, 6);
    list.insert(0, 7);
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;
    list.insert(100, 0);
}

void test3() {
    SinglyLinkedListWithSentinel list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);

    cout << list.removeFirst() << endl;
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.remove(1) << endl;
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    cout << list.remove(1) << endl;
    cout << list.remove(0) << endl;
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    list.remove(0);
}


int main() {
    // test1();
    // test2();
    test3();
    return 0;
}
