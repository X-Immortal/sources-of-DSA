//
// Created by xyx on 2025/12/14.
//
#include "Graph.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <queue>
#include <stack>

using std::cout;
using std::endl;

// 拓扑排序：每个顶点应出现在由它可达的任何一个顶点之前
// 前提条件：有向无环图

// Kahn算法
void topological_sort_Kahn(std::vector<Vertex *> &graph) {
    // 统计各个顶点的入度
    for (auto vertex : graph) {
        for (auto edge : vertex->edges) {
            edge.linked->in_degree++;
        }
    }

    // 将入度为0的顶点加入队列
    std::queue<Vertex *> queue;
    for (auto vertex : graph) {
        if (vertex->in_degree == 0) {
            queue.push(vertex);
        }
    }

    int size = graph.size();
    graph.clear();
    // 从队列中移除顶点，并遍历该顶点的邻接顶点，将入度减1，如果入度减为0，则加入队列
    while (!queue.empty()) {
        Vertex *vertex = queue.front();
        queue.pop();
        graph.push_back(vertex);
        for (auto &edge : vertex->edges) {
            if (--edge.linked->in_degree == 0) {
                queue.push(edge.linked);
            }
        }
    }

    if (size != graph.size()) {
        throw std::runtime_error("there is cycle in the graph");
    }
}

void dfs(Vertex *vertex, std::stack<Vertex *> &stack) {
    if (vertex->status == VISITED) {
        return;
    }
    if (vertex->status == VISITING) {
        throw std::runtime_error("there is cycle in the graph");
    }

    vertex->status = VISITING;
    for (auto &edge : vertex->edges) {
        dfs(edge.linked, stack);
    }

    vertex->status = VISITED;
    stack.push(vertex);
}

// 基于DFS的拓扑排序
void topological_sort_dfs(std::vector<Vertex *> &graph) {
    std::stack<Vertex *> stack;
    for (auto vertex : graph) {
        dfs(vertex, stack);
    }

    graph.clear();
    while (!stack.empty()) {
        graph.push_back(stack.top());
        stack.pop();
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Vertex v1("网页基础");
    Vertex v2("Java基础");
    Vertex v3("JavaWeb");
    Vertex v4("Spring框架");
    Vertex v5("微服务框架");
    Vertex v6("数据库");
    Vertex v7("实战项目");

    v1.edges.emplace_back(&v3);
    v2.edges.emplace_back(&v3);
    v3.edges.emplace_back(&v4);
    v6.edges.emplace_back(&v4);
    v4.edges.emplace_back(&v5);
    v5.edges.emplace_back(&v7);
    // v7.edges.emplace_back(&v5); // 构造环

    std::vector graph = {&v1, &v2, &v3, &v4, &v5, &v6, &v7};

    try {
        topological_sort_dfs(graph);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << endl;
        return -1;
    }

    for (auto vertex : graph) {
        cout << vertex->get_name() << " ";
    }

    return 0;
}