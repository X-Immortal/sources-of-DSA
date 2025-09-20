//
// Created by xyx on 2025/9/14.
//

#ifndef ONLINE_CLASS_STACK_H
#define ONLINE_CLASS_STACK_H

template<class T>
class Stack {
public:
    virtual ~Stack() = default;

    virtual bool push(T value) = 0;

    virtual T pop() = 0;

    virtual T peek() = 0;

    virtual bool isEmpty() = 0;

    virtual bool isFull() = 0;
};

#endif //ONLINE_CLASS_STACK_H