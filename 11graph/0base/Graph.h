//
// Created by xyx on 2025/12/13.
//

#ifndef ONLINE_CLASS_GRAPH_H
#define ONLINE_CLASS_GRAPH_H

#include <string>
#include <list>
#include <climits>

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

    // 用于遍历
    bool visited = false;

    // 用于拓扑排序
    int in_degree = 0; // 入度
    Status status = NOT_VISITED;

    // 用于最短路
    static constexpr int POS_INF = INT_MAX;
    int distance = POS_INF; // 初始值为正无穷大
    Vertex *prev = nullptr; // 指示当前节点与源节点最短路径上的父节点

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