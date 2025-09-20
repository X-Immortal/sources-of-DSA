//
// Created by xyx on 25-7-22.
//
#include <iostream>
#include <functional>

using std::cout;
using std::endl;

// 链表：数据元素的线性集合，每个元素指向下一个元素，元素存储上并不连续
// 随机访问时间复杂度：O(n)
// 头部插入删除访问时间复杂度：O(1)
// 尾部插入删除访问时间复杂度：维护tail为O(1)，不维护为O(n)

// 普通单向链表
class SinglyLinkedList {
    // 链表与节点为组合关系，应将节点定义为内部类
    class Node {
        friend class SinglyLinkedList;

        int value;
        Node *next;

        explicit Node(int value, Node *next = nullptr) : value(value), next(next) {}
    };

    Node *head = nullptr;
    int size = 0;

public:
    SinglyLinkedList() = default;

    SinglyLinkedList(const SinglyLinkedList &list) : size(list.size) {
        if (list.head == nullptr) {
            return;
        }
        head = new Node(list.head->value);
        for (Node *prev = head, *cur = list.head->next; cur != nullptr; cur = cur->next, prev = prev->next) {
            prev->next = new Node(cur->value);
        }
    }

    ~SinglyLinkedList() {
        for (Node *cur = head; cur != nullptr;) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
    }

    void addFirst(int value) {
        head = new Node(value, head);
        size++;
    }

    void forEach(const std::function<void(int)> &func) const {
        for (Node *cur = head; cur != nullptr; cur = cur->next) {
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
        return Iterator(head);
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
        if (index < 0 || index > size) {
            throw std::out_of_range("index out of range");
        }
        if (index == 0) {
            addFirst(value);
            return;
        }
        Node *prev = find(index - 1);
        prev->next = new Node(value, prev->next);
        size++;
    }

    void removeFirst() {
        if (head == nullptr) {
            throw std::out_of_range("list is empty");
        }
        Node *cur = head;
        head = head->next;
        delete cur;
        size--;
    }

    void remove(int index) {
        if (index == 0) {
            removeFirst();
            return;
        }
        if (index < 0 || index >= size) {
            throw std::out_of_range("index out of range");
        }
        Node *prev = find(index - 1), *cur = prev->next;
        prev->next = cur->next;
        delete cur;
        size--;
    }

    bool isEmpty() const {
        return size == 0;
    }

    int getSize() const {
        return size;
    }

    void print() {
        recursion(head);
    }

private:
    Node *find(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("index out of range");
        }
        Node *cur = head;
        for (int i = 0; i < index; i++) {
            cur = cur->next;
        }
        return cur;
    }

    void recursion(Node *cur) {
        if (cur == nullptr) {
            cout << endl;
            return;
        }
        cout << cur->value << " ";
        recursion(cur->next);
    }
};

void test1() {
    SinglyLinkedList list;
    list.addFirst(1);
    list.addFirst(2);
    list.addFirst(3);
    list.addFirst(4);
    list.forEach([](int value) {
        cout << value << " ";
    });
    cout << endl;

    SinglyLinkedList list2(list);
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
    SinglyLinkedList list;
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
    SinglyLinkedList list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);

    list.removeFirst();
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    list.remove(1);
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    list.remove(1);
    list.remove(0);
    for (auto elem: list) {
        cout << elem << " ";
    }
    cout << endl;

    list.remove(0);
}

void test4() {
    SinglyLinkedList list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addLast(4);
    list.print();
}


int main() {
    // test1();
    // test2();
    // test3();
    test4();
    return 0;
}
