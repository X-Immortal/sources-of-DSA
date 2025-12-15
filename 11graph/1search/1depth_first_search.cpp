//
// Created by xyx on 2025/12/13.
//
#include "Graph.h"
#include <iostream>
#include <stack>

using std::cout;
using std::endl;

void dfs_rec(Vertex *v) {
    v->visited = true;
    cout << v->name << " ";
    for (auto &edge : v->edges) {
        if (!edge.linked->visited) {
            dfs_rec(edge.linked);
        }
    }
}

void dfs_loop(Vertex *v) {
    std::stack<Vertex *> stack;
    stack.push(v);

    while (!stack.empty()) {
        Vertex *cur = stack.top();
        stack.pop();
        cur->visited = true;
        cout << cur->name << " ";
        for (auto &edge : cur->edges) {
            if (!edge.linked->visited) {
                stack.push(edge.linked);
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

    dfs_rec(&v1);
    v1.visited = v2.visited = v3.visited = v4.visited = v5.visited = v6.visited = false;
    cout << endl;
    dfs_loop(&v1);

    return 0;
}
