//
// Created by xyx on 2025/12/14.
//
#include "Graph.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;

namespace std {
    template<>
    struct hash<Vertex *> {
        size_t operator()(const Vertex *v) const noexcept {
            return hash<string>()(v->name);
        }
    };

    template<>
    struct equal_to<Vertex *> {
        bool operator()(const Vertex *v1, const Vertex *v2) const noexcept {
            return v1->name == v2->name;
        }
    };
}

using dist_arr_t = std::vector<std::vector<int>>;
using prev_arr_t = std::vector<std::vector<Vertex *>>;

std::pair<dist_arr_t&, prev_arr_t&> get_distances(std::vector<Vertex *> &graph) {
    dist_arr_t distance(graph.size());
    prev_arr_t prev(graph.size());
    for (int begin = 0; begin < graph.size(); begin++) {
        distance[begin].resize(graph.size());
        prev[begin].resize(graph.size());

        // 将邻接表(链表)转为哈希表，提升查询效率
        std::unordered_map<decltype(Edge::linked), decltype(Edge::weight)> neighbors;
        for (Edge &edge : graph[begin]->edges) {
            neighbors.emplace(edge.linked, edge.weight);
        }

        for (int end = 0; end < graph.size(); end++) {
            if (begin == end) {
                // 自己到自己的距离为0
                distance[begin][end] = 0;
                prev[begin][end] = nullptr;
            } else {
                if (neighbors.contains(graph[end])) {
                    distance[begin][end] = neighbors[graph[end]];
                    prev[begin][end] = graph[begin];
                } else {
                    // 两顶点不连通，初始距离为正无穷
                    distance[begin][end] = Vertex::POS_INF;
                    prev[begin][end] = nullptr;
                }
            }
        }
    }

    return {distance, prev};
}

// 多源最短路径算法：Floyd-Warshall算法
void Floyd_Warshall(std::vector<Vertex *> &graph) {
    // 1. 初始化距离矩阵
    auto &&arrs = get_distances(graph);
    auto &distances = arrs.first;
    auto &prev = arrs.second;

    // 2. 每次取一个顶点，更新所有经过该顶点的路径的距离
    for (int mid = 0; mid < graph.size(); ++mid) {
        // 3. 遍历所有起点
        for (int begin = 0; begin < graph.size(); ++begin) {
            // 起点到中间节点的路不通，直接跳过这个起点
            if (distances[begin][mid] == Vertex::POS_INF) {
                continue;
            }

            // 4. 遍历所有终点
            for (int end = 0; end < graph.size(); ++end) {
                // 中间节点到终点的路不通，直接跳过这个终点
                if (distances[begin][mid] == Vertex::POS_INF) {
                    continue;
                }

                // 更新距离
                int new_dist = distances[begin][mid] + distances[mid][end];
                distances[begin][end] = std::min(distances[begin][end], new_dist);
            }
        }
    }

}

/*   ----(4)---> v1 ---(-2)---
 *   |                       |
 *   |                       v
 *  v2 ---------(3)--------> v3
 *   ^                       |
 *   |                       |
 *   ---(-1)--- v4 <---(2)----
 */
int main() {
    Vertex v1("v1");
    Vertex v2("v2");
    Vertex v3("v3");
    Vertex v4("v4");

    v1.edges.assign({
        Edge(&v3, -2)
    });
    v2.edges.assign({
        Edge(&v1, 4),
        Edge(&v3, 3)
    });
    v3.edges.assign({
        Edge(&v4, 2)
    });
    v4.edges.assign({
        Edge(&v2, -1)
    });

    std::vector graph = {&v1, &v2, &v3, &v4};

    Floyd_Warshall(graph);

    return 0;
}