//
// Created by xyx on 2025/12/16.
//
#include "Graph.h"
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif
#include <vector>

using std::cout;
using std::endl;

int main() {
#ifdef WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    DirectedGraph graph;
    graph.add_vertex({
        "网页基础", "Java基础", "JavaWeb",
        "Spring框架", "微服务框架", "数据库",
        "实战项目"
    });

    graph.link("网页基础", "JavaWeb");
    graph.link("Java基础", "JavaWeb");
    graph.link("JavaWeb", "Spring框架");
    graph.link("数据库", "Spring框架");
    graph.link("Spring框架", "微服务框架");
    graph.link("微服务框架", "实战项目");
    // graph.link("实战项目", "微服务框架"); // 构造环

    try {
        auto &&result = graph.topological_sort_Kahn();
        for (auto &name : result) {
            cout << name << " ";
        }
        cout << endl;

        auto &&result2 = graph.topological_sort_dfs();
        for (auto &name : result2) {
            cout << name << " ";
        }
    } catch (std::runtime_error &e) {
        std::cerr << e.what() << endl;
        return -1;
    }

    return 0;
}
