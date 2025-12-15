//
// Created by xyx on 2025/12/13.
//

#include "Graph.h"

b_Edge::b_Edge(int weight) : weight(weight) {}

b_Edge::~b_Edge() = default;


namespace UDG {
    Edge::Edge(Vertex *v1, Vertex *v2, int weight) : v1(v1), v2(v2), b_Edge(weight) {}

    Edge::~Edge() = default;
}

namespace DG {
    Edge::Edge(Vertex *linked, int weight) : linked(linked), b_Edge(weight) {}

    Edge::~Edge() = default;
}