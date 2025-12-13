//
// Created by xyx on 2025/12/13.
//
#include "Graph.h"
#include <iostream>
#include <queue>

using std::cout;
using std::endl;

void bfs(Vertex *v) {
    std::queue<Vertex *> queue;
    queue.push(v);
    v->visited = true;
    while (!queue.empty()) {
        Vertex *cur = queue.front();
        queue.pop();
        cout << cur->name << " ";
        for (auto edge : cur->edges) {
            if (!edge.linked->visited) {
                queue.push(edge.linked);
                edge.linked->visited = true;
            }
        }
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
        Edge(&v6, 14)
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

    bfs(&v1);

    return 0;
}