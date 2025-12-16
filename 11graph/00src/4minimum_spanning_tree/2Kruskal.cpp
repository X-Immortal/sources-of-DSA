//
// Created by  Apple on 2025/12/15.
//
#include "Graph.h"
#include "DisjointSet.h"
#include <vector>
#include <queue>

// 最小生成树算法：Kruskal算法
// 以边为操作对象
// std::vector<Edge *> Kruskal(const std::vector<Edge *> &graph, int size) {
//     // 1. 将所有边加入优先级队列
//     std::priority_queue queue([] (Edge *a, Edge *b) {
//         return a->weight >= b->weight;
//     }, graph);
//
//     // 2. 创建结果集合，用来存储最小生成树的边
//     std::vector<Edge *> result;
//     result.reserve(size - 1);
//
//     // 3. 创建并查集，用来判断两个顶点是否连通
//     DS::DisjointSet set(size);
//
//     while (result.size() < size - 1) {
//         // 4. 获取最小边
//         auto edge = queue.top();
//         queue.pop();
//
//         // 5. 若当前边连接的两个顶点不连通，则加入结果集合
//         if (!set.united(edge->v1->id, edge->v2->id)) {
//             result.push_back(edge);
//             set.unite(edge->v1->id, edge->v2->id);
//         }
//     }
//
//     return result;
// }