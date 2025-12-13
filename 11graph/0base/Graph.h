//
// Created by xyx on 2025/12/13.
//

#ifndef ONLINE_CLASS_GRAPH_H
#define ONLINE_CLASS_GRAPH_H

#include <string>
#include <list>

struct Vertex;
struct Edge;

enum Status {
    NOT_VISITED,
    VISITING,
    VISITED
};

struct Vertex {
    std::string name;
    std::list<Edge> edges; // 邻接表，存储与当前顶点相连的边
    bool visited = false;
    int in_degree = 0;
    Status status = NOT_VISITED;

public:
    Vertex(const std::string &name);

    Vertex(std::string &&name);

    Vertex(const char *name);

    std::string &get_name();

private:
};

struct Edge {
    Vertex *linked; // 边的终点
    int weight; // 权重

public:
    Edge(Vertex *linked);

    Edge(Vertex *linked, int weight);

private:
};

#endif //ONLINE_CLASS_GRAPH_H