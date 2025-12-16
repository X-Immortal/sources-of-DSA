//
// Created by xyx on 2025/12/15.
//

#ifndef ONLINE_CLASS_DISJOINTSET_H
#define ONLINE_CLASS_DISJOINTSET_H

namespace DS {
    class DisjointSet {
        int *parent; // 数组中的值为父节点编号
        int parent_size;
        int *size; // 存储每个子集的大小
        int size_size;

    public:
        DisjointSet(int size);

        ~DisjointSet();

        int find(int x);

        void unite(int x, int y);

        bool united(int x, int y);
    };
}

#endif //ONLINE_CLASS_DISJOINTSET_H