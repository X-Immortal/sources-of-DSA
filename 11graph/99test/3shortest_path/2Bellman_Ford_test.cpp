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
    DirectedGraph graph;

    graph.add_vertex({"v1", "v2", "v3", "v4"});

    graph.link("v1", "v2", 2);
    // graph.link("v2", "v3", 1); // 正常情况
    // graph.link("v2", "v3", -2); // 正常情况
    graph.link("v2", "v3", -4); // 负环
    graph.link("v3", "v4", 1);
    graph.link("v3", "v1", 1); // 负环

    try {
        auto &&result = graph.Bellman_Ford("v1");
        for (auto &p : result) {
            cout << p.first << ": " << p.second.first <<
                "(" << p.second.second << ")" << endl;
        }
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << endl;
        return -1;
    }

    return 0;
}
