//
// Created by xyx on 2025/12/14.
//
#include "Graph.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;


// 单源最短路径算法：Bellman-Ford算法
// 用于处理带负权边的图
// 负环：环上所有边的权重之和为负
// 当图中存在负环时，最短路无解（解为负无穷）
void DirectedGraph::Bellman_Ford(int start) {
    // 1. 为每个顶点设置一个初始距离，起点为0，其余为无穷大
    reset();
    auto &dist = v(start)->distance;
    auto &prev = v(start)->prev;

    // 3. 迭代次数为(顶点数-1)次
    for (int i = 0; i < vertices.size(); ++i) {
        // 2. 遍历所有边，更新它们指向的顶点的距离
        for (auto &vertex : vertices) {
            for (auto &edge : vertex.adjacency_list) {
                int distance = dist[vertex.id] + edge.weight;
                if (dist[vertex.id] != Vertex::POS_INF &&
                        distance < dist[edge.linked]) {
                    // 额外增加一次迭代，若还能找到更小的距离，说明存在负环
                    if (i == vertices.size() - 1) {
                        throw std::runtime_error("there is minus cycle");
                    }
                    dist[edge.linked] = distance;
                    prev[edge.linked] = vertex.id;
                }
            }
        }
    }
}