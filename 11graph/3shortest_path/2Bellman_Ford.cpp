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
void Bellman_Ford(std::vector<Vertex *> &graph, Vertex *start) {
    // 1. 为每个顶点设置一个初始距离，起点为0，其余为无穷大
    start->distance = 0;

    // 3. 迭代次数为(顶点数-1)次
    for (int i = 0; i < graph.size(); ++i) {
        // 2. 遍历所有边，更新它们指向的顶点的距离
        for (auto vertex : graph) {
            for (auto &edge : vertex->edges) {
                if (vertex->distance != Vertex::POS_INF &&
                        vertex->distance + edge.weight < edge.linked->distance) {
                    // 额外增加一次迭代，若还能找到更小的距离，说明存在负环
                    if (i == graph.size() - 1) {
                        throw std::runtime_error("there is minus cycle");
                    }
                    edge.linked->distance = vertex->distance + edge.weight;
                    edge.linked->prev = vertex;
                }
            }
        }
    }
}

/*
 *  正常情况：
 *      (2)     (-2)      (1)
 *  v1 ----> v2 ----> v3 ----> v4
 *  |                 ^
 *  |_____(1)_________|
 *
 *  负环：
 *      (2)     (-4)      (1)
 *  v1 ----> v2 ----> v3 ----> v4
 *  ^                 |
 *  |_____(1)_________|
 */
int main() {
    Vertex v1("v1");
    Vertex v2("v2");
    Vertex v3("v3");
    Vertex v4("v4");

    v1.edges.assign({
        Edge(&v2, 2),
        // Edge(&v3, 1) // 正常情况
    });
    v2.edges.assign({
        // Edge(&v3, -2), // 正常情况
        Edge(&v3, -4), // 负环
    });
    v3.edges.assign({
        Edge(&v4, 1),
        Edge(&v1, 1), // 负环
    });

    std::vector graph = {&v1, &v2, &v3, &v4};

    try {
        Bellman_Ford(graph, &v1);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << endl;
        return -1;
    }

    for (auto vertex : graph) {
        cout << vertex->get_name() << ": " << vertex->distance <<
            "(" << (vertex->prev == nullptr ? "null" : vertex->prev->get_name()) << ")" << endl;
    }

    return 0;
}