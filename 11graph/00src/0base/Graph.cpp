//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"

// struct b_Vertex
int Graph::b_Vertex::id_counter = 0;

bool Graph::b_Vertex::visited() const {
    return status == VISITED;
}

bool Graph::b_Vertex::visiting() const {
    return status == VISITING;
}

void Graph::b_Vertex::start_visit() {
    status = VISITING;
}

void Graph::b_Vertex::end_visit() {
    status = VISITED;
}

void Graph::b_Vertex::reset(int size) {
    in_degree = 0;
    status = NOT_VISITED;
    distance.assign(size, POS_INF);
}


// struct b_Edge
Graph::b_Edge::b_Edge(int weight) : weight(weight) {
}


// class Graph
Graph::~Graph() = default;
