//
// Created by xyx on 2025/9/14.
//

#ifndef ONLINE_CLASS_STACK_H
#define ONLINE_CLASS_STACK_H

template<class T>
class Stack {
public:
    virtual ~Stack() = default;

    virtual bool push(const T &value) = 0;

    virtual T pop() = 0;

    virtual T peek() const = 0;

    virtual bool isEmpty() const = 0;

    virtual bool isFull() const = 0;
};

#endif //ONLINE_CLASS_STACK_H