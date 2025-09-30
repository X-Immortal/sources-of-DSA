//
// Created by xyx on 2025/9/27.
//

#ifndef ONLINE_CLASS_PRIORITY_H
#define ONLINE_CLASS_PRIORITY_H

class Priority {
public:
    virtual ~Priority() = default;

    virtual int priority() const = 0;
};

#endif //ONLINE_CLASS_PRIORITY_H