//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

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
        cout << vertex->name << ": " << vertex->distance <<
            "(" << (vertex->prev == nullptr ? "null" : vertex->prev->name) << ")" << endl;
    }

    return 0;
}
