//
// Created by  Apple on 2025/12/15.
//
#include "Graph.h"
#include <queue>

// 最小生成树：找到最少数量(顶点数量-1)的边，使所有顶点都连通，且权值总和最小
// 最小生成树算法：Prim算法
// 以顶点为操作对象，过程与Dijkstra算法类似
void DirectedGraph::Prim(int start) {
    // 1. 为每个顶点设置一个初始距离，起点为0，其余为无穷大
    reset();
    v(start)->min_weight = 0;

    // 优先级队列中存储<id, dist>
    // 此处应该存储距离的副本，而不应该让同一节点共享同一份距离，否则会破坏堆的特性
    auto cmp = [](const std::pair<int, int> &a, const std::pair<int, int> &b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(cmp)> queue(cmp);
    // 2. 将起点加入优先级队列
    queue.emplace(start, 0);

    while (!queue.empty()) {
        // 3. 从队列中获取距离最小的顶点
        Vertex *cur = v(queue.top().first);
        queue.pop();
        // 排除已访问节点
        if (cur->visited()) {
            continue;
        }

        // 4. 将当前顶点从未访问集合中移除
        cur->end_visit();
        // 5. 更新当前顶点所有未访问邻居的距离
        for (auto &edge : cur->adjacency_list) {
            auto linked = v(edge.linked);
            if (!linked->visited()) {
                int weight = edge.weight;
                if (weight < linked->min_weight) {
                    linked->min_weight = weight;
                    linked->min_linked = cur->id;
                    // 将更新后的顶点入队（可能带来重复）
                    queue.emplace(edge.linked, weight);
                }
            }
        }
    }
}
