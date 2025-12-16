//
// Created by xyx on 2025/9/13.
//
#pragma once

template<class T>
class Queue {
public:
    virtual ~Queue() = default;

    virtual bool offer(const T &value) = 0;

    virtual T poll() = 0;

    virtual T peek() const = 0;

    virtual bool isEmpty() const = 0;

    virtual bool isFull() const = 0;
};