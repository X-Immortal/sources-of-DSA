//
// Created by xyx on 2025/12/16.
//
#include <stdexcept>

#include "Graph.h"

// struct Vertex
void UndirectedGraph::Vertex::add_edge(int index) {
    adjacency_list.emplace_back(index);
}


// struct Edge
UndirectedGraph::Edge::Edge(int v1, int v2, int weight) : b_Edge(weight), v1(v1), v2(v2) {
}


// class UndirectedGraph
void UndirectedGraph::reset() {
    for (auto &v : vertices) {
        v.reset(vertices.size());
    }
}

UndirectedGraph::Vertex *UndirectedGraph::v(int id) const {
    if (id < 0 || id >= vertices.size()) {
        throw std::out_of_range("id out of range");
    }
    return const_cast<Vertex *>(&vertices[id]);
}

UndirectedGraph::Edge * UndirectedGraph::e(int index) const {
    if (index < 0 || index >= edges.size()) {
        throw std::out_of_range("id out of range");
    }
    return const_cast<Edge *>(&edges[index]);
}

bool UndirectedGraph::add_vertex(const std::string &name) {
    if (vertex_map.contains(name)) {
        return false;
    }

    vertices.emplace_back(name);
    vertex_map.emplace(name, vertices.size() - 1);
    return true;
}

bool UndirectedGraph::add_vertex(const std::initializer_list<std::string> &names) {
    for (auto &name : names) {
        if (!add_vertex(name)) {
            return false;
        }
    }
    return true;
}

bool UndirectedGraph::link(const std::string &begin, const std::string &end, int weight) {
    try {
        int begin_id = vertex_map.at(begin);
        int end_id = vertex_map.at(end);
        edges.emplace_back(begin_id, end_id, weight);
        vertices[begin_id].add_edge(edges.size() - 1);
        vertices[end_id].add_edge(edges.size() - 1);
        return true;
    } catch (std::out_of_range &) {
        return false;
    }
}
