//
// Created by xyx on 2025/10/1.
//

#ifndef ONLINE_CLASS_MAXHEAP_H
#define ONLINE_CLASS_MAXHEAP_H

class MaxHeap {
    int *array;
    int capacity;
    int size;

public:
    MaxHeap(int capacity);

    MaxHeap(int *array, int size);

    ~MaxHeap();

    void heapify();

    int poll();

    int poll(int index);

    int peek() const;

    bool offer(int value);

    void replace(int value);

    bool isEmpty() const;

    bool isFull() const;

private:
    void up(int index);

    void down(int index);

    void swap(int i, int j);

    int parent(int index) const;

    int left(int index) const;

    int right(int index) const;
};

#endif //ONLINE_CLASS_MAXHEAP_H