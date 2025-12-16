//
// Created by  Apple on 2025/12/15.
//
#include "Graph.h"
#include <queue>

// 最小生成树：找到最少数量(顶点数量-1)的边，使所有顶点都连通，且权值总和最小
// 最小生成树算法：Prim算法
// 以顶点为操作对象，过程与Dijkstra算法类似
// void Prim(std::vector<Vertex *> &graph, Vertex *start) {
//     // 1. 为每个顶点设置一个初始距离，起点为0，其余为无穷大
//     start->distance = 0;
//
//     // 2. 将所有顶点加入优先级队列
//     std::priority_queue queue([](Vertex *a, Vertex *b) {
//         return a->distance >= b->distance;
//     }, graph);
//
//     while (!queue.empty()) {
//         // 3. 从队列中获取距离最小的顶点
//         Vertex *cur = queue.top();
//         // 避免重复操作
//         if (cur->visited) {
//             queue.pop();
//             continue;
//         }
//
//         // 4. 更新当前顶点所有未访问邻居的距离
//         for (auto &edge : cur->edges) {
//             Vertex *linked;
//             if (edge->v1 == cur) {
//                 linked = edge->v2;
//             } else {
//                 linked = edge->v1;
//             }
//
//             if (!linked->visited) {
//                 int distance = edge->weight;
//                 if (distance < linked->distance) {
//                     linked->distance = distance;
//                     linked->prev = cur;
//                     // 此处要将更新后的顶点重新入队，保证它处在正确的位置（但会带来重复）
//                     queue.push(linked);
//                 }
//             }
//         }
//
//         // 5. 将当前顶点从未访问集合中移除
//         queue.pop();
//         cur->visited = true;
//     }
// }
