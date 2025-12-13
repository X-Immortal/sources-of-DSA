//
// Created by xyx on 2025/12/13.
//

#include "Graph.h"

Edge::Edge(Vertex *linked) : linked(linked), weight(1) {}

Edge::Edge(Vertex *linked, int weight) : linked(linked), weight(weight) {}
