//
// Created by xyx on 2025/12/13.
//
#include "Graph.h"
#include <iostream>
#include <stack>

using std::cout;
using std::endl;

void DirectedGraph::dfs_rec_do(int start, const std::function<void(Vertex *)> &callback, bool before) {
    auto vertex = v(start);
    if (vertex->visited()) {
        return;
    }
    if (vertex->visiting()) {
        throw std::runtime_error("there is cycle in the graph");
    }

    vertex->start_visit();
    if (before) {
        callback(vertex);
    }
    for (auto &edge : vertex->adjacency_list) {
        dfs_rec_do(edge.linked, callback, before);
    }
    if (!before) {
        callback(vertex);
    }
    vertex->end_visit();
}

void DirectedGraph::dfs_loop(int start, const std::function<void(Vertex *)> &callback) {
    reset();

    std::stack<Vertex *> stack;
    stack.push(v(start));

    while (!stack.empty()) {
        Vertex *cur = stack.top();
        stack.pop();
        cur->end_visit();
        callback(cur);
        for (auto &edge : cur->adjacency_list) {
            if (!v(edge.linked)->visited()) {
                stack.push(v(edge.linked));
            }
        }
    }
}

