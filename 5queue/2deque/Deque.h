//
// Created by xyx on 2025/9/26.
//

#ifndef ONLINE_CLASS_DEQUE_H
#define ONLINE_CLASS_DEQUE_H

template<class T>
class Deque {
public:
    virtual ~Deque() = default;

    virtual bool offerFirst(T e) = 0;

    virtual bool offerLast(T e) = 0;

    virtual T pollFirst() = 0;

    virtual T pollLast() = 0;

    virtual T peekFirst() = 0;

    virtual T peekLast() = 0;

    virtual bool isEmpty() = 0;

    virtual bool isFull() = 0;
};

#endif //ONLINE_CLASS_DEQUE_H