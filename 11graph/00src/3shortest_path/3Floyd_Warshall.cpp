//
// Created by xyx on 2025/12/14.
//
#include "Graph.h"
#include <vector>
#include <stdexcept>

void DirectedGraph::init_dist() {
    reset();
    for (int begin = 0; begin < vertices.size(); begin++) {
        auto &dist = v(begin)->distance;
        auto &prev = v(begin)->prev;

        for (auto &edge : v(begin)->adjacency_list) {
            dist[v(edge.linked)->id] = edge.weight;
            prev[v(edge.linked)->id] = begin;
        }
    }
}

// 多源最短路径算法：Floyd-Warshall算法
void DirectedGraph::Floyd_Warshall_do() {
    // 1. 初始化距离矩阵
    init_dist();

    // 2. 每次取一个顶点，更新所有经过该顶点的路径的距离
    for (int mid = 0; mid < vertices.size(); ++mid) {
        // 3. 遍历所有起点
        for (int begin = 0; begin < vertices.size(); ++begin) {
            // 起点到中间节点的路不通，直接跳过这个起点
            if (vertices[begin].distance[mid] == Vertex::POS_INF) {
                continue;
            }

            // 4. 遍历所有终点
            for (int end = 0; end < vertices.size(); ++end) {
                // 中间节点到终点的路不通，直接跳过这个终点
                if (vertices[mid].distance[end] == Vertex::POS_INF) {
                    continue;
                }

                // 更新距离
                int new_dist = vertices[begin].distance[mid] + vertices[mid].distance[end];
                if (new_dist < vertices[begin].distance[end]) {
                    if (begin == end) {
                        throw std::runtime_error("there is minus cycle in the graph");
                    }
                    vertices[begin].distance[end] = new_dist;
                    vertices[begin].prev[end] = vertices[mid].prev[end];
                }
            }
        }
    }
}
