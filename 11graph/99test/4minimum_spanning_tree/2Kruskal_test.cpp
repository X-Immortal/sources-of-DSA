//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"
#include <iostream>
#include <vector>

using std::cout;
using std::endl;

int main() {
    UndirectedGraph graph;

    graph.add_vertex({
        "v1", "v2", "v3",
        "v4", "v5", "v6",
        "v7"
    });

    graph.link("v1", "v2", 2);
    graph.link("v1", "v3", 4);
    graph.link("v1", "v4", 1);
    graph.link("v2", "v4", 3);
    graph.link("v2", "v5", 10);
    graph.link("v3", "v4", 2);
    graph.link("v3", "v6", 5);
    graph.link("v4", "v5", 7);
    graph.link("v4", "v6", 8);
    graph.link("v4", "v7", 4);
    graph.link("v5", "v7", 6);
    graph.link("v6", "v7", 1);

    auto &&mst = graph.Kruskal();

    for (auto edge : mst) {
        cout << graph.v(graph.e(edge)->v1)->name << "<->" <<
            graph.v(graph.e(edge)->v2)->name << ": " <<
                graph.e(edge)->weight << endl;
    }

    return 0;
}
