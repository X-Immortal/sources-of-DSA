//
// Created by xyx on 2025/12/13.
//

#include "Graph.h"

int b_Vertex::id_counter = 0;

b_Vertex::b_Vertex(const std::string &name, int id) : id(id), name(name) {}

b_Vertex::b_Vertex(std::string &&name, int id) : id(id), name(std::move(name)) {}

b_Vertex::b_Vertex(const char *name, int id) : id(id), name(name) {}

b_Vertex::~b_Vertex() = default;

namespace UDG {
    Vertex::Vertex(const std::string &name, int id) : b_Vertex(name, id) {}

    Vertex::Vertex(std::string &&name, int id) : b_Vertex(std::move(name), id) {}

    Vertex::Vertex(const char *name, int id) : b_Vertex(name, id) {}

    Vertex::~Vertex() = default;
}

namespace DG {
    Vertex::Vertex(const std::string &name, int id) : b_Vertex(name, id) {}

    Vertex::Vertex(std::string &&name, int id) : b_Vertex(std::move(name), id) {}

    Vertex::Vertex(const char *name, int id) : b_Vertex(name, id) {}

    Vertex::~Vertex() = default;
}