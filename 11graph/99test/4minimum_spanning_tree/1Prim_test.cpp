//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"
#include <iostream>
#include <queue>

using std::cout;
using std::endl;

int main() {
    Vertex v1("v1");
    Vertex v2("v2");
    Vertex v3("v3");
    Vertex v4("v4");
    Vertex v5("v5");
    Vertex v6("v6");
    Vertex v7("v7");

    Edge e12(&v1, &v2, 2);
    Edge e13(&v1, &v3, 4);
    Edge e14(&v1, &v4, 1);
    Edge e24(&v2, &v4, 3);
    Edge e25(&v2, &v5, 10);
    Edge e34(&v3, &v4, 2);
    Edge e36(&v3, &v6, 5);
    Edge e45(&v4, &v5, 7);
    Edge e46(&v4, &v6, 8);
    Edge e47(&v4, &v7, 4);
    Edge e57(&v5, &v7, 6);
    Edge e67(&v6, &v7, 1);

    v1.edges.assign({&e12, &e13, &e14});
    v2.edges.assign({&e12, &e24, &e25});
    v3.edges.assign({&e13, &e34, &e36});
    v4.edges.assign({&e14, &e24, &e34, &e45, &e46, &e47});
    v5.edges.assign({&e25, &e45, &e57});
    v6.edges.assign({&e36, &e46, &e67});
    v7.edges.assign({&e47, &e57, &e67});

    std::vector graph = {&v1, &v2, &v3, &v4, &v5, &v6, &v7};

    Prim(graph, &v1);

    for (auto vertex : graph) {
        cout << vertex->name << ": " << vertex->distance <<
            "(" << (vertex->prev == nullptr ? "null" : vertex->prev->name) << ")" << endl;
    }
    return 0;
}
