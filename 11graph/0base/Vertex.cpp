//
// Created by xyx on 2025/12/13.
//

#include "Graph.h"

Vertex::Vertex(const std::string &name) : name(name) {}

Vertex::Vertex(std::string &&name) : name(std::move(name)){}

Vertex::Vertex(const char *name) : name(name) {}

std::string &Vertex::get_name() {
    return name;
}


