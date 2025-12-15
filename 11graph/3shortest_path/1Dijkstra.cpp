//
// Created by xyx on 2025/12/14.
//
#include "Graph.h"
#include <iostream>
#include <vector>
#include <queue>

using std::cout;
using std::endl;

// 单源最短路径算法：Dijkstra算法
// 前提：所有边的权重都是正数
void Dijkstra(std::vector<Vertex *> &graph, Vertex *start) {
    // 1. 为每个顶点设置一个初始距离，起点为0，其余为无穷大
    start->distance = 0;

    // 2. 将所有顶点加入优先级队列
    std::priority_queue queue([](Vertex *a, Vertex *b) {
        return a->distance >= b->distance;
    }, graph);


    while (!queue.empty()) {
        // 3. 从队列中获取距离最小的顶点
        Vertex *cur = queue.top();
        // 避免重复操作
        if (cur->visited) {
            queue.pop();
            continue;
        }

        // 4. 更新当前顶点所有未访问邻居的距离
        for (auto &edge : cur->edges) {
            if (!edge.linked->visited) {
                int distance = cur->distance + edge.weight;
                if (distance < edge.linked->distance) {
                    edge.linked->distance = distance;
                    edge.linked->prev = cur;
                    // 此处要将更新后的顶点重新入队，保证它处在正确的位置（但会带来重复）
                    queue.push(edge.linked);
                }
            }
        }

        // 5. 将当前顶点从未访问集合中移除
        queue.pop();
        cur->visited = true;
    }
}

int main() {
    Vertex v1("v1");
    Vertex v2("v2");
    Vertex v3("v3");
    Vertex v4("v4");
    Vertex v5("v5");
    Vertex v6("v6");

    v1.edges.assign({
        Edge(&v3, 9),
        Edge(&v2, 7),
        Edge(&v1, 14)
    });
    v2.edges.assign({
        Edge(&v4, 15)
    });
    v3.edges.assign({
        Edge(&v4, 11),
        Edge(&v6, 2)
    });
    v4.edges.assign({
        Edge(&v5, 6)
    });
    v6.edges.assign({
        Edge(&v5, 9)
    });

    std::vector graph = {&v1, &v2, &v3, &v4, &v5, &v6};

    Dijkstra(graph, &v1);

    for (auto vertex : graph) {
        cout << vertex->get_name() << ": " << vertex->distance <<
            "(" << (vertex->prev == nullptr ? "null" : vertex->prev->get_name()) << ")" << endl;
    }

    return 0;
}