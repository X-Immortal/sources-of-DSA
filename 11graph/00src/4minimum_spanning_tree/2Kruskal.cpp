//
// Created by  Apple on 2025/12/15.
//
#include "Graph.h"
#include "DisjointSet.h"
#include <vector>
#include <queue>

// 最小生成树算法：Kruskal算法
// 以边为操作对象
std::vector<int> UndirectedGraph::Kruskal() {
    // 1. 将所有边加入优先级队列
    auto cmp = [this](int a, int b) {
        return this->e(a)->weight > this->e(b)->weight;
    };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> queue(cmp);
    for (int i = 0; i < edges.size(); i++) {
        queue.push(i);
    }

    // 2. 创建结果集合，用来存储最小生成树的边
    std::vector<int> result;
    result.reserve(vertices.size() - 1);

    // 3. 创建并查集，用来判断两个顶点是否连通
    DS::DisjointSet set(vertices.size());

    while (result.size() < vertices.size() - 1) {
        // 4. 获取最小边
        auto edge = queue.top();
        queue.pop();

        // 5. 若当前边连接的两个顶点不连通，则加入结果集合
        if (!set.united(v(e(edge)->v1)->id, v(e(edge)->v2)->id)) {
            result.push_back(edge);
            set.unite(v(e(edge)->v1)->id, v(e(edge)->v2)->id);
        }
    }

    return result;
}