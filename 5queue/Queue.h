//
// Created by xyx on 2025/9/13.
//
#pragma once

template<class T>
class Queue {
public:
    virtual ~Queue() = default;

    virtual bool offer(T value) = 0;

    virtual T poll() = 0;

    virtual T peek() = 0;

    virtual bool isEmpty() = 0;

    virtual bool isFull() = 0;
};