//
// Created by xyx on 2025/12/13.
//
#include "DisjointSet.h"
#include <algorithm>

namespace DS {
    DisjointSet::DisjointSet(int size) : parent_size(size), size_size(size) {
        parent = new int[size];
        this->size = new int[size](1);
        // 初始时，所有元素都单独成集合
        for (int i = 0; i < size; i++) {
            parent[i] = i;
        }
    }

    DisjointSet::~DisjointSet() {
        delete[] parent;
    }

    // 查找元素x的集合编号
    int DisjointSet::find(int x) {
        // 指向自身的元素为集合的“老大”，其下标为集合的编号
        if (x == parent[x]) {
            return x;
        }
        // 路径压缩优化
        // 每次查找完一个元素，就让该元素直接指向集合编号，从而压缩了查找路径
        // 此处还一次性把路径上的所有元素都压缩了
        return parent[x] = find(parent[x]);
    }

    // 让两个集合“相交”
    void DisjointSet::unite(int x, int y) {
        // 合并时只需让一个“老大”指向另一个“老大”
        x = find(x);
        y = find(y);

        // unite by size优化：让小集合的“老大”指向大集合的“老大”
        // 这样做可以缩短整体的查找路径
        if (size[x] < size[y]) {
            std::swap(x, y);
        }
        parent[y] = x;
        size[x] += size[y];
    }

    // 判断两个元素是否属于同一个集合
    bool DisjointSet::united(int x, int y) {
        return find(x) == find(y);
    }
}
