//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

void print_path(DirectedGraph::full_path_t &arr) {
    for (int i = 0; i < arr.size(); i++) {
        for (int j = 0; j < arr.size(); j++) {
            auto &[begin, path] = arr[i];
            auto &[end, info] = path[j];
            auto &[dist, prev] = info;

            cout << begin << "->" << end << ": ";
            if (dist == DirectedGraph::POS_INF) {
                cout << "∞";
            } else {
                cout << dist;
            }
            cout << '\t';
            cout << prev << endl;
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
    DirectedGraph graph;

    graph.add_vertex({"v1", "v2", "v3", "v4"});

    graph.link("v1", "v3", -2);
    graph.link("v2", "v1", 4);
    graph.link("v2", "v3", 3);
    graph.link("v3", "v4", 2);
    // graph.link("v4", "v2", -1); // 正常情况
    graph.link("v4", "v2", -6); // 负环

    try {
        auto &&result = graph.Floyd_Warshall();
        print_path(result);
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
