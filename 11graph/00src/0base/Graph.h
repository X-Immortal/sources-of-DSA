//
// Created by xyx on 2025/12/13.
//

#ifndef ONLINE_CLASS_GRAPH_H
#define ONLINE_CLASS_GRAPH_H

#include <string>
#include <list>
#include <climits>
#include <vector>
#include <unordered_map>
#include <functional>

class Graph {
protected:
    enum Status {NOT_VISITED, VISITING, VISITED};

    struct b_Vertex {
        static int id_counter;
        const int id; // 顶点编号
        const std::string name;

        // 用于拓扑排序
        int in_degree = 0; // 入度
        Status status = NOT_VISITED;

        // 用于最短路
        static constexpr int POS_INF = INT_MAX;
        // 初始值为正无穷大
        std::vector<int> distance;

        template<typename T> requires std::constructible_from<std::string, T&&>
        explicit b_Vertex(T &&name) : id(id_counter++), name(std::forward<T>(name)) {
        }

        bool visited() const;
        bool visiting() const;
        void start_visit();
        void end_visit();

        void reset(int size);
    };

    struct b_Edge {
        int weight; // 权重

        explicit b_Edge(int weight);
    };

public:
    virtual ~Graph() = 0;

    virtual bool add_vertex(const std::string &name) = 0;
    virtual bool add_vertex(const std::initializer_list<std::string> &names) = 0;
    virtual bool link(const std::string &begin, const std::string &end, int weight = 1) = 0;

private:
    virtual void reset() = 0;
};

class DirectedGraph : public Graph {
    struct Edge;

    struct Vertex : b_Vertex {
        std::list<Edge> adjacency_list; // 邻接表，存储与当前顶点相连的边

        // 指示当前节点与源节点最短路径上的父节点
        std::vector<int> prev;

        template<typename T> requires std::constructible_from<std::string, T&&>
        explicit Vertex(T &&name) : b_Vertex(std::forward<T>(name)) {
        }

        void reset(int size);
        void link(int linked, int weight);
    };

    struct Edge : b_Edge {
        int linked; // 边的终点

        explicit Edge(int linked, int weight = 1);
    };

    std::vector<Vertex> vertices;
    std::unordered_map<std::string, int> vertex_map;

    friend int main();

public:
    bool add_vertex(const std::string &name) override;
    bool add_vertex(const std::initializer_list<std::string> &names) override;
    bool link(const std::string &begin, const std::string &end, int weight = 1) override;

    std::vector<std::string> topological_sort_Kahn();
    std::vector<std::string> topological_sort_dfs();

    std::vector<std::pair<std::string, std::pair<int, std::string>>>
    Dijkstra(const std::string &start);

private:
    void dfs_rec_do(int start, const std::function<void(Vertex *)> &callback, bool before);
    void dfs_rec(int start, const std::function<void(Vertex *)> &callback);
    void dfs_loop(int start, const std::function<void(Vertex *)> &callback);

    void bfs(int start, const std::function<void(Vertex *)> &callback);

    void Dijkstra(int start);

    void reset() override;

    Vertex *v(int id) const;
};

class UndirectedGraph : public Graph {
    struct Edge;

    struct Vertex : b_Vertex {
        std::list<Edge *> adjacency_list; // 邻接表，存储与当前顶点相连的边

        // 指示当前节点与源节点最短路径上的父节点
        std::vector<Vertex *> prev;

        explicit Vertex(const std::string &name);
        explicit Vertex(std::string &&name);
        explicit Vertex(const char *name);
    };

    struct Edge : b_Edge {
        Vertex *v1;
        Vertex *v2;

        Edge(Vertex *v1, Vertex *v2, int weight = 1);
    };

    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::unordered_map<std::string, int> vertex_map;

public:

private:
    void reset() override;
};

#endif //ONLINE_CLASS_GRAPH_H