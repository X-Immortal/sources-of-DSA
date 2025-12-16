//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"

// struct Vertex
UndirectedGraph::Vertex::Vertex(const char *name) : b_Vertex(name) {
}

UndirectedGraph::Vertex::Vertex(const std::string &name) : b_Vertex(name) {
}

UndirectedGraph::Vertex::Vertex(std::string &&name) : b_Vertex(std::move(name)) {
}


// struct Edge
UndirectedGraph::Edge::Edge(Vertex *v1, Vertex *v2, int weight) : b_Edge(weight), v1(v1), v2(v2) {
}


// class UndirectedGraph
void UndirectedGraph::reset() {
    for (auto &v : vertices) {
        v.reset(vertices.size());
    }
}
