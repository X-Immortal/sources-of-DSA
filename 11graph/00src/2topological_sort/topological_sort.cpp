//
// Created by xyx on 2025/12/14.
//
#include "Graph.h"

#include <vector>
#include <queue>
#include <stack>
#include <stdexcept>


// 拓扑排序：每个顶点应出现在由它可达的任何一个顶点之前
// 前提条件：有向无环图

// Kahn算法
std::vector<std::string> DirectedGraph::topological_sort_Kahn() {
    // 统计各个顶点的入度
    for (auto &vertex : vertices) {
        for (auto &edge : vertex.adjacency_list) {
            v(edge.linked)->in_degree++;
        }
    }

    // 将入度为0的顶点加入队列
    std::queue<Vertex *> queue;
    for (auto &vertex : vertices) {
        if (vertex.in_degree == 0) {
            queue.push(&vertex);
        }
    }

    std::vector<std::string> result;
    result.reserve(vertices.size());
    // 从队列中移除顶点，并遍历该顶点的邻接顶点，将入度减1，如果入度减为0，则加入队列
    while (!queue.empty()) {
        Vertex *vertex = queue.front();
        queue.pop();
        result.push_back(vertex->name);
        for (auto &edge : vertex->adjacency_list) {
            if (--v(edge.linked)->in_degree == 0) {
                queue.push(v(edge.linked));
            }
        }
    }

    if (result.size() != vertices.size()) {
        throw std::runtime_error("there is cycle in the graph");
    }

    return result;
}

// 基于DFS的拓扑排序
std::vector<std::string> DirectedGraph::topological_sort_dfs() {
    reset();
    std::stack<Vertex *> stack;
    for (auto &vertex : vertices) {
        dfs_rec_do(vertex.id, [&stack] (Vertex *vertex) {
            stack.push(vertex);
        }, false);
    }

    std::vector<std::string> result;
    while (!stack.empty()) {
        result.push_back(stack.top()->name);
        stack.pop();
    }
    return result;
}