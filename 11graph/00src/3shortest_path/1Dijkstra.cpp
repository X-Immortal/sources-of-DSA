//
// Created by xyx on 2025/12/14.
//
#include "Graph.h"
#include <vector>
#include <queue>
#include <unordered_set>

// 单源最短路径算法：Dijkstra算法
// 前提：所有边的权重都是正数
void DirectedGraph::Dijkstra(int start) {
    // 1. 为每个顶点设置一个初始距离，起点为0，其余为无穷大
    reset();
    auto &dist = vertices[start].distance;
    dist[start] = 0;
    auto &prev = vertices[start].prev;

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
        // 排除已访问节点和不连通节点
        if (cur->visited() || dist[cur->id] == Vertex::POS_INF) {
            continue;
        }

        // 4. 将当前顶点从未访问集合中移除
        cur->end_visit();
        // 5. 更新当前顶点所有未访问邻居的距离
        for (auto &edge : cur->adjacency_list) {
            if (!v(edge.linked)->visited()) {
                int distance = dist[cur->id] + edge.weight;
                if (distance < dist[v(edge.linked)->id]) {
                    dist[v(edge.linked)->id] = distance;
                    prev[v(edge.linked)->id] = cur->id;
                    // 将更新后的顶点入队（可能带来重复）
                    queue.emplace(edge.linked, distance);
                }
            }
        }
    }
}
