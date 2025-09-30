//
// Created by xyx on 25-7-21.
//
#include <iostream>
#include <cstring>
#include <functional>

using std::cout;
using std::endl;

// 数组：由一组元素(值或变量)组成的数据结构，每个元素至少有一个索引或键来标识
// 数组内的元素是连续存储的，数组中元素的地址可以通过其索引计算出来
// address = base + index * size
// 随机访问时间复杂度：O(1)

class DynamicArray {
    int size = 0;
    int capacity = 8;
    // 懒惰初始化，等到真正添加元素时才分配内存
    int *arr = nullptr;

public:
    ~DynamicArray() {
        delete[] arr;
    }

    DynamicArray() = default;

    // explicit: 禁用隐式转换法初始化
    explicit DynamicArray(int capacity) {
        this->capacity = capacity;
    }

    DynamicArray(const DynamicArray &array) : size(array.size), capacity(array.capacity) {
        if (array.arr == nullptr) {
            return;
        }
        arr = new int[capacity];
        memcpy(arr, array.arr, size * sizeof(int));
    }

    // 尾部插入
    // 平均时间复杂度：O(1)
    // 最坏时间复杂度：O(n)
    void addLast(int element) {
        add(size, element);
    }

    // 头部和中间插入时间复杂度：O(n)
    void add(int index, int element) {
        if (index < 0 || index > size) {
            throw std::runtime_error("index out of range");
        }
        if (arr == nullptr) {
            arr = new int[capacity];
        }
        if (size == capacity) {
            expand();
        }
        memmove(arr + index + 1, arr + index, (size - index) * sizeof(int));
        arr[index] = element;
        size++;
    }

    // 查询时间复杂度：O(1)
    int get(int index) const {
        if (index < 0 || index >= size) {
            throw std::runtime_error("index out of range");
        }
        return arr[index];
    }

    int getSize() const {
        return size;
    }

    void forEach(const std::function<void(int)> &func) const {
        for (int i = 0; i < size; i++) {
            func(arr[i]);
        }
    }

    class Iterator {
    public:
        // using: typedef的现代化方案
        using iterator_category = std::forward_iterator_tag;
        using value_type = int;
        using difference_type = int;
        using pointer = int *;
        using reference = int &;

        explicit Iterator(pointer ptr) : current(ptr) {}

        reference operator*() const {
            return *current;
        }

        Iterator &operator++() {
            ++current;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++current;
            return temp;
        }

        bool operator==(const Iterator &it) const {
            return current == it.current;
        }

        bool operator!=(const Iterator &it) const {
            return !(*this == it);
        }

        difference_type operator-(const Iterator &it) const {
            return current - it.current;
        }

    private:
        pointer current;
    };

    Iterator begin() const {
        return Iterator(arr);
    }

    Iterator end() const {
        return Iterator(arr + size);
    }

    int remove(int index) {
        if (index < 0 || index >= size) {
            throw std::runtime_error("index out of range");
        }
        int ret = arr[index];
        if (index < size - 1) {
            memmove(arr + index, arr + index + 1, (size - index - 1) * sizeof(int));
        }
        size--;
        return ret;
    }

    bool isEmpty() const {
        return size == 0;
    }

private:
    void expand() {
        capacity += capacity / 2;
        int *newArr = new int[capacity];
        memcpy(newArr, arr, size * sizeof(int));
        delete[] arr;
        arr = newArr;
    }
};

void test1() {
    DynamicArray array(2);
    array.addLast(1);
    array.addLast(2);
    array.addLast(3);
    array.addLast(4);
    array.add(2, 5);
    array.addLast(6);
    array.addLast(7);
    array.addLast(8);
    array.addLast(9);

    for (int i = 0; i < array.getSize(); i++) {
        cout << array.get(i) << " ";
    }
    cout << endl;

    for (int elem : array) {
        cout << elem << " ";
    }
    cout << endl;

    array.forEach([](int elem) {cout << elem << " " ;});
    cout << endl;
}

void test2() {
    DynamicArray array;
    array.addLast(1);
    array.addLast(2);
    array.addLast(3);
    array.addLast(4);
    array.addLast(5);

    array.forEach([](int val){cout << val << " " ;});
    cout << endl;

    cout << array.remove(2) << endl;
    array.forEach([](int val){cout << val << " " ;});
    cout << endl;
}

int main() {
    // test1();
    test2();
    return 0;
}