//
// Created by xyx on 2025/12/13.
//
#include "Graph.h"
#include <queue>

void DirectedGraph::bfs(int start, const std::function<void(Vertex *)> &callback) {
    reset();
     auto vertex = v(start);
     std::queue<Vertex *> queue;
     queue.push(vertex);
     vertex->end_visit();
     while (!queue.empty()) {
         Vertex *cur = queue.front();
         queue.pop();
         callback(cur);
         for (auto &edge : cur->adjacency_list) {
             if (!v(edge.linked)->visited()) {
                 queue.push(v(edge.linked));
                 v(edge.linked)->end_visit();
             }
         }
     }
}
