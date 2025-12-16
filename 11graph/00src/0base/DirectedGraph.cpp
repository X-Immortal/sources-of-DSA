//
// Created by xyx on 2025/12/16.
//
#include <algorithm>
#include <stdexcept>

#include "Graph.h"

// struct Vertex
void DirectedGraph::Vertex::reset(int size) {
    b_Vertex::reset(size);
    prev.assign(size, -1);
}

void DirectedGraph::Vertex::link(int linked, int weight) {
    adjacency_list.emplace_back(linked, weight);
}

// struct Edge
DirectedGraph::Edge::Edge(int linked, int weight) : b_Edge(weight), linked(linked) {
}


// class DirectedGraph
bool DirectedGraph::add_vertex(const std::string &name) {
    if (vertex_map.contains(name)) {
        return false;
    }

    vertices.emplace_back(name);
    vertex_map.insert({name, vertices.size() - 1});
    return true;
}

bool DirectedGraph::add_vertex(const std::initializer_list<std::string> &names) {
    for (auto &name : names) {
        if (!add_vertex(name)) {
            return false;
        }
    }
    return true;
}

bool DirectedGraph::link(const std::string &begin, const std::string &end, int weight) {
    try {
        int begin_id = vertex_map.at(begin);
        int end_id = vertex_map.at(end);
        vertices[begin_id].link(end_id, weight);
        return true;
    } catch (std::out_of_range &) {
        return false;
    }
}

void DirectedGraph::reset() {
    for (auto &v : vertices) {
        v.reset(vertices.size());
    }
}

DirectedGraph::Vertex *DirectedGraph::v(int id) const {
    if (id < 0 || id >= vertices.size()) {
        throw std::out_of_range("id out of range");
    }
    return const_cast<Vertex *>(&vertices[id]);
}

void DirectedGraph::dfs_rec(int start, const std::function<void(Vertex *)> &callback) {
    reset();
    dfs_rec_do(start, callback, true);
}

std::vector<std::pair<std::string, std::pair<int, std::string>> >
DirectedGraph::Dijkstra(const std::string &start) {
    try {
        int start_id = vertex_map.at(start);
        Dijkstra(start_id);
        std::vector<std::pair<std::string, std::pair<int, std::string>>> result;
        result.reserve(vertices.size());
        auto &dist = vertices[start_id].distance;
        auto &&prev = vertices[start_id].prev;
        for (int i = 0; i < vertices.size(); i++) {
            result.emplace_back(vertices[i].name, std::make_pair(dist[i], prev[i] == -1 ? "null" : vertices[prev[i]].name));
        }
        return result;
    } catch (std::out_of_range &) {
        return {};
    }
}
