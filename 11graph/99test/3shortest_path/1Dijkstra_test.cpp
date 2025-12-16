//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

int main() {
    DirectedGraph graph;
    graph.add_vertex({
        "v1", "v2", "v3",
        "v4", "v5", "v6"
    });

    graph.link("v1", "v3", 9);
    graph.link("v1", "v2", 7);
    graph.link("v1", "v6", 14);
    graph.link("v2", "v4", 15);
    graph.link("v3", "v4", 11);
    graph.link("v3", "v6", 2);
    graph.link("v4", "v5", 6);
    graph.link("v6", "v5", 9);

    auto &&result = graph.Dijkstra("v1");

    for (auto &p : result) {
        cout << p.first << ": " << p.second.first <<
            "(" << p.second.second << ")" << endl;
    }

    return 0;
}
