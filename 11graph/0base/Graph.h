//
// Created by xyx on 2025/12/13.
//

#ifndef ONLINE_CLASS_GRAPH_H
#define ONLINE_CLASS_GRAPH_H

#include <string>
#include <list>
#include <climits>

#include "Graph.h"

enum Status {
    NOT_VISITED,
    VISITING,
    VISITED
};

struct b_Vertex {
    static int id_counter;
    int id; // 顶点编号
    std::string name;

    // 用于遍历
    bool visited = false;

    // 用于拓扑排序
    int in_degree = 0; // 入度
    Status status = NOT_VISITED;

    // 用于最短路
    static constexpr int POS_INF = INT_MAX;
    int distance = POS_INF; // 初始值为正无穷大

    b_Vertex(const std::string &name, int id);
    b_Vertex(std::string &&name, int id);
    b_Vertex(const char *name, int id);
    virtual ~b_Vertex() = 0;
};

struct b_Edge {
    int weight; // 权重

    explicit b_Edge(int weight);
    virtual ~b_Edge() = 0;
};

namespace DG {
    struct Vertex;
    struct Edge;

    struct Vertex : public b_Vertex {
        std::list<Edge> edges; // 邻接表，存储与当前顶点相连的边

        Vertex *prev = nullptr; // 指示当前节点与源节点最短路径上的父节点

        static constexpr int POS_INF = b_Vertex::POS_INF;

        explicit Vertex(const std::string &name, int id = id_counter++);
        explicit Vertex(std::string &&name, int id = id_counter++);
        explicit Vertex(const char *name, int id = id_counter++);
        ~Vertex() override;
    };

    struct Edge : public b_Edge {
        Vertex *linked; // 边的终点

        explicit Edge(Vertex *linked, int weight = 1);
        ~Edge() override;
    };
}

namespace UDG {
    struct Vertex;
    struct Edge;

    struct Vertex : public b_Vertex {
        std::list<Edge> edges; // 邻接表，存储与当前顶点相连的边

        Vertex *prev = nullptr; // 指示当前节点与源节点最短路径上的父节点

        static constexpr int POS_INF = b_Vertex::POS_INF;

        explicit Vertex(const std::string &name, int id = id_counter++);
        explicit Vertex(std::string &&name, int id = id_counter++);
        explicit Vertex(const char *name, int id = id_counter++);
        ~Vertex() override;
    };

    struct Edge : public b_Edge {
        Vertex *v1;
        Vertex *v2;

        Edge(Vertex *v1, Vertex *v2, int weight = 1);
        ~Edge() override;
    };
}

#endif //ONLINE_CLASS_GRAPH_H