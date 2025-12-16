//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"
#include <iostream>

using std::cout;
using std::endl;

int main() {
    DirectedGraph graph;
    graph.add_vertex({
        "v1", "v2", "v3",
        "v4", "v5", "v6",
        "v7"
    });

    graph.bi_link("v1", "v2", 2);
    graph.bi_link("v1", "v3", 4);
    graph.bi_link("v1", "v4", 1);
    graph.bi_link("v2", "v4", 3);
    graph.bi_link("v2", "v5", 10);
    graph.bi_link("v3", "v4", 2);
    graph.bi_link("v3", "v6", 5);
    graph.bi_link("v4", "v5", 7);
    graph.bi_link("v4", "v6", 8);
    graph.bi_link("v4", "v7", 4);
    graph.bi_link("v5", "v7", 6);
    graph.bi_link("v6", "v7", 1);

    graph.Prim(0);

    for (auto &vertex : graph.vertices) {
        cout << vertex.name << ": " << vertex.min_weight <<
            "(" << (vertex.min_linked == -1 ? "null" : graph.v(vertex.min_linked)->name) << ")" << endl;
    }
    return 0;
}
